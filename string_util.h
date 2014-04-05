#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

namespace core{
	std::vector<std::string> explode(
		std::string a_Input, const std::string &a_Delimiter
	);

	std::string implode(
		const std::vector<std::string> &a_Input, 
		const std::string &a_Delimiter
	);

	std::string str_replace(
		const std::string& a_Input, 
		const std::string& a_Replace, 
		const std::string& a_With
	);

	std::string ltrim(const std::string &a_Input, char l_ToTrim = ' ');
	std::string rtrim(const std::string &a_Input, char l_ToTrim = ' ');
	std::string trim(const std::string& a_Input, char l_ToTrim = ' ');

	bool starts_with(const std::string &a_String, const std::string &a_Start);
	bool ends_with(const std::string &a_String, const std::string &a_End);
	std::string to_upper(const std::string &a_String);
	std::string to_lower(const std::string &a_String);
}