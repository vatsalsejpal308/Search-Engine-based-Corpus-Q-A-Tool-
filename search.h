// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
using namespace std;

class SearchEngine {
private:
    vector<vector<string>> sentences;
    vector<vector<int>> locations;

public: 
    SearchEngine();

    ~SearchEngine();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    vector<vector<int>> search(string pattern);
};