#include <boost/regex.hpp>
#include <locale>
#include <codecvt>

#include <boost/filesystem.hpp>

#include "standard_functions.h"



template <>
std::wstring string_convert(const std::string& input) {	
	try {
		return string_to_wstring(input);
	} catch (std::range_error&) {
		return L"";
	}
}
template <>
std::string string_convert(const std::wstring& input) { 
	try {
		return wstring_to_string(input);
	} catch (std::range_error&) {
		return "";
	}
}

template <>
std::string string_convert(const std::string& input) { return input; }
template <>
std::wstring string_convert(const std::wstring& input) { return input; }


std::string wstring_to_string(const std::wstring& input) {
#ifdef _WIN32
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
#else
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
#endif
	return converter.to_bytes(input.c_str());
}
std::wstring string_to_wstring(const std::string& input) {
#ifdef _WIN32
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

#else
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
#endif
	return converter.from_bytes(input.c_str());
}



std::string GetReplace(std::string In)
{

	char* out(std::getenv(In.c_str()));

	if (out) {
		return out;
	}
	return "%" + In + "%";
}
std::wstring GetReplace(std::wstring In)
{

	bool success;
	std::wstring ret;
#ifdef _WIN32
	const wchar_t* out;
	out = (_wgetenv(In.c_str()));
	success = (out != NULL);
	ret = out;
#else
	const char* out;
	std::string sIn(wstring_to_string(In))
	out = std::getenv(sIn.c_str());
	success = (out != NULL);
	ret = string_to_wstring(out);
#endif

	if (success) {
		return ret;
	}
	return L"%" + In + L"%";
}

std::string MakeRichPath(std::string inputString) 
{
	const boost::regex regex("%([^%]+)%");
	boost::smatch results;
	std::string::const_iterator first(inputString.begin());
	std::string::const_iterator last(inputString.end());
	
	while (boost::regex_search(first, inputString.cend(), results, regex )) {
		std::string replaceString(GetReplace(results.str(1)));
		std::advance(first, results.position());
		inputString.replace(first, first + results.length(), replaceString);
		
		first = inputString.cbegin();
		std::advance(first, results.position() + replaceString.length());
	}
	

	return inputString;
}
std::wstring MakeRichPath(std::wstring inputString) 
{
	const boost::wregex regex(L"%([^%]+)%");
	boost::wsmatch results;
	std::wstring::const_iterator first(inputString.begin());
	std::wstring::const_iterator last(inputString.end());
	
	while (boost::regex_search(first, inputString.cend(), results, regex )) {
		std::wstring replaceString(GetReplace(results.str(1)));
		std::advance(first, results.position());
		inputString.replace(first, first + results.length(), replaceString);
		
		first = inputString.cbegin();
		std::advance(first, results.position() + replaceString.length());
	}
	

	return inputString;
}
os_string_type path_to_string(const boost::filesystem::path& p) {
	os_string_type fname_string;
#ifdef _WIN32
	fname_string = string_convert<os_string_type> (p.wstring());
#else
	fname_string = string_convert<os_string_type> (p.string());
#endif
	return fname_string;
}


