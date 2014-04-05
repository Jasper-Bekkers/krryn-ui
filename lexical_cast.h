#pragma once
#include <limits>
#include <iomanip>
#include <sstream>
#include <string>

namespace core{
	template<typename _Tx>
	std::streamsize float_length(){
		return std::numeric_limits<_Tx>::is_signed
			+ std::numeric_limits<_Tx>::is_specialized
			+ std::numeric_limits<_Tx>::digits10 * 2;
	}

	template<typename _Tx>
	std::streamsize int_length(){
		return std::numeric_limits<_Tx>::digits
			+ std::numeric_limits<_Tx>::digits10 + 1;
	}

	// Return a dummy value when we're not dealing with numbers
	template <typename _Tx> std::streamsize type_length() { return 14; }

	// floating point data types
	template <> std::streamsize type_length<float>();
	template <> std::streamsize type_length<double>();
	template <> std::streamsize type_length<long double>();

	// integer data types
	template <> std::streamsize type_length<short>();
	template <> std::streamsize type_length<int>();
	template <> std::streamsize type_length<long>();
	template <> std::streamsize type_length<long long>();
	template <> std::streamsize type_length<char>();
	template <> std::streamsize type_length<wchar_t>();
	template <> std::streamsize type_length<signed char>();
	template <> std::streamsize type_length<bool>();

	template <> std::streamsize type_length<unsigned short>();
	template <> std::streamsize type_length<unsigned int>();
	template <> std::streamsize type_length<unsigned long>();
	template <> std::streamsize type_length<unsigned long long>();
	template <> std::streamsize type_length<unsigned char>();

	template<typename _Tx, typename _Ty>
	_Tx lexical_cast(const _Ty &a_Value){
		std::stringstream ss;
		_Tx l_Value;
		ss << std::setprecision(type_length<_Ty>()) << a_Value;
		ss >> std::setprecision(type_length<_Tx>()) >> l_Value;
		return l_Value;
	}
}
