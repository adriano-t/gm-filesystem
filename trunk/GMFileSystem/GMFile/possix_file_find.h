#pragma once
#include <string>
#include <boost/regex.hpp>
#include <boost/filesystem.hpp>



class possix_file_find {
public:
	explicit possix_file_find(boost::filesystem::path filemask, unsigned long attributes = 0);
	possix_file_find(std::string filemask, unsigned long attributes = 0);
	
	boost::filesystem::path getCurrent();
	void advance();
	bool isLast();

private:
	boost::filesystem::directory_iterator current;
	boost::regex mask;
	unsigned long attr;
	boost::system::error_code err_code;


	void MakeRegex(const boost::filesystem::path& filename);
	void MakeRegex(std::string filename);
	bool fits_mask();
	bool fits_attributelist();

	static boost::filesystem::directory_iterator end_itr;
	static void string_replace_all(std::string& string, const std::string& substr, const std::string& replace);
	static unsigned long get_attributes(const boost::filesystem::directory_entry& file);
};

//boost::filesystem::path possix_find_file_first;
//boost::filesystem::path possix_find_file_next;