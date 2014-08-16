#include <string>
#include <regex>
#include <boost/filesystem.hpp>

#include "possix_file_find.h"
#include "standard_functions.h"

boost::filesystem::directory_iterator possix_file_find::end_itr = boost::filesystem::directory_iterator();

possix_file_find::possix_file_find(boost::filesystem::path filemask, unsigned long attributes)
	: attr(attributes),
	err_code()
{
	current = boost::filesystem::directory_iterator(filemask.parent_path(), err_code);
	MakeRegex(filemask.filename());
	if (current != end_itr && (!fits_mask() || !fits_attributelist())) {
		advance();
	}
}
possix_file_find::possix_file_find(os_string_type filemask, unsigned long attributes)
	: attr(attributes),
	err_code()
	
{
	current = boost::filesystem::directory_iterator(boost::filesystem::path( MakeRichPath(filemask)).parent_path(), err_code);
	auto p(filemask.rfind('\\'));
	if (p != filemask.npos) {
		filemask.erase(0, p+1);
	}
	MakeRegex(filemask);
	if (current != end_itr && (!fits_mask() || !fits_attributelist())) {
		advance();
	}
}

bool possix_file_find::fits_mask()
{
	if (current == end_itr) {
		return false;
	}
	os_string_type fname(path_to_string(current->path().filename()));
	return std::regex_match(fname, mask);
}
bool possix_file_find::fits_attributelist()
{
	if (current == end_itr) {
		return false;
	}
	os_string_type testname(path_to_string(current->path()));
	unsigned long attributes(get_attributes(*current));
	bool subtest1 = (attributes & ::GMdirectory) != 0;
	bool test1 = ((attr & ~GMdirectory) != 0 || subtest1);
	bool test2 = ((attr & attributes) == 0);
	return !(test1 && test2);
}


boost::filesystem::path possix_file_find::getCurrent()
{
	if (current != end_itr) {
		return current->path();
	} else {
		return os_string_type();
	}
}
void possix_file_find::advance()
{
	if (current != end_itr) {
		while (current.increment(err_code) != end_itr &&
			(!fits_mask() || !fits_attributelist())) {
		}
	}
}
bool possix_file_find::isLast()
{
	return current == end_itr;
}


void possix_file_find::string_replace_all(os_string_type& string, const os_string_type& substr, const os_string_type& replace)
{
	std::string::size_type pos(0);
	auto count(substr.length());
	while ((pos = string.find(substr, pos)) != std::string::npos) {
		string.replace(pos, count, replace);
		pos+=replace.length();
	}
}

void possix_file_find::MakeRegex(const boost::filesystem::path& filename)
{
	MakeRegex(path_to_string(filename));
}
void possix_file_find::MakeRegex(os_string_type filename)
{
	string_replace_all(filename, _T("."), _T("[.]"));
	string_replace_all(filename, _T("*"), _T(".*"));
	string_replace_all(filename, _T("?"), _T("."));
	mask = std::basic_regex<os_char_type>(filename);
}
unsigned long possix_file_find::get_attributes(const boost::filesystem::directory_entry& file)
{		
	boost::system::error_code err;
	auto status(file.status());
	if (boost::system::error_condition::unspecified_bool_type(err)) return -1;

	unsigned long ret(::GMreadonly);
	boost::filesystem::perms permissions(status.permissions());
	if ((permissions & (boost::filesystem::owner_write | boost::filesystem::group_write | boost::filesystem::others_write )) == (boost::filesystem::owner_write | boost::filesystem::group_write | boost::filesystem::others_write) ) {
		ret &= ~GMreadonly;
	}
	
	switch (status.type()){
	case boost::filesystem::directory_file:
		ret |= GMdirectory;
		break;
	}
	return ret;
}