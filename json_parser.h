#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <map>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class JSON_Parser
{
	public:
    
	/* Parse JSON */
    void parseJSON(char*);

	/* 
		Check, If word is digit - this is position, 
		If word is char* - this is a search word 
	*/
	bool check_isdigit(string);

	/* Get */
	vector<string>& getQueryWords()              { return query_words;    }
	map<int, vector<int> >& getDocId_Positions() { return docId_Positions;}

	private:

	/* Document id and positions of words in the document */
	map<int, vector<int> > docId_Positions;

	/* Query words, that program receives from JSON */
	vector<string>		   query_words;
};

#endif
