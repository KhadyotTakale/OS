#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>
#include <algorithm>
#include <bits/stdc++.h>

using namespace std;

class TrieNode {
public:
    unordered_map<char, TrieNode*> children;
    bool isEndOfWord;

    TrieNode() : isEndOfWord(false) {}
};

class Trie {
public:
    TrieNode* root;

    Trie() {
        root = new TrieNode();
    }

    void insert(const string& word) {
        TrieNode* node = root;
        for (char c : word) {
            if (!node->children[c]) {
                node->children[c] = new TrieNode();
            }
            node = node->children[c];
        }
        node->isEndOfWord = true;
    }

    void search(TrieNode* node, const string& prefix, vector<string>& suggestions) {
        if (node->isEndOfWord) {
            suggestions.push_back(prefix);
        }
        for (auto it : node->children) {
            search(it.second, prefix + it.first, suggestions);
        }
    }

    vector<string> getSuggestions(const string& prefix) {
        TrieNode* node = root;
        vector<string> suggestions;
        for (char c : prefix) {
            if (!node->children[c]) {
                return suggestions;
            }
            node = node->children[c];
        }
        search(node, prefix, suggestions);
        return suggestions;
    }

    void getSuggestionsWithSubstring(TrieNode* node, const string& current, const string& substring, vector<string>& suggestions) {
        if (node->isEndOfWord && current.find(substring) != string::npos) {
            suggestions.push_back(current);
        }
        for (auto it : node->children) {
            getSuggestionsWithSubstring(it.second, current + it.first, substring, suggestions);
        }
    }

    vector<string> getSuggestionsWithSubstring(const string& substring) {
        vector<string> suggestions;
        getSuggestionsWithSubstring(root, "", substring, suggestions);
        return suggestions;
    }

    vector<string> getAllMatches(const string& query) {
        vector<string> suggestions = getSuggestions(query);
        vector<string> substringMatches = getSuggestionsWithSubstring(query);

        suggestions.insert(suggestions.end(), substringMatches.begin(), substringMatches.end());
        sort(suggestions.begin(), suggestions.end());
        suggestions.erase(unique(suggestions.begin(), suggestions.end()), suggestions.end());

        return suggestions;
    }
};

vector<pair<string, string>> loadCommands(const string& filename) {
    ifstream file(filename);
    vector<pair<string, string>> commands;
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string command, description;
        if (getline(iss, command, ':') && getline(iss, description)) {
            commands.emplace_back(command, description);
        }
    }
    return commands;
}

int main() {
    vector<pair<string, string>> commands = loadCommands("/Users/khadyottakale/Documents/OS/commands.txt");

    Trie trie;

    for (const auto& cmd : commands) {
        trie.insert(cmd.first);
    }

    string input;
    cout << "Enter command: ";
    cin >> input;

    vector<string> suggestions = trie.getAllMatches(input);

    if (!suggestions.empty()) {
        cout << "Did you mean:\n";
        for (const auto& suggestion : suggestions) {
            auto it = find_if(commands.begin(), commands.end(), [&suggestion](const pair<string, string>& cmd) {
                return cmd.first == suggestion;
            });
            if (it != commands.end()) {
                cout << "  - " << suggestion << " : " << it->second << '\n';
            }
        }
    } else {
        cout << "No suggestions found.\n";
    }

    return 0;
}
