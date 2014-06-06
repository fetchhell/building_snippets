#include "snippet_builder.h"
#include "json_parser.h"
#include <libpq-fe.h>

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define INPUT_NAME  "json_input"
#define OUTPUT_NAME "json_output"
#define BUFSIZE        1000000

int main()
{
	/* Connection to DB works while program running */
	PGconn     *conn;
	PGresult   *res;

	const char *conninfo = "dbname = search_db password = search user = search_user";
	conn = PQconnectdb(conninfo);

	if (PQstatus(conn) != CONNECTION_OK)
	{
           printf("Connection to database failed: %s", PQerrorMessage(conn));
           return 0;
	}

        /* Output snippet */
	string output_snippet = "[ ";

        char buf[BUFSIZE];
        int fd;
	do {

          /* Launch FIFO */
          if ( mkfifo(INPUT_NAME, 0777) ) {
		perror("mkfifo");
		return 1;
          }
          printf("%s is created\n", INPUT_NAME);
        
          /* Read input JSON */
          if ( (fd = open(INPUT_NAME, O_RDONLY)) <= 0 ) {
	      perror("open");
	      return 1;
          }

          printf("%s is opened\n", INPUT_NAME);  

          memset(buf, '\0', BUFSIZE);
          read(fd, buf, BUFSIZE-1);

          perror("read");
          close(fd);
          remove(INPUT_NAME);

          /* Parse JSON */
          JSON_Parser     jp;
	  jp.parseJSON(buf);

	  map<int, vector<int> >::iterator itMap, next_itMap;
	  next_itMap = jp.getDocId_Positions().begin();

	  for(itMap = jp.getDocId_Positions().begin(); itMap != jp.getDocId_Positions().end(); itMap++)
	  {
			char query[256];
			sprintf(query, "SELECT news_text, data_type FROM news WHERE id = %d", itMap->first);

			res = PQexec(conn, query);

			int nnews_text = PQfnumber(res, "news_text"); 
			int ndata_type = PQfnumber(res, "data_type"); 

			string snews_text, sdata_type;
			snews_text =  PQgetvalue(res, 0, nnews_text);
			sdata_type =  PQgetvalue(res, 0, ndata_type);

			Snippet_builder sb;

			/* 1. Divide text on sentences, each sentence divide on words */
			sb.parse_on_sentences(snews_text);
			sb.parse_changed_sentences(sdata_type);

			/* 2. Find sentences, which nessesary to calculate rank */
			sb.find_sentences(jp.getQueryWords());

			/* 3. Calculate tf-idf for each found sentence */
			sb.count_tf_idf();

			/* 4. Sort rank of the sentences */
			sb.sort_rank();

			/* 5. Print found snippets */
			char DocId[256];
			sprintf(DocId, "[ %d , ", itMap->first);

		        output_snippet += DocId;
			sb.make_snippets(output_snippet);

                        next_itMap++;
			if(next_itMap != jp.getDocId_Positions().end())
				output_snippet += ", ";		
	  }

          output_snippet += " ]";

          /* Discard old snippets */
	  FILE* f = fopen("json_output", "w");
	  fclose(f);

          /* Write output JSON */
          if ( (fd = open(OUTPUT_NAME, O_WRONLY)) <= 0 ) {
                 perror("open");
                 return 1;
          } 

          write(fd, output_snippet.c_str(), output_snippet.size());    
          close(fd);
          perror("write");

          printf("//---------------------\n");

          output_snippet = "[ ";
              
	} while(1);

	return 0;
}
