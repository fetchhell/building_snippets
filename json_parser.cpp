#include "json_parser.h"

//------------------------------------
void JSON_Parser::parseJSON(char* json_input_)
{
	string json_input = json_input_;

	int json_input_size = json_input.size();

	string doc_id      = "";
	int current_doc_id = 0;

	bool bracket_open = 0, bracket_close = 1;
	for(int i = 0; i < json_input_size; i++)
	{
		if(bracket_open)
		{
			/* Entries */
			string word = "";
			for(int j = i; j < json_input_size; j++)
			{
				if(json_input[j] == ' ' || json_input[j] == '[') 
					continue;
				else if(json_input[j] == ','
					             || json_input[j] == ']')
				{
					if(check_isdigit(word))
						docId_Positions[current_doc_id].push_back(atoi(word.c_str()));
					else 
						query_words.push_back(word);

					if(json_input[j] == ']')
					{
						i += abs(i - (j + 1));

						bracket_open  = !bracket_open;
			            bracket_close = !bracket_close;

						break;
					}
					word = "";
				}
				else 
					word += json_input[j];
			}
			continue;
		}

		if(json_input[i] == '[' 
			|| json_input[i] == ']')
		{
			bracket_open  = !bracket_open;
			bracket_close = !bracket_close;

			continue;
		}
 
		/* document id */
	    if(json_input[i] == ',')
		{
			if(!doc_id.empty())
			{
				current_doc_id = atoi(doc_id.c_str());
				doc_id = "";
			}
			continue;
		}

		if(json_input[i] != ' ' && json_input[i] != '\t' && json_input[i] != '\n')
			doc_id += json_input[i];
	}
}

//------------------------------------
bool JSON_Parser::check_isdigit(string word)
{
	int size_of_word = word.size();

	int ccount_digit = 0;
	for(int i = 0; i < size_of_word; i++)	
		if(('0' <= word[i] && word[i] <= '9') 
			|| (word[i] == '-' && !i)) ccount_digit++;
	
	if(ccount_digit != size_of_word)
		return 0;

	return 1;
}
