#include "include/search.h"

#include <algorithm>
#include <fstream>
#include <iterator>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

string cleanToken(const string& token) {
  // TODO student
  string result = "";
  int start = 0;
  int end = token.length() - 1;

  bool hasAplha = false;

  if (token.empty()) {
    return "";
  }
  // Gets rid of punct in begining
  while ((start < token.length()) && (ispunct(token[start]))) {
    start++;
  }
  // Gets rid of punct in end
  while ((end >= start) && (ispunct(token[end]))) {
    end--;
  }

  for (int i = start; i <= end; i++) {
    if (isalpha(token[i])) {
      hasAplha = true;
    }
    result += tolower(token[i]);
  }

  if (!hasAplha) {
    return "";
  }

  return result;
}

set<string> gatherTokens(const string& text) {
  // TODO student
  set<string> result;
  stringstream ss(text);
  string word;

  while (ss >> word) {
    string cleanedWord = cleanToken(word);
    if (!cleanedWord.empty()) {
      result.insert(cleanedWord);
    }
  }

  return result;
}

int buildIndex(const string& filename, map<string, set<string>>& index) {
  // TODO student
  ifstream inFS;

  inFS.open(filename);
  string keys;
  string values;
  int count = 0;
  if (!inFS.is_open()) {
    return 0;
  }
  while (getline(inFS, keys)) {
    count++;
    getline(inFS, values);
    stringstream ss(values);
    string word;
    while (ss >> word) {
      word = cleanToken(word);
      if (!word.empty()) {
        index[word].insert(keys);
      }
    }
  }

  inFS.close();

  return count;
}

set<string> findQueryMatches(const map<string, set<string>>& index,
                             const string& sentence) {
  // TODO student
  set<string> result;
  stringstream ss(sentence);
  string word;
  map<string, set<string>> temp = index;

  ss >> word;
  result = temp[cleanToken(word)];
  while (ss >> word) {
    if (word[0] == '+') {
      word = cleanToken(word);
      set<string> tempResult = result;
      result.clear();
      set_intersection(tempResult.begin(), tempResult.end(),
          temp[word].begin(), temp[word].end(),
          inserter(result, result.begin()));
    }
    else if (word[0] == '-') {
      word = cleanToken(word);
      set<string> tempResult = result;
      result.clear();
      set_difference(tempResult.begin(), tempResult.end(),
          temp[word].begin(), temp[word].end(),
          inserter(result, result.begin()));
    }
    else {
      word = cleanToken(word);
      set<string> tempResult = result;
      result.clear();
      set_union(tempResult.begin(), tempResult.end(),
          temp[word].begin(), temp[word].end(),
          inserter(result, result.begin()));
    }
  }
  return result;
}

void searchEngine(const string& filename) {
  // TODO student
  map <string, set<string>> index;
  
  int pgesIndexed = buildIndex(filename, index);

  if (pgesIndexed == 0) {
    cout << "Invalid filename." << endl;
  }

  cout << "Stand by while building index..." << endl;
  int uniqueTerms = index.size();

  cout << "Indexed " << pgesIndexed << " pages containing " << uniqueTerms << " unique terms" << endl;

  string query;
  bool check = true;
  
  while (check) {
    cout << endl;
    cout << "Enter query sentence (press enter to quit): ";
    getline(cin , query);

    if (query.empty()) {
      break;
    }

    set <string> matchedPgs = findQueryMatches(index, query);


    if (matchedPgs.empty()) {
      cout << "Found 0 matching pages" << endl;
    }
    else {
      cout << "Found " << matchedPgs.size() << " matching pages" << endl;
      for (const string &url : matchedPgs) {
        cout << url << endl;
      }
    }
  }
  cout << "Thank you for searching!" << endl;
}

