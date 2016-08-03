#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;

vector<string> formatKeys(vector<string> keys) {

  for(int i = 0; i < keys.size(); ++i) {
    string formated = "\"";

    for(int j = 0; j < keys[i].length(); ++j) {
      if(keys[i][j] == ' ') {
        formated += '-';
      } else {
        formated += keys[i][j];
      }
    }
    formated += "\"";
    keys[i] = formated;
  }

  return keys;
}

string formatJsonObject(vector<string> keys, vector<string> values) {

  string object = "\t{\n";

  for(int i = 0; i < keys.size(); ++i) {

    string value = "\"";
    if(values.size() > i) {
      value += values[i];
    }
    value += "\"";

    object += "\t\t";
    object += (keys[i] + ": ");
    object += value;

    if(i == keys.size()-1) {
      object += "\n";
    } else {
      object += ",\n";
    }
  }

  object += "\t},\n";

  return object;
}

vector<string> splitLine(string line) {
  int i = 0;
  string element = "";
  vector<string> splited;
  
  while(i < line.length()) {
    if(line[i] == ',') {
      splited.push_back(element);
      element = "";
    } else {
      element += line[i];
    }
    i++;
  }
  splited.push_back(element);

  return splited;
}

static void show_usage(char* name){
  cerr << "Usage: " << name << " -s SOURCE_PATH -d DESTINATION_PATH"<<endl;
}

bool initFiles(int argc, char* argv[]) {
  string sourcePath = "";
  string destinationPath = "";

  if(argc < 3) {
    return false;
  }

  for(int i = 0; i < argc; ++i) {
    string arg = argv[i];

    if(arg == "-s") {
      if(i + 1 >= argc) {
        return false;
      }
      sourcePath = argv[++i];
    }
    else if(arg == "-d") {
      if(i + 1 >= argc) {
        return false;
      }
      destinationPath = argv[++i];
    }
  }

  if(sourcePath == "" || destinationPath == "") {
    return false;
  }

  freopen(sourcePath.c_str(), "r+", stdin);
  freopen(destinationPath.c_str(), "w+", stdout);

  return true;
}

int main(int argc, char* argv[]) {

  if(!initFiles(argc, argv)) {
    show_usage(argv[0]);
    return 1;
  }
 
  int i = 0;
  string key = "";
  string line = "";
  string json = "";
  vector<string> keys;
  vector<string> values;

  // Read headers
  getline(cin, line);

  keys = splitLine(line);
  keys = formatKeys(keys);

  json = "[\n";

  // Read line per line until EOF
  while(getline(cin, line)) {
    // Avoid empty lines
    if(line != "") {
      values = splitLine(line);
      json += formatJsonObject(keys, values);
    }
  }

  // Remove last comma
  json[json.size()-2] = ' ';

  json += "]";

  cout<<json;

  return 0;
}
