#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <utility>
#include <tuple>
#include <algorithm>


class TrieNode {
public:
    char letter;
    std::map<char, TrieNode*> child;
    bool leaf;
    int value;
    TrieNode(char c) {
        letter = c;
        leaf = false;
        value = 0;
    }

    char getMaximumChild(char* ignores) {
        char max = '\0';
        int value = -1;
        for (auto const& ch : child) {
            if (child[ch.first]->value > value && ch.first != ignores[0] && ch.first != ignores[1]) {
                max = ch.first;
                value = child[ch.first]->value;
            }
        }
        return max;
    }

};

class Trie {
public:
    TrieNode* root;
private:
    
    std::string _file;
    std::string _common_file;
public:
    int size;
    Trie(std::string file, std::string comm) {
        _file = std::move(file);
        _common_file = std::move(comm);
        size = 0;
        root = new TrieNode('\0');
    }

    void insert(std::string s, int value) {
        size++;
        TrieNode* current_node = root;
        for (char c : s) {
            if (current_node != root)
                current_node->value += value;
            if (current_node->child.count(c) == 0) {
                current_node->child[c] = new TrieNode(c);
            }
            current_node = current_node->child[c];
        }
        current_node->leaf = true;

    };



    bool check(std::string s) {
        TrieNode* current_node = root;
        for (char c : s) {
            if (current_node->child.count(c) == 0) {
                return false;
            }
            current_node = current_node->child[c];
        }
        return true;
    }

    void displayWords() {
        displayWordsAux(root, "");
    }

    void displayWordsAux(TrieNode* root, std::string words) {
        if (root->child.empty()) {
            return;
        }
        if (root->leaf) {
            std::cout << words << " ";
        }
        for (int i = 0; i < 26; i++) {
            char ch = 'a' + i;
            if (root->child.count(ch) == 0) {
                displayWordsAux(root->child[i], words + ch);
            }
        }
    }
    void  loadFromFileWords() {
        std::ifstream f(_file);
        std::string word;
        while (getline(f, word)) {
            this->insert(word, 1);
        }
        f.close();
    }

    void loadFromFileCommonWords() {
            std::ifstream f(_common_file);
            std::string word;
            while (getline(f, word)) {
                if (!this->check(word)) {
                    this->insert(word, 20);
                }
            }
            f.close();
    }

    std::vector<std::string> suggestCompletions(std::string sample){
        std::vector<std::string> completions;
        char ignores[] = { '\0','\0' };
        std::pair<std::string, char> pair = suggestCompletion(sample, ignores);
        ignores[0] = pair.second;
        completions.push_back(pair.first);
        pair = suggestCompletion(sample, ignores);
        ignores[1] = pair.second;
        completions.push_back(pair.first);
        pair = suggestCompletion(sample, ignores);
        completions.push_back(pair.first);
        return completions;
    }

    std::pair<std::string,char> suggestCompletion(std::string sample,char* ignores) {
        std::string completion = "" + sample;
        TrieNode* current_node = root;
        for (char c : sample) {
            if (current_node->child.count(c)) {
                current_node = current_node->child[c];
            }
            else {
                return std::make_pair("",'\0');
            }
        }

        if (current_node == nullptr) {
            return std::make_pair("", '\0');
        }
        char maximalPath;
        char max = current_node->getMaximumChild(ignores);

        if (current_node->leaf) {
            return std::make_pair(completion, max);
        }

        while (!current_node->leaf) {
            maximalPath = current_node->getMaximumChild(ignores);
            if (maximalPath == '\0') {
                return std::make_pair("", '\0');
            }
            completion.push_back(maximalPath);
            current_node = current_node->child[maximalPath];
            if (current_node == nullptr) {
                return std::make_pair("", '\0');
            }

        }

        return std::make_pair(completion,max);
    }

};
