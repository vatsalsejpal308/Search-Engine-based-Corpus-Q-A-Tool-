// Do NOT add any other includes
#include "dict.h"

Dict::Dict(int capacity = 400000){
    dict = new hashmap(capacity);
    cap = capacity;
}

Dict::~Dict(){
    delete dict;
}

bool is_allowed_character(char ch) {
    bool flag = (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9');
    return flag;
}

void Dict::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){
    sentence.push_back(' ');
    string word = "";
    for(char ch : sentence) {
        char c = tolower(ch);
        if(is_allowed_character(c)) {
            word.push_back(c);
        } else if(word.length() > 0) {
            dict -> insert(word , 1);
            word = "";
        }
    }
}

int Dict::get_word_count(string word){
    int count = dict -> get_val(word);
    return count;
}

void Dict::dump_dictionary(string filename){
    //later
}