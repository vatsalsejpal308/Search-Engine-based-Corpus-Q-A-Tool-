#include<bits/stdc++.h>
//#include "Node.h"
#include "qna_tool.h"

using namespace std;

int main(){

    QNA_tool qna_tool;

    int num_books = 98;

    for(int i = 1; i <= num_books; i++){

        // std::cout << "Inserting book " << i << std::endl;

        std::string filename = "mahatma-gandhi-collected-works-volume-";
        filename += to_string(i);
        filename += ".txt";

        std::ifstream inputFile(filename);

        if (!inputFile.is_open()) {
            std::cerr << "Error: Unable to open the input file mahatma-gandhi." << std::endl;
            return 1;
        }

        std::string tuple;
        std::string sentence;


        while (std::getline(inputFile, tuple, ')') && std::getline(inputFile, sentence)) {
            // Get a line in the sentence
            tuple += ')';

            std::vector<int> metadata;    
            std::istringstream iss(tuple);

            // Temporary variables for parsing
            std::string token;

            // Ignore the first character (the opening parenthesis)
            iss.ignore(1);

            // Parse and convert the elements to integers
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
                    metadata.push_back(num);
                } else {
                    // Convert the element to integer
                    int num = std::stoi(token);
                    metadata.push_back(num);
                }
            }

            // Now we have the string in sentence
            // And the other info in metadata
            // Add to the dictionary

            // Insert in the qna_tool
            qna_tool.insert_sentence(metadata[0], metadata[1], metadata[2], metadata[3], sentence);
        }

        inputFile.close();

    }
    cout<<"inserted all the books"<<endl;
    string question = "What were the views of Mahatma on Partition?";

    /*
        Part-1 : Get the top-k paragraphs
        Checking Correctness
    */

    // Let's try to ask a simple question to the qna_tool
    // Node* head = qna_tool.get_top_k_para(question, 5);
    // cout<<"got the top 5 paras nigga"<<endl;

    // vector<string> paras;
    // while(head != nullptr){
    //     string res = qna_tool.get_paragraph(head->book_code, head->page, head->paragraph);
    //     paras.push_back(res);
    //     head = head->right;
    // }

    // for(int i = 0; i < (int)paras.size(); i++){
    //     cout << paras[i] << endl << endl << endl;
    // }

    /*
        Part-2 : Query the LLM
    */
    int choice;
    cout<<"CHATGPT OR BARD (1 / 2) : )";
    cin>>choice;
    cout<<endl;

    string filename;
    if(choice == 1) {
        filename = "api_call.py";
    } else if(choice == 2) {
        filename = "bard_api_call.py";
    } else {
        cout<<"Wrong choice"<<endl;
        return 0;
    }

    char response = 'Y';
    while(response == 'Y' || response == 'y') {
        std::cout<<"ASK QUESTION : ";
        string q;
        getline(cin , q);
        qna_tool.query(q , filename);

        std::cout<<endl;

        std::cout<<"ANY OTHER QUESTION? (Y / N) : ";
        cin>>response;
        std::cout<<endl;
    }
    
    return 0;
}