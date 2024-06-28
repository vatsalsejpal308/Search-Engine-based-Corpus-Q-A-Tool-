// Do NOT add any other includes
#include "search.h"
SearchEngine::SearchEngine() {
    
}

SearchEngine::~SearchEngine() {
    
}

int get_sentence_count(vector<string> &sentence , string &pattern) {
    int count = 0;
    for(string word : sentence) {
        if(word == pattern) {
            count++;
        }
    }

    return count;
}

bool is_allowed_char(char ch) {
    bool flag = (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9');
    return flag;
}

void SearchEngine::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    vector<string> words;
    string word = "";
    for (char ch : sentence){
        char c = tolower(ch);
        if(is_allowed_char(c)) {
            word.push_back(c);
        } else if(word.length() > 0) {
            words.push_back(word);
            word = "";
        }
    }

    sentences.push_back(words);
    vector<int> location = {book_code, page, paragraph};
    locations.push_back(location);
}

vector<vector<int>> SearchEngine::search(string pattern){
    for(char c : pattern){
        c = tolower(c);
    }
    
    vector<vector<int>> ans;

    int index = 0 , n = sentences.size();
    while(index < n) {
        
        int book_code = locations[index][0];
        int page = locations[index][1];
        int paragraph = locations[index][2];

        int count = 0;

        while(index < n && (book_code == locations[index][0] && page == locations[index][1] && paragraph == locations[index][2])) {
            vector<string> sentence = sentences[index];
            int sentence_count = get_sentence_count(sentence , pattern);
            count += sentence_count;
            index++;
        }
        if(count > 0){
            vector<int> ans_element = {book_code , page , paragraph , count};
            ans.push_back(ans_element);
        }

        
    }

    return ans;
}