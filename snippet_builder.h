#ifndef BUILDER_H
#define BUILDER_H

#include <set>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <stdio.h>
#include <math.h>

using namespace std;

class Snippet_builder
{
	public:
	
	/* 1. Divide text on sentences, each sentence divide on words */
    void parse_on_sentences(string);
	void parse_changed_sentences(string);

	/* 2. Find sentences, which nessesary to calculate rank */
	void find_sentences(vector<string>&);

	/* 3. Sort rank of the sentences */
	void sort_rank();

	/* 4. Calculate tf-idf for each found sentence */
	void count_tf_idf();

	/* test function delete, if needed */
	void print_snippets(string&);

	private:

	/* sentences divide on words from not changed text */
	vector<vector<string> > sentences_not_changed;

	/* sentences from not changed text */
	vector<vector<string> > sentences_full_not_changed;

	/* sentences divide by words from changed text */
	vector<multiset<string> > sentences_changed;

	/* number of words in each sentence */
	vector<int> number_of_words_in_sentences;

	/* input query words */
	vector<string> qquery_words;

	/* map between ¹ sentence and query */
	map<int, vector<int> > sentence_query;

	/* tf-idf */
	map<int, float> tf_idf;

	/* tf-idf sorted */
	vector<pair<float, int> > tf_idf_sorted;
};

#endif
