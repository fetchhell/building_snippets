1. Install postgres 9.3 => DB and tables https://github.com/akonit/search/blob/master/create_db.sql#L2 
2. Path of the libraries must be /opt/PostgreSQL/9.3/lib. If this path and path in your operating system are not the same, change it in Makefile 
3. Type "make" in console to deploy main program
4. Type "g++ -o mij make_input_json.cpp" in another console for testing main program
5. Type "./main", this is launch the main program
6. Type "./mij", in another console. It will pass to main program input JSON, which you will find in json_input.txt.

P.S. tables you can fill - 
     column 'news text' => sentence_not_changed.txt; 
     column 'data_type' => sentence_changed.txt;  
that lies in folder 'to_fill_DB'

