#include "Node.h"

Node::Node() {
    book_code = -1;
    page = -1;
    paragraph = -1;
    sentence_no = -1;
    offset = -1;

    left = nullptr;
    right = nullptr;
}

Node::Node(int b_code, int pg, int para, int s_no, int off){
    book_code = b_code;
    page = pg;
    paragraph = para;
    sentence_no = s_no;
    offset = off;

    left = nullptr;
    right = nullptr;
}