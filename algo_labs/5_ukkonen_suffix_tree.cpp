#include <iostream>
#include <map>
#include <vector>

int CURRENT_LEN = 0;
int ACTIVE_EDGE = 0;
int REMIND_SUFF_CNT = 0;
int INPUT_STR_SIZE = 0;

typedef struct TSNode{
    struct TSNode* suffix_link;
    std::map <char, struct TSNode*> childs_;
    int left_pos;
    int right_pos;
    TSNode(int left, int right): suffix_link(nullptr), left_pos(left), right_pos(right) {}
}TNode;

TNode* ROOT = nullptr;
TNode* LAST_NEW_NODE = nullptr;
TNode* CURRENT_NODE = ROOT;

TNode* BuildNewNode(int left_pos, int &right_pos) {
    return new TNode(left_pos, right_pos);
}

int EdgeSize(TNode* node_, int pos_) {
    return std::min(node_ -> right_pos, pos_ + 1) - node_ -> left_pos;
}

bool WalkingDown(int cur_pos, TNode* node_) {
    int edge_size = EdgeSize(node_, cur_pos);
    if (CURRENT_LEN >= edge_size) {
        ACTIVE_EDGE += edge_size;
        CURRENT_LEN -= edge_size;
        CURRENT_NODE = node_;
        return true;
    }
    return false;
}

void ProlongTree(int pos_, std::string &str_) {
    LAST_NEW_NODE = ROOT;
    ++REMIND_SUFF_CNT;
    while (REMIND_SUFF_CNT) {
        if (!CURRENT_LEN) {
            ACTIVE_EDGE = pos_;
        }
        std::map <char, TNode*>::iterator it = CURRENT_NODE -> childs_.find(str_[ACTIVE_EDGE]);
        TNode* next_node = (it == CURRENT_NODE -> childs_.end()) ? nullptr : it -> second;
        if (!next_node) {
            TNode* leaf_ = BuildNewNode(pos_, INPUT_STR_SIZE);
            CURRENT_NODE -> childs_[str_[ACTIVE_EDGE]] = leaf_;
            if (LAST_NEW_NODE != ROOT) {
                LAST_NEW_NODE -> suffix_link = CURRENT_NODE;
            }
            LAST_NEW_NODE = CURRENT_NODE;
        } else {
            if (WalkingDown(pos_, next_node)) {
                continue;
            }
            if (str_[next_node -> left_pos + CURRENT_LEN] == str_[pos_]) {
                if (LAST_NEW_NODE != ROOT) {
                    LAST_NEW_NODE -> suffix_link = CURRENT_NODE;
                }
                LAST_NEW_NODE = CURRENT_NODE;
                ++CURRENT_LEN;
                break;
            }

            TNode* split_ = new TNode(next_node -> left_pos, next_node -> left_pos + CURRENT_LEN);
            TNode* leaf_ = new TNode(pos_, INPUT_STR_SIZE);

            CURRENT_NODE -> childs_[str_[ACTIVE_EDGE]] = split_;
            split_ -> childs_[str_[pos_]] = leaf_;
            next_node -> left_pos += CURRENT_LEN;
            split_ -> childs_[str_[next_node -> left_pos]] = next_node;

            if (LAST_NEW_NODE != ROOT) {
                LAST_NEW_NODE -> suffix_link = split_;
            }
            LAST_NEW_NODE = split_;
        }
        --REMIND_SUFF_CNT;
        if (CURRENT_NODE == ROOT && CURRENT_LEN > 0) {
            --CURRENT_LEN;
            ACTIVE_EDGE = pos_ - REMIND_SUFF_CNT + 1;
        } else {
            CURRENT_NODE = (CURRENT_NODE -> suffix_link) ? CURRENT_NODE -> suffix_link : ROOT;
        }
    }
}

TNode* BuildSuffixTree(std::string& str_) {
    ROOT = BuildNewNode(0, INPUT_STR_SIZE);
    REMIND_SUFF_CNT = 0;
    ACTIVE_EDGE = 0;
    CURRENT_NODE = ROOT -> suffix_link = LAST_NEW_NODE = ROOT;
    CURRENT_LEN = 0;
    for (size_t i = 0; i < str_.size(); ++i) {
        ProlongTree(i, str_);
    }
    return ROOT;
}

std::pair <int, TNode*> GetNextLeftPos(TNode* tree_, std::string &str_) {
    std::map <char, TNode*>::iterator begin_ = tree_ -> childs_.begin();
    std::map <char, TNode*>::iterator current_min = tree_ -> childs_.begin();
    std::map <char, TNode*>::iterator end_ = tree_ -> childs_.end();
    if (begin_ == end_) {
        return std::make_pair(-1, nullptr);
    }
    for (; begin_ != end_; ++begin_) {
        if (str_[begin_ -> second -> left_pos] < str_[current_min -> second -> left_pos]) {
            current_min = begin_;
        }
    }
    return std::make_pair(current_min -> second -> left_pos, current_min -> second);
}

void ShiftDown(TNode* tree_, std::string &str_, int need_to_print, int &current_id) {
    if (need_to_print < 0) {
        return;
    }
    std::pair <int, TNode*> next_ = GetNextLeftPos(tree_, str_); // {left pos in vertex, link on next (child) node}
    if (next_.first == -1 || need_to_print < next_.first - tree_ -> left_pos) {
        for (int i = 0; i < need_to_print; ++i) {
            if (str_[current_id + i] != '~') {
                std::cout << str_[current_id + i];
            }
        }
    } else {
        for (int i = 0; i < tree_ -> right_pos - tree_ -> left_pos; ++i) {
            if (str_[current_id + i] != '~') {
                std::cout << str_[current_id + i];
            }
        }
        ShiftDown(next_.second, str_, need_to_print - (tree_ -> right_pos - tree_ -> left_pos), next_.first);
    }
}

void PrintSortedString(TNode* tree_, std::string &str_, int need_to_print) {
    std::pair <int, TNode*> next_ = GetNextLeftPos(tree_, str_);
    ShiftDown(next_.second, str_, need_to_print, next_.first);
}

int main() {
    std::ios::sync_with_stdio(false);
    std::string input_str;
    std::cin >> input_str;
    int ans_size = input_str.size();
    input_str += (input_str);
    INPUT_STR_SIZE = input_str.size();
    TNode* suffix_tree = BuildSuffixTree(input_str);
    PrintSortedString(suffix_tree, input_str, ans_size);
    std::cout << std::endl;
    return 0;
}