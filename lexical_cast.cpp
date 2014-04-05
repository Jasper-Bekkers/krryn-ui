#include "lexical_cast.h"

namespace core{

// floating point data types
template <> std::streamsize type_length<float>() { return float_length<float>(); }
template <> std::streamsize type_length<double>() { return float_length<double>(); }
template <> std::streamsize type_length<long double>() { return float_length<long double>(); }

// integer data types
template <> std::streamsize type_length<short>() { return int_length<short>(); }
template <> std::streamsize type_length<int>() { return int_length<int>(); }
template <> std::streamsize type_length<long>() { return int_length<long>(); }
template <> std::streamsize type_length<long long>() { return int_length<long long>(); }
template <> std::streamsize type_length<char>() { return int_length<char>(); }
template <> std::streamsize type_length<wchar_t>() { return int_length<wchar_t>(); }
template <> std::streamsize type_length<signed char>() { return int_length<signed char>(); }
template <> std::streamsize type_length<bool>() { return int_length<bool>(); }

template <> std::streamsize type_length<unsigned short>() { return int_length<unsigned short>(); }
template <> std::streamsize type_length<unsigned int>() { return int_length<unsigned int>(); }
template <> std::streamsize type_length<unsigned long>() { return int_length<unsigned long>(); }
template <> std::streamsize type_length<unsigned long long>() { return int_length<unsigned long long>(); }
template <> std::streamsize type_length<unsigned char>() { return int_length<unsigned char>(); }

}
