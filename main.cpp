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

string formatJsonObject(vector<int> indexes, vector<string> values) {

  if(values.size() <= indexes[0] || values.size() <= indexes[1]) {
    return "";
  }

  string object = "\t\"";
  object += values[indexes[0]];
  object += "\": \"";
  object += values[indexes[1]];
  object += "\",\n";

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
  cerr << "Usage: " << name << " -s SOURCE_PATH -d DESTINATION_PATH --keyName 'COLUMN NAME' --valueName 'COLUMN NAME'"<<endl;
}

bool validateParameters(int argc, char* argv[], vector<string>& names) {
  string sourcePath = "";
  string destinationPath = "";
  string keyName = "";
  string valueName = "";

  if(argc < 9) {
    return false;
  }

  for(int i = 1; i < argc - 1; ++i) {
    string arg = argv[i];

    if(arg == "-s") {
      sourcePath = argv[++i];
    }
    if(arg == "-d") {
      destinationPath = argv[++i];
    }
    if(arg == "--keyName") {
      keyName = argv[++i];
    }
    if(arg == "--valueName") {
      valueName = argv[++i];
    }
  }

  if(sourcePath == "" || destinationPath == "" || keyName == "" || valueName == "") {
    return false;
  }

  freopen(sourcePath.c_str(), "r+", stdin);
  freopen(destinationPath.c_str(), "w+", stdout);
  names.push_back(keyName);
  names.push_back(valueName);

  return true;
}

vector<int> findIndexes(vector<string> values, vector<string> names) {
  vector<int> result (2, -1);

  for(int i = 0; i < values.size(); ++i) {
    if(values[i] == names[0]) {
      result[0] = i;
    }
    else if(values[i] == names[1]) {
      result[1] = i;
    }
  }

  if(result[0] == -1 || result[1] == -1) {
    vector<int> error (0);
    return error;
  }

  return result;
}

int main(int argc, char* argv[]) {

  int i = 0;
  string key = "";
  string line = "";
  string json = "";
  vector<string> values;
  vector<string> names;
  vector<int> indexes;

  if(!validateParameters(argc, argv, names)) {
    show_usage(argv[0]);
    return 1;
  }

  // Read headers
  getline(cin, line);
  values = splitLine(line);
  indexes = findIndexes(values, names);

  // If the keyName of valueName wasn't found, display the usage message
  if(indexes.size() != 2) {
    show_usage(argv[0]);
    return 1;
  }

  json = "{\n";

  // Read line per line until EOF
  while(getline(cin, line)) {
    // Avoid empty lines
    if(line != "") {
      values = splitLine(line);
      json += formatJsonObject(indexes, values);
    }
  }

  // Remove last comma
  if(json[json.size()-2] == ',') {
    json[json.size()-2] = ' ';
  }

  json += "}";

  cout<<json;

  return 0;
}
