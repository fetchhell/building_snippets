#include "snippet_builder.h"

//---------------------------------------------------------
void Snippet_builder::parse_on_sentences(string sentence){

	int number_of_sentences = 1, number_of_words = 0;
	sentences_not_changed.resize(number_of_sentences);
	sentences_full_not_changed.resize(number_of_sentences);

	string word = "", ssentence = "";
	int sentence_length = sentence.length();
	for(int i = 1; i < sentence_length; i++)
	{
		ssentence += sentence[i - 1];

		if(sentence[i - 1] == ' ' || sentence[i - 1] == ',' || sentence[i - 1] == '\t'|| 
		   sentence[i - 1] == '\n'|| sentence[i - 1] == ';' || sentence[i - 1] == '(' || 
		   sentence[i - 1] == ')' || sentence[i - 1] == '\"'|| sentence[i - 1] == '-'
		   )
		{
			if(!word.empty())
			{
				sentences_not_changed[number_of_sentences - 1].push_back(word);
			    word = "";

				number_of_words++;
			}

			continue;
		}

		while  (sentence[i - 1] == '.' || sentence[i - 1] == '!' ||
			    sentence[i - 1] == '?')
		{
			if(!word.empty())
			{
				sentences_not_changed[number_of_sentences - 1].push_back(word);
			    word = "";

				number_of_words++;
			}

			if( sentence[i] != '.' && sentence[i] != '!' &&
			    sentence[i] != '?')
			{
				if(!ssentence.empty())
				{
					sentences_full_not_changed[number_of_sentences - 1].push_back(ssentence);
					number_of_words_in_sentences.push_back(number_of_words);
				}

				number_of_sentences++;
				sentences_not_changed.resize(number_of_sentences);
				sentences_full_not_changed.resize(number_of_sentences);

				number_of_words = 0; ssentence = "";
				break;
			}

			i++;
		}

		if(sentence[i - 1] == '.' || sentence[i - 1] == '!' ||
		   sentence[i - 1] == '?') continue;

		word += sentence[i - 1];
	}

	if(!ssentence.empty())	
		sentences_full_not_changed[number_of_sentences - 1].push_back(ssentence);
	
	if(!word.empty())
	{
		sentences_not_changed[number_of_sentences - 1].push_back(word);

		number_of_words++;
		number_of_words_in_sentences.push_back(number_of_words);
	}
}

//---------------------------------------------------------
void Snippet_builder::parse_changed_sentences(string sentence){

	int number_of_sentences = 1;
	sentences_changed.resize(number_of_sentences);

	string word = "";

	int sentence_length = sentence.length();
	for(int i = 0; i < sentence_length; i++)
	{
		if(sentences_changed[number_of_sentences - 1].size() == number_of_words_in_sentences[number_of_sentences - 1])
		{
			if(!word.empty()){
				sentences_changed[number_of_sentences - 1].insert(word);
			    word = "";
			}

			number_of_sentences++;
			sentences_changed.resize(number_of_sentences);
		}

		if(sentence[i] == ' ' || sentence[i] == '\t'
			|| sentence[i] == '\n')
		{
			if(!word.empty()){
				sentences_changed[number_of_sentences - 1].insert(word);
			    word = "";
			}
			continue;
		}
		
		word += sentence[i];
	}

	if(!word.empty())
		sentences_changed[number_of_sentences - 1].insert(word);
}

//---------------------------------------------------------
void Snippet_builder::find_sentences(vector<string>& query_words)
{
	if(sentences_changed.empty() || query_words.empty()) return;

	/* Copy input query words */
	qquery_words = query_words;

    /* Search nessesary sentences */

    int number_of_sentences  = sentences_changed.size();
	int number_of_query_words = qquery_words.size();

	for(int i = 0; i < number_of_sentences; i++)
	{
		for(int j = 0; j < number_of_query_words; j++)
		{
			if(sentences_changed[i].end() != sentences_changed[i].find(qquery_words[j]))		
				sentence_query[i].push_back(j);
			
		  /* if find, push ¹ sentence and ¹ word in query in sentence_query */
		}
	}
}

//---------------------------------------------------------
void Snippet_builder::count_tf_idf(){

     map<int, vector<int> >::iterator it;
	 for(it = sentence_query.begin(); it != sentence_query.end(); it++)
	 {
		 tf_idf[it->first] = 0.0f;
		 int N = sentences_changed[it->first].size();

		 int number_of_search_words = it->second.size();
		 for(int j = 0; j < number_of_search_words; j++)
		 {
			 int n = sentences_changed[it->first].count(qquery_words[it->second[j]]);
			 float tf = n / (float) N;

			 tf_idf[it->first] += ( tf * log( 1.0f / tf ) );
		 }
	 }
}

//---------------------------------------------------------
bool compare (pair<float,int> i, pair<float,int> j) { return ( i.first > j.first ); }

//---------------------------------------------------------
void Snippet_builder::sort_rank(){

	map<int, float>::iterator it;
	for(it = tf_idf.begin(); it != tf_idf.end(); it++)	
		tf_idf_sorted.push_back(pair<float, int>(it->second, it->first));
	
	sort(tf_idf_sorted.begin(), tf_idf_sorted.end(), compare);
}

//---------------------------------------------------------
void Snippet_builder::print_snippets(string& output){

        vector<int> tf_idf_;

	int number_of_expected_sentences = 5, i = 0;
	vector<pair<float, int> >::iterator it;
	for(it = tf_idf_sorted.begin(); it != tf_idf_sorted.end() && i < number_of_expected_sentences; it++, i++)	
		tf_idf_.push_back(it->second);
	
	sort(tf_idf_.begin(), tf_idf_.end());
	int sz = tf_idf_.size();

	for(i = 0; i < sz; i++)	
		output += sentences_full_not_changed[tf_idf_[i]][0];

	output += " ]";
}
