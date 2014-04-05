#include "string_util.h"
#include <algorithm>
#include <cstring>
#include <iterator>

namespace core{
	std::vector<std::string> explode(std::string a_Input, const std::string &a_Delimiter){
		std::vector<std::string> l_Result;
		std::string::size_type l_Position = a_Input.find_first_of(a_Delimiter);

		while(l_Position != std::string::npos){
			if(l_Position > 0){
				l_Result.push_back(a_Input.substr(0, l_Position));
			}
			a_Input = a_Input.substr(l_Position + a_Delimiter.length());
			l_Position = a_Input.find_first_of(a_Delimiter);
		}
		if(a_Input.length() > 0){
			l_Result.push_back(a_Input);
		}
		return l_Result;
	}

	std::string str_replace(
		const std::string& a_Input,
		const std::string& a_Replace, 
		const std::string& a_With)
	{
		std::string l_Result(a_Input);

		if(a_Replace != a_With && !a_Replace.empty()){
			std::string::size_type l_Position = 0;
			std::string::size_type l_FindPosition = 0;

			while((l_Position = l_Result.find(a_Replace, l_FindPosition)) != std::string::npos)
			{
				l_Result.replace(l_Position, a_Replace.size(), a_With);
				l_FindPosition = l_Position + a_With.size();
			}
		}

		return l_Result;
	}

	std::string implode(const std::vector<std::string> &a_Input, const std::string &a_Delimiter){
		std::stringstream l_Stream;
		std::copy(a_Input.begin(), a_Input.end() - 1, std::ostream_iterator<std::string>(l_Stream, a_Delimiter.c_str()));

		l_Stream << a_Input.back() << std::endl;

		return l_Stream.str();
	}

	std::string ltrim(const std::string &a_Input, char l_ToTrim){
		std::string l_Output(a_Input);

		std::string::size_type l_Position = l_Output.find_first_not_of(l_ToTrim);

		if(l_Position != std::string::npos){
			l_Output.erase(0, l_Position);
		}

		return l_Output;
	}

	std::string rtrim(const std::string &a_Input, char l_ToTrim){
		std::string l_Output(a_Input);

		std::string::size_type l_Position = l_Output.find_last_not_of(l_ToTrim);

		if(l_Position != std::string::npos){
			l_Output.erase(l_Position + 1);
		}

		return l_Output;
	}

	std::string trim(const std::string& a_Input, char l_ToTrim){
		return ltrim(rtrim(a_Input, l_ToTrim), l_ToTrim);
	}

	bool starts_with(const std::string &a_String, const std::string &a_Start){
		return a_String.substr(0, a_Start.size()) == a_Start;
	}

	bool ends_with(const std::string &a_String, const std::string &a_End){
		return a_String.substr(a_String.size() - a_End.size()) == a_End;
	}

	std::string to_upper(const std::string &a_String){
		std::string l_Result(a_String);
		std::transform(l_Result.begin(), l_Result.end(), l_Result.begin(), toupper);
		return l_Result;
	}

	std::string to_lower(const std::string &a_String){
		std::string l_Result(a_String);
		std::transform(l_Result.begin(), l_Result.end(), l_Result.begin(), tolower);
		return l_Result;
	}


}
