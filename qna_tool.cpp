#include <assert.h>
#include <sstream>
#include "qna_tool.h"
#include<cmath>

using namespace std;

QNA_tool::QNA_tool(){
    // Implement your function here  
    general_corpus = new hashmap(500000);
    inserted_corpus = new Dict(500000);

    insert_csv_data();
}

QNA_tool::~QNA_tool(){
    // Implement your function here  
    delete general_corpus;
    delete inserted_corpus;
}

void QNA_tool::insert_csv_data() {
    string file_name = "unigram_freq.csv";
    ifstream inputFile(file_name);
    string line;

    getline(inputFile, line);

    while (getline(inputFile, line)) {
        string word;
        string wordCount_str;

        istringstream lineStream(line);

        getline(lineStream, word, ',');
        getline(lineStream, wordCount_str);

        long long wordCount = stoll(wordCount_str);
        general_corpus->insert(word, wordCount);
    }

    inputFile.close();
}

void QNA_tool::insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence){ 
    inserted_corpus -> insert_sentence(book_code , page , paragraph , sentence_no , sentence);
    vector<string> words = get_words(sentence);
    if(data.size() == 0) {
        data.push_back({words});
        locn.push_back({book_code , page , paragraph});
    } else {
        vector<int> recent = locn[locn.size() - 1];
        if(recent[0] == book_code && recent[1] == page && recent[2] == paragraph) {
            data[data.size() - 1].push_back(words);
        } else {
            data.push_back({words});
            locn.push_back({book_code , page , paragraph});
        }
    }
}

bool QNA_tool::is_allowed_char(char ch) {
    bool flag = (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9');
    return flag;
}

vector<string> QNA_tool::get_words(string sentence) {
    sentence.push_back(' ');
    vector<string> words;
    string word = "";

    for(char ch : sentence) {
        ch = tolower(ch);
        if(is_allowed_char(ch)) {
            word.push_back(ch);
        } else if(word.length() > 0) {
            words.push_back(word);
            word = "";
        }
    }

    return words;
}

Node* QNA_tool::get_top_k_para(string question, int k) {
    question.push_back(' ');
    vector<double> scores;
    vector<string> query_words = get_words(question);
    for(auto paragraph : data) {
        double score = 0.0;
        for(auto query_word : query_words) {
            int count = 0;
            for(auto sentence : paragraph) {
                for(auto word : sentence) {
                    if(word == query_word) {
                        count++;
                    }
                }
            }
            long long a = inserted_corpus -> get_word_count(query_word) + 1;
            long long b = general_corpus -> get_val(query_word) + 1;
            double res = (static_cast<double>(a) * static_cast<double>(count) / static_cast<double>(b));
            score += res;
        }
        scores.push_back(score);
    }

    Node* sentinel = new Node(-1 , -1 , -1 , -1 , -1);
    Node* tail = sentinel;

    for(int i = 0 ; i < k ; i++) {
        double max_score = -1.0;
        int index;
        for(int j = 0 ; j < (int)locn.size() ; j++) {
            if(scores[j] > max_score) {
                max_score = scores[j];
                index = j;
            }
        }
        Node* node = new Node(locn[index][0] , locn[index][1] , locn[index][2] , -1 , -1);
        tail -> right = node;
        node -> left = tail;
        tail = node;
        scores[index] = -1.0;
    }

    Node* head = sentinel -> right;
    delete sentinel;
    sentinel = nullptr;

    Node* current = head;
    int i = 1;
    while(current && i < k) {
        current = current -> right;
        i++;
    }
    
    if(current) {
        current -> right = nullptr;
    }

    return head;

}

int no_of_para(string query_word , vector<vector<vector<string>>>& data) {
    int n = 0;
    for(auto paragraph : data) {
        bool found = false;
        for(auto sentence : paragraph) {
            for(auto word : sentence) {
                if(word == query_word) {
                    found = true;
                }
            }
        }
        if(found) {
            n++;
        }
    }
    return n;
}

Node* QNA_tool::get_top_k_para_ultra(string question, int k) {
    question.push_back(' ');
    vector<double> scores(data.size() , 0.0);
    vector<string> query_words = get_words(question);
    double s , idf , tf , tfidf;
    for(auto query_word : query_words) {
        s = log(static_cast<double>(inserted_corpus -> get_word_count(query_word) + 1) / static_cast<double>(general_corpus -> get_val(query_word) + 1));
        idf = log(static_cast<double>(data.size() + 1) / static_cast<double>(no_of_para(query_word , data) + 1));
        for(int i = 0 ; i < (int)data.size() ; i++) {
            vector<vector<string>> paragraph = data[i];
            int occurrences_in_para = 0;
            int words_in_para = 0;
            for(auto sentence : paragraph) {
                words_in_para += sentence.size();
                for(auto word : sentence) {
                    if(word == query_word) {
                        occurrences_in_para++;
                    }
                }
            }
            tf = static_cast<double>(occurrences_in_para + 1) / static_cast<double>(words_in_para + 1);
            tfidf = tf * idf;
            scores[i] += tfidf * s;
        }        
    }

    for(int i = 0 ; i < (int)data.size() ; i++) {
        vector<vector<string>> paragraph = data[i];
        int words_in_p = 0;
        for(auto sentence : paragraph) {
            words_in_p += sentence.size();
        }
        scores[i] = scores[i] / static_cast<double>(words_in_p);
    }

    Node* sentinel = new Node(-1 , -1 , -1 , -1 , -1);
    Node* tail = sentinel;

    for(int i = 0 ; i < k ; i++) {
        double max_score = -1.0;
        int index;
        for(int j = 0 ; j < (int)locn.size() ; j++) {
            if(scores[j] > max_score) {
                max_score = scores[j];
                index = j;
            }
        }
        Node* node = new Node(locn[index][0] , locn[index][1] , locn[index][2] , -1 , -1);
        tail -> right = node;
        node -> left = tail;
        tail = node;
        scores[index] = -1.0;
    }

    Node* head = sentinel -> right;
    delete sentinel;
    sentinel = nullptr;

    Node* current = head;
    int i = 1;
    while(current && i < k) {
        current = current -> right;
        i++;
    }
    
    if(current) {
        current -> right = nullptr;
    }

    return head;

}

void QNA_tool::deleteList(Node*& root) {
    Node* current = root , * forward = nullptr;
    while(current) {
        forward = current -> right;
        delete current;
        current = forward;
    }
}

string QNA_tool::read_gpt_response() {
    string filename = "response.txt";
    ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open the file " << filename << std::endl;
        return "";  // Return an empty string on error
    }

    stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}

void QNA_tool::query(string question, string filename){ 

    string API_KEY;

    if(filename == "api_call.py") {
        API_KEY = "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";
    } else {
        API_KEY = "vatsal-sejpal@llm-project-a7.iam.gserviceaccount.com";
    }

    string q1 = "For the given sentence : \"" + question + "\", give me only the important words in lowercase and avoid the typos. Please do not write anything else in the response";
    query_llm_chat("api_call.py" , API_KEY , q1);
    string a1 = read_gpt_response();


    string q2_1 = "I have a very huge corpus of collection of Gandhi's books. I have a question and its answer lies within the corpus. ";
    string q2_2 = "I can not feed the whole corpus to you and hence based on the question : \"" + question + "\" tell me how many paragraphs should ";
    string q2_3 = "I feed to you in order to get a very relevant answer. Your answer must be in range 3 - 5 (both included) and only respond with a number nothing else.";
    string q2 = q2_1 + q2_2 + q2_3;
    query_llm_chat("api_call.py" , API_KEY , q2);
    string a2 = read_gpt_response();


    int k = stoi(a2);
    Node* root = get_top_k_para_ultra(a1 , k);
    query_llm(filename , root , k , API_KEY , question);
    string a3 = read_gpt_response();
    deleteList(root);
}

std::string QNA_tool::get_paragraph(int book_code, int page, int paragraph){

    cout << "Book_code: " << book_code << " Page: " << page << " Paragraph: " << paragraph << endl;
    
    std::string filename = "mahatma-gandhi-collected-works-volume-";
    filename += to_string(book_code);
    filename += ".txt";

    std::ifstream inputFile(filename);

    std::string tuple;
    std::string sentence;

    if (!inputFile.is_open()) {
        std::cerr << "Error: Unable to open the input file " << filename << "." << std::endl;
        exit(1);
    }

    std::string res = "";

    while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
        // Get a line in the sentence
        tuple += ')';

        int metadata[5];
        std::istringstream iss(tuple);

        // Temporary variables for parsing
        std::string token;

        // Ignore the first character (the opening parenthesis)
        iss.ignore(1);

        // Parse and convert the elements to integers
        int idx = 0;
        while (std::getline(iss, token, ',')) {
            // Trim leading and trailing white spaces
            size_t start = token.find_first_not_of(" ");
            size_t end = token.find_last_not_of(" ");
            if (start != std::string::npos && end != std::string::npos) {
                token = token.substr(start, end - start + 1);
            }
            
            // Check if the element is a number or a string
            if (token[0] == '\'') {
                // Remove the single quotes and convert to integer
                int num = std::stoi(token.substr(1, token.length() - 2));
                metadata[idx] = num;
            } else {
                // Convert the element to integer
                int num = std::stoi(token);
                metadata[idx] = num;
            }
            idx++;
        }

        if(
            (metadata[0] == book_code) &&
            (metadata[1] == page) &&
            (metadata[2] == paragraph)
        ){
            res += sentence;
        }
    }

    inputFile.close();
    return res;
}

void QNA_tool::query_llm_chat(string filename, string API_KEY, string question) {
        // Write the query to query.txt
        filename = "api_call.py";
        std::ofstream outfile("query.txt");
        outfile << question;
        // You can add anything here - show all your creativity and skills of using ChatGPT
        outfile.close();

        // Call api_call.py with the provided query and store the response in response.txt
        int k = 0;
        std::string command = "python " + filename + " " + API_KEY + " " + std::to_string(k) + " query.txt";

        system(command.c_str());
    }

    void QNA_tool::query_llm(string filename, Node* root, int k, string API_KEY, string question) {
        // first write the k paragraphs into different files
        Node* traverse = root;
        int num_paragraph = 0;

        while (num_paragraph < k) {
            assert(traverse != nullptr);
            std::string p_file = "paragraph_" + std::to_string(num_paragraph) + ".txt";
            // delete the file if it exists
            remove(p_file.c_str());
            std::ofstream outfile(p_file);
            std::string paragraph = get_paragraph(traverse->book_code, traverse->page, traverse->paragraph);
            assert(paragraph != "$I$N$V$A$L$I$D$");
            outfile << paragraph;
            outfile.close();
            traverse = traverse->right;
            num_paragraph++;
        }

        // write the query to query.txt
        std::ofstream outfile("query.txt");
        outfile << "These are the excerpts from Mahatma Gandhi's books.\nOn the basis of this, ";
        outfile << question;
        // You can add anything here - show all your creativity and skills of using ChatGPT
        outfile.close();

        // you do not need to necessarily provide k paragraphs - can configure yourself

        // python <filename> API_KEY num_paragraphs query.txt
        std::string command;
        if(filename == "api_call.py") {
            command = "python " + filename + " " + API_KEY + " " + std::to_string(k) + " query.txt";
        } else { 
            command = "python " + filename;
        }

        system(command.c_str());
        return;
    }