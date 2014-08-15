#pragma once

#include <string>
#include <boost/filesystem.hpp>


#ifdef _WIN32
typedef std::wstring os_string_type;
typedef wchar_t os_char_type;
typedef boost::filesystem::wpath os_path_type;
#else
typedef std::string os_string_type;
typedef char os_char_type;
typedef boost::filesystem::wpath os_path_type;
#endif


#if defined(UNICODE)
#define _T(x) L ##x
#else
#define _T(x) x
#endif

typedef char* char_array;
typedef const char* const_char_array;
typedef wchar_t* wchar_array;
typedef const wchar_t* const_wchar_array;
typedef os_char_type* os_char_array;
typedef const os_char_type* const_os_char_array;


std::string MakeRichPath(std::string inputString);
std::wstring MakeRichPath(std::wstring inputString);

std::string wstring_to_string(const std::wstring& input);
std::wstring string_to_wstring(const std::string& input);
template <typename cvt_Ty>
std::wstring string_to_wstring(const std::string& input, cvt_Ty* pcvt) {
	std::wstring_convert<cvt_Ty> converter(pcvt);
	return converter.from_bytes(input);
}
template <typename cvt_Ty>
std::string wstring_to_string(const std::string& input, cvt_Ty* pcvt) {
	std::wstring_convert<cvt_Ty> converter(pcvt);
	return converter.to_bytes(input);
}

os_string_type path_to_string(const boost::filesystem::path& p);


template <typename OSS_Ty>
OSS_Ty string_convert(const std::string& input);
template <typename OSS_Ty>
OSS_Ty string_convert(const std::wstring& input);


template <>
std::wstring string_convert(const std::string& input);
template <>
std::string string_convert(const std::wstring& input);

template <>
std::string string_convert(const std::string& input);
template <>
std::wstring string_convert(const std::wstring& input);

enum GMAttributes
{
	GMreadonly = 0x1,
	GMhidden = 0x2,
	GMsystem = 0x4,
	GMvolumeid = 0x8,
	GMdirectory = 0x10,
	GMarchiv = 0x20,

	GMall = 0x3F
};

template <typename OSS_Ty>
OSS_Ty string_convert(const std::wstring& input) {
	return OSS_Ty(input);
}
template <typename OSS_Ty>
OSS_Ty string_convert(const std::string& input) {
	return OSS_Ty(input);
}

