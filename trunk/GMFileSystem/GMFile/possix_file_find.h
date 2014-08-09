#pragma once
#include <string>
#include <regex>
#include <boost/filesystem.hpp>
#include "standard_functions.h"



class possix_file_find {
public:
	explicit possix_file_find(boost::filesystem::path filemask, unsigned long attributes = 0);
	possix_file_find(os_string_type filemask, unsigned long attributes = 0);
	
	boost::filesystem::path getCurrent();
	void advance();
	bool isLast();

private:
	boost::filesystem::directory_iterator current;
	std::basic_regex<os_char_type> mask;
	unsigned long attr;
	boost::system::error_code err_code;


	void MakeRegex(const boost::filesystem::path& filename);
	void MakeRegex(os_string_type filename);
	bool fits_mask();
	bool fits_attributelist();

	static boost::filesystem::directory_iterator end_itr;
	static void string_replace_all(os_string_type& string, const os_string_type& substr, const os_string_type& replace);
	static unsigned long get_attributes(const boost::filesystem::directory_entry& file);
};

//boost::filesystem::path possix_find_file_first;
//boost::filesystem::path possix_find_file_next;