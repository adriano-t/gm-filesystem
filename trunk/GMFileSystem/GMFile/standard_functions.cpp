#include <boost/regex.hpp>

#include "standard_functions.h"





std::string GetReplace(std::string In)
{

	char* out(std::getenv(In.c_str()));

	if (out) {
		return out;
	}
	return "%" + In + "%";
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