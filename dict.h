// Do NOT add any other includes
#include <string> 
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

class hashmap {
private:
    struct ListNode {
        string key;
        long long val;
        ListNode(string k, long long v) : key(k), val(v) {}
    };

    int size;
    int capacity;
    vector<vector<ListNode>> map;

    int hash(string key) {
        unsigned int hash = 1033;
        for (char ch : key) {
            hash = (hash * 31 + ch) % capacity; // Fix the hash function
        }
        return hash;
    }

public:
    hashmap(int capacity = 400000) {
        this->capacity = capacity;
        map = vector<vector<ListNode>>(capacity);
        size = 0;
    }

    void insert(string key, long long val) {
        int index = hash(key);
        bool updated = false;
        for (ListNode& node : map[index]) {
            if (node.key == key) {
                node.val += val;
                updated = true;
                break; // Once the key is found and updated, no need to continue the loop
            }
        }
        if (!updated) {
            map[index].push_back(ListNode(key, val));
            size++;
        }
    }

    long long get_val(string key) {
        int index = hash(key);
        for (const ListNode& node : map[index]) {
            if (node.key == key) {
                return node.val;
            }
        }
        return 0;
    }

    vector<string> get_keys() {
        vector<string> ans;
        for(auto bucket : map) {
            for(auto& node : bucket) {
                ans.push_back(node.key);
            }
        }

        return ans;
    }
};

class Dict {
private:
    // You can add attributes/helper functions here
    hashmap* dict;
    int cap;

public: 
    /* Please do not touch the attributes and 
    functions within the guard lines placed below  */
    /* ------------------------------------------- */
    Dict(int cap);

    ~Dict();

    void insert_sentence(int book_code, int page, int paragraph, int sentence_no, string sentence);

    int get_word_count(string word);

    void dump_dictionary(string filename);

    /* -----------------------------------------*/
};