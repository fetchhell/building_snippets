#include "snippet_builder.h"

/* Parameters to change */
#define max_number_of_sentences 10
#define max_number_of_words     35
#define radius					3

//---------------------------------------------------------
bool check(char ch)
{
	if(    ch == ' ' || ch == ',' || ch == '\t'|| 
		   ch == '\n'|| ch == ';' || ch == '(' || 
		   ch == ')' || ch == '\"'|| ch == '-' ||
		   ch == '\r' ||ch == '\'' || ch == '*' ||
		   ch == '{' || ch == '}' || ch == '[' ||
		   ch == ']' || ch == '\\' || ch== '/'|| 
		   ch == '<' || ch == '>' || ch == '|' ||
		   ch == '+' || ch == '=' || ch == '^' ||
		   ch == '&' || ch == ':' || ch == '¹' ||
		   ch == '#' || ch == '`' || ch == '%' ||
		   ch == '$'
    )
	 return 1;
	return 0;
}

//---------------------------------------------------------
bool check_point(char ch)
{
	if(ch == '.' || ch == '!' ||
			    ch == '?')
				return 1;
	return 0;
}

//---------------------------------------------------------
void Snippet_builder::parse_on_sentences(string sentence){

	int number_of_sentences = 1, number_of_words = 0;
	sentences_not_changed.resize(number_of_sentences);
	sentences_full_not_changed.resize(number_of_sentences);

	string word = "", ssentence = "";
	int word_position_in_sentence = -1;
	int sentence_length = sentence.length();
	for(int i = 1; i < sentence_length; i++)
	{
		if(ssentence.empty() && sentence[i - 1] == ' ');
		else
		{
		   ssentence += sentence[i - 1];
		   word_position_in_sentence++;
		}

		if(
			check(sentence[i - 1])
		   )
		{
			if(!word.empty())
			{
				sentences_not_changed[number_of_sentences - 1].push_back(word);
				positions_in_not_changed_sentences[number_of_sentences - 1].push_back(position_length(word_position_in_sentence - word.length(), word.length()));

			    word = "";
				number_of_words++;
			}

			continue;
		}

		while  (check_point(sentence[i - 1]))
		{
			if(!word.empty())
			{
				sentences_not_changed[number_of_sentences - 1].push_back(word);
				positions_in_not_changed_sentences[number_of_sentences - 1].push_back(position_length(word_position_in_sentence - word.length(), word.length()));
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

				number_of_words = 0; word_position_in_sentence = -1; ssentence = ""; 
				break;
			}

			i++;
		}

		if(sentence[i - 1] == '.' || sentence[i - 1] == '!' ||
		   sentence[i - 1] == '?') continue;

		word += sentence[i - 1];
	}

	if(!ssentence.empty())
	{
		ssentence += sentence[sentence_length - 1];
		sentences_full_not_changed[number_of_sentences - 1].push_back(ssentence);
	}
	
	if(!word.empty())
	{
		if(
			!check(sentence[sentence_length - 1]) && !check_point(sentence[sentence_length - 1])
		   )
		{
			word += sentence[sentence_length - 1];
			word_position_in_sentence++;
		}

		sentences_not_changed[number_of_sentences - 1].push_back(word);
		positions_in_not_changed_sentences[number_of_sentences - 1].push_back(position_length((word_position_in_sentence + 1) - word.length(), word.length()));

		number_of_words++;
		number_of_words_in_sentences.push_back(number_of_words);

		number_of_words = 0;
	}

	/* Check, if empty, set zero */
	if(number_of_words > 0)
		number_of_words_in_sentences.push_back(number_of_words);

	if(number_of_words_in_sentences.empty())
		number_of_words_in_sentences.push_back(0);

	if(sentences_full_not_changed.size() == 1)	
		if(sentences_full_not_changed[0].empty())
			sentences_full_not_changed[0].push_back("");
}

//---------------------------------------------------------
void Snippet_builder::parse_changed_sentences(string sentence){

	int number_of_sentences = 1;
	sentences_changed.resize(number_of_sentences);
	sentences_changed_vector.resize(number_of_sentences);

	string word = "";

	int nsentences = number_of_words_in_sentences.size();
	int sentence_length = sentence.length();
	for(int i = 0; i < sentence_length; i++)
	{
		if(number_of_sentences > nsentences)
			break;

		if(sentences_changed[number_of_sentences - 1].size() == number_of_words_in_sentences[number_of_sentences - 1])
		{
			if(!word.empty()){
				sentences_changed[number_of_sentences - 1].insert(word);
				sentences_changed_vector[number_of_sentences - 1].push_back(word);
			    word = "";
			}

			number_of_sentences++;
			sentences_changed.resize(number_of_sentences);
			sentences_changed_vector.resize(number_of_sentences);
		}

		if(sentence[i] == ' ' || sentence[i] == '\t'
			|| sentence[i] == '\n' || sentence[i] == '\r')
		{
			if(!word.empty()){
				sentences_changed[number_of_sentences - 1].insert(word);
				sentences_changed_vector[number_of_sentences - 1].push_back(word);
			    word = "";
			}
			continue;
		}
		
		word += sentence[i];
	}

	if(!word.empty())
	{
		sentences_changed[number_of_sentences - 1].insert(word);
		sentences_changed_vector[number_of_sentences - 1].push_back(word);
	}
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

	 map<string, vector<int> > nmeets_search_words;

     map<int, vector<int> >::iterator it;
	 for(it = sentence_query.begin(); it != sentence_query.end(); it++)
	 {
		 int number_of_search_words = it->second.size();
		 for(int j = 0; j < number_of_search_words; j++)
		 {
			vector<int> *ptr = &nmeets_search_words[qquery_words[it->second[j]]];
			if(ptr->empty())
				ptr->push_back(0);
			(*ptr)[0]++;
		 }
	 }

	 int number_of_sentences = number_of_words_in_sentences.size();

	 for(it = sentence_query.begin(); it != sentence_query.end(); it++)
	 {
		 tf_idf[it->first] = 0.0f;
		 int N = sentences_changed[it->first].size();

		 int number_of_search_words = it->second.size();
		 for(int j = 0; j < number_of_search_words; j++)
		 {
			 int n = sentences_changed[it->first].count(qquery_words[it->second[j]]);
			 float tf = n / (float) N;

			 tf_idf[it->first] += ( tf * log( number_of_sentences / (float) nmeets_search_words[qquery_words[it->second[j]]][0] ) );
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
void Snippet_builder::find_search_words_positions(vector<int>& ndocuments){

	vector<int>::iterator it_v;
	for(it_v = ndocuments.begin(); it_v != ndocuments.end(); it_v++)
	{
		int sz = sentences_changed_vector[*it_v].size();
		for(int i = 0; i < sz; i++)
		{
			vector<int>::iterator it_sq;
			for(it_sq = sentence_query[*it_v].begin(); it_sq != sentence_query[*it_v].end(); it_sq++)			
				if(qquery_words[*it_sq] == sentences_changed_vector[*it_v][i])
				{
					vector<position_length>::iterator it = find(searchWord_positions[*it_v].begin(), searchWord_positions[*it_v].end(), positions_in_not_changed_sentences[*it_v][i]);

					if(it == searchWord_positions[*it_v].end())
						searchWord_positions[*it_v].push_back(positions_in_not_changed_sentences[*it_v][i]);
				}
		}
	}
}

//---------------------------------------------------------
vector<int>* Snippet_builder::calculate_which_words_will_stay_in_snippet(int document_number)
{	
	int number_of_words_in_sentence = positions_in_not_changed_sentences[document_number].size();
	int number_of_search_words_in_sentence = searchWord_positions[document_number].size();
	
	vector<int>* exists_positions = new vector<int>(number_of_words_in_sentence, 0);

	/* Find, which positions still stay in snippets */
	int k = 0;
 	for(int i = 0; i < number_of_words_in_sentence; i++)
	{
		if(!searchWord_positions[document_number].empty())
			if(searchWord_positions[document_number][k].pos == positions_in_not_changed_sentences[document_number][i].pos)
			{
				(*exists_positions)[i] = 1;

				/* 1. Limits >= 0 */
				int offset_inf = i - radius;
				if(offset_inf >= 0)
					(*exists_positions)[offset_inf] = 1;
				if(offset_inf + 1 >= 0)
					(*exists_positions)[offset_inf + 1] = 1;
				if(offset_inf + 2 >= 0)
					(*exists_positions)[offset_inf + 2] = 1;

				/* 2. Limits < number_of_words_in_sentence */
				int offset_sup  = i + radius;
				if(offset_sup < number_of_words_in_sentence)
					(*exists_positions)[offset_sup] = 1;
				if(offset_sup - 1 < number_of_words_in_sentence)
					(*exists_positions)[offset_sup - 1] = 1;
				if(offset_sup - 2 < number_of_words_in_sentence)
					(*exists_positions)[offset_sup - 2] = 1;

				k++;
			}
		if(k >= number_of_search_words_in_sentence) break;
	}

	return exists_positions;
}

//---------------------------------------------------------
string Snippet_builder::parse_snippets_with_tags(int document_number, int first_document, int& number_of_words, string& snippet){

	string output = "";

	int number_of_words_in_sentence = positions_in_not_changed_sentences[document_number].size();
	int number_of_search_words_in_sentence = searchWord_positions[document_number].size();

	vector<int>* exists_positions = calculate_which_words_will_stay_in_snippet(document_number);

	/* Make intervals */
	vector<pair<int, int> > *start_stop = new vector<pair<int, int> >();
	int start = -1, stop = -1;
	for(int i = 0; i < number_of_words_in_sentence; i++)
	{
		if((*exists_positions)[i] && start < 0)
		{
			start = positions_in_not_changed_sentences[document_number][i].pos;
			stop  = -1;
		}
		else if(( !(*exists_positions)[i] && stop < 0 && start >= 0 ) || ( number_of_words >= max_number_of_words ))
		{
			if(start < 0) 
				break;

			stop = positions_in_not_changed_sentences[document_number][i].pos;
			 
			(*start_stop).push_back(pair<int, int>(start, stop));
			start = -1;
		}

		if((*exists_positions)[i]) 
			number_of_words++;
	}
	if(start >= 0 && stop < 0)	
		(*start_stop).push_back(pair<int, int>(start, snippet.length()));

	delete exists_positions;

	/* Use intervals to parse sentence */
	int snippet_length = snippet.length();

	if((*start_stop)[0].first && document_number == first_document)
		output += "... ";

	int start_stop_size = (*start_stop).size();
	for(int i = 0; i < start_stop_size; i++)	
		output += snippet.substr((*start_stop)[i].first, (*start_stop)[i].second - (*start_stop)[i].first);	
	
	output += " ... ";

	// if(number_of_words >= max_number_of_words)
	//	output += "<cutting>";

	delete start_stop;
	return output;
}

//---------------------------------------------------------
void Snippet_builder::recount_positions(int document_number){

	int number_of_search_words_in_sentence = searchWord_positions[document_number].size();
 	for(int i = 0; i < number_of_search_words_in_sentence; i++)
	{
		vector<position_length> *ptr = &positions_in_not_changed_sentences[document_number];

		vector<position_length>::iterator it = 
		find(ptr->begin(), ptr->end(), searchWord_positions[document_number][i]);

		if(it == ptr->end()) continue;

		for(int j = i + 1; j < number_of_search_words_in_sentence; j++)
			searchWord_positions[document_number][j].pos += 7;

		searchWord_positions[document_number][i].len += 7;	

		it->len += 7; it++;

		for(;it != ptr->end(); it++)
			it->pos += 7;
	}
}

//---------------------------------------------------------
void Snippet_builder::make_snippets(string& output){

	if(tf_idf_sorted.empty() && !qquery_words.empty())
	{
		int number_of_words = 0;
		int number_of_sentences = sentences_full_not_changed.size();

		string snippet = "";
		int i;
		for(i = 0; i < number_of_sentences; i++)
		{ 
			number_of_words += number_of_words_in_sentences[i];
			if(number_of_words >= max_number_of_words)
			{
				int offset = number_of_words - max_number_of_words;
				snippet += sentences_full_not_changed[i][0].substr(0, positions_in_not_changed_sentences[i][positions_in_not_changed_sentences[i].size() - offset].pos);

				break;
			}

			snippet += sentences_full_not_changed[i][0];
		}

		output += snippet;

		int common_number_of_words = 0;
		for(int j = 0; j < number_of_sentences; j++)
			common_number_of_words += number_of_words_in_sentences[j];

		if(i < number_of_sentences && number_of_words < common_number_of_words)
			output += " ... ";

		return;
	}

	/* Sorting numbers of the documents */
	vector<int> tf_idf_;

	int i = 0;
	vector<pair<float, int> >::iterator it;
	for(it = tf_idf_sorted.begin(); it != tf_idf_sorted.end() && i < max_number_of_sentences; it++, i++)	
		tf_idf_.push_back(it->second);
	
	sort(tf_idf_.begin(), tf_idf_.end());

	/* Find positions of the search words */
	find_search_words_positions(tf_idf_);
    int sz = tf_idf_.size();

	/* Adding tags <b> </b> */
	int number_of_words = 0;
	string snippet = "";

	for(i = 0; i < sz; i++)
	{
		int start_position = 0;

		int npositions = searchWord_positions[tf_idf_[i]].size();
		string tail    = sentences_full_not_changed[tf_idf_[i]][0];
		for(int j = 0; j < npositions; j++)
		{
			position_length* ptr = &searchWord_positions[tf_idf_[i]][j];

			string head        = sentences_full_not_changed[tf_idf_[i]][0].substr(start_position, ptr->pos - start_position);
			string search_word = sentences_full_not_changed[tf_idf_[i]][0].substr(ptr->pos, ptr->len);
			tail               = sentences_full_not_changed[tf_idf_[i]][0].substr(ptr->pos + ptr->len, sentences_full_not_changed[tf_idf_[i]][0].length() - (ptr->pos + ptr->len));

			snippet += head + "<b>" + search_word + "</b>";
			start_position = ptr->pos + ptr->len;
		}

		if(!tail.empty())
			snippet += tail;

		recount_positions(tf_idf_[i]);

		/* Parse snippet with tags */
		output += parse_snippets_with_tags(tf_idf_[i], tf_idf_[0], number_of_words, snippet);

		snippet = "";
		if(number_of_words >= max_number_of_words) break;
	}

	output += " ]";
}