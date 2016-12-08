#pragma once
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <vector>
#include <sstream>

// trim from start
static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
		std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}

//
static inline std::string getFirstWord(std::string& s) {
	size_t endPos=s.find_first_of(' ');
	std::string word = s.substr(0, endPos);
	return word;
}

// split string
std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> strings;
	std::istringstream f(s);
	std::string a;
	while (std::getline(f, a , delim)){
		strings.push_back(a);
	}
	return strings;
}

//contains
bool contains(const std::string& subject, std::string substring) {
	return (subject.find(substring) != std::string::npos);
}
std::string replaceString(std::string subject, const std::string& search,
	const std::string& replace) {
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}

std::vector<std::vector<std::string>> extractParameters(std::string& line){
	std::string command = getFirstWord(line);
	std::string param = line.substr(command.length() + 1, line.length() - (command.length() + 1));
	std::vector<std::vector<std::string>> result;
	result.push_back({ command });
	param.erase(remove_if(param.begin(), param.end(), 
		[](char x) {
			return std::isspace(x) || x == '(';
		}), param.end());
	std::vector<std::string> paramGroups = split(param, ')');
	for (auto group : paramGroups)
	{
		result.push_back(split(group, ','));
	}
	return result;
}

std::vector<std::string> parseSimpLine(std::string& line) {
	std::vector<std::string> params = {};
	stringstream ss(line);
	string tmp;
	while (ss>>tmp) {
		params.push_back(tmp);
	}
	return params;
}

std::vector<std::vector<std::string>> extractLine_Surface(std::string& line) {
	std::string command = getFirstWord(line);
	std::string param = line.substr(command.length() + 1, line.length() - (command.length() + 1));
	std::vector<std::vector<std::string>> result;
	result.push_back({ command });
	param.erase(remove_if(param.begin(), param.end(),
		[](char x) {
		return x == '(';
	}), param.end());
	std::vector<std::string> paramGroups = split(param, ')');
	
	result.push_back(split(paramGroups[0], ','));
	result.push_back(split(paramGroups[1], ' '));
	
	return result;
}

std::vector<std::vector<double>> extractNormals_Polygon(std::string& line) {
	vector<vector<double>> normals;
	for (int i = 0; i < 3;i++){
		size_t pos_n = line.find('n',7);
		size_t pos_l = line.find('[', pos_n);
		size_t pos_r = line.find(']', pos_l);
		string k = line.substr(pos_l+1, pos_r-pos_l);
		vector<string> comps = split(k,',');
		vector<double> normal = { stod(comps[0]),stod(comps[1]),stod(comps[1]) };
		normals.push_back(normal);
		line.erase(pos_n, k.size()+3);
	}
	return normals;
}