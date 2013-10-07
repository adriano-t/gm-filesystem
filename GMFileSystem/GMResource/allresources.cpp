#include <boost/filesystem.hpp>
#ifdef _DEBUG
#include <Windows.h>
#include <boost\lexical_cast.hpp>
#endif


#include "../GMFile/standard_functions.h"
#include "allresources.h"


static std::string RetString;
std::deque<boost::filesystem::path> added_files =std::deque<boost::filesystem::path>();
boost::filesystem::path working_dir="";
boost::filesystem::path save_dir="";

inline const char* ReturnString(const std::string& val)
{
	RetString = val;
	return RetString.c_str();
}



boost::filesystem::path find_unique_fname(const std::string& extension) 
{
	return find_unique_fname(working_dir, extension);
}
boost::filesystem::path find_unique_fname(const boost::filesystem::path directory, const std::string&  extension)
{
	boost::filesystem::path newfilename;
	std::string fname("%%%%");
	do {
		fname += "-%%%%";
		newfilename = boost::filesystem::unique_path(directory / (fname + extension));
	} while (boost::filesystem::exists(newfilename));

	assert(boost::filesystem::exists(newfilename) == false);

	return newfilename;
}



std::string _copy_keep_extension(const std::string& filename, const std::string& extension) 
{
	boost::filesystem::path newfilename(find_unique_fname(extension));
	try {
		boost::filesystem::create_hard_link(filename, newfilename); //create hard link should work....
		added_files.push_back(newfilename);
	} catch (boost::filesystem::filesystem_error& err) {
		auto c(err.code().value());
		if (c == boost::system::errc::too_many_links || c == 1142) {
			try {
				boost::filesystem::copy(filename, newfilename); //fallback option to copy
				added_files.push_back(newfilename);
			} catch (boost::filesystem::filesystem_error& err2) {
#ifdef _DEBUG
				std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
				tstr += err2.what();
				::MessageBoxA(0, tstr.c_str(), "copy fail",MB_ICONERROR);
#endif
				return "";
			}
		} else {
#ifdef _DEBUG
			std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
			tstr += err.what();
			MessageBoxA(0, tstr.c_str(), "hardlink fail" ,MB_ICONERROR);
#endif
			return "";
		}
	}
	return newfilename.string();
}


bool export_direct(const std::string& tempfile, const std::string& filename) 
{
	boost::filesystem::path temppath = tempfile;
	added_files.push_back(temppath);
	try {
		boost::filesystem::create_hard_link(temppath, filename); //create hard link should work....
	} catch (boost::filesystem::filesystem_error& err) {
		auto c(err.code().value());
		if (c == boost::system::errc::too_many_links || c == 1142) {
			try {
				boost::filesystem::copy(temppath, filename); //fallback option to copy
			} catch (boost::filesystem::filesystem_error& err2) {
#ifdef _DEBUG
				std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
				tstr += err2.what();
				::MessageBoxA(0, tstr.c_str(), "copy fail",MB_ICONERROR);
#endif
				return false;
			}
		} else {
#ifdef _DEBUG
			std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
			tstr += err.what();
			MessageBoxA(0, tstr.c_str(), "hardlink fail" ,MB_ICONERROR);
#endif
			return false;
		}
	}
	return true;
}

GMEXPORT const char* copy_fast(const char* filename) 
{
	std::string file(MakeRichPath(filename));
	std::string ext( file.substr(file.rfind('.')));
	return ReturnString(_copy_keep_extension(file, ext));
}
GMEXPORT const char*  find_unique_fname(const char* directory, const char* extension)
{
	std::string d(directory);
	if (d == "") {
		return ReturnString(find_unique_fname(save_dir, std::string(extension)).string());
	} else {
		return ReturnString(find_unique_fname(boost::filesystem::path(directory), std::string(extension)).string());
	}
}
GMEXPORT double set_working_directory(const char* filename) 
{
	working_dir = MakeRichPath(filename);
	return 0;
}
GMEXPORT double set_gm_save_area(const char* directory)
{ //ONLY THANKS TO A MISSING FEATURE IN GM THIS HAS TO BE DONE
	save_dir = MakeRichPath(directory);
	return 0;
}
GMEXPORT double clean_temporary()
{
	int n(0);
	boost::system::error_code ec;
	while (!added_files.empty()) {
		auto t(added_files.back().string());
		bool b(boost::filesystem::exists(t));
		try {
			boost::filesystem::remove(t);
		} catch (boost::filesystem::filesystem_error err) {
			auto w(err.what());
			auto c(err.code());
			auto m(c.message());
			auto v(c.value());
		}
		added_files.pop_back();
		++n;
	}
	return n;
}
GMEXPORT double export_sound(const char* tempfile, const char* filename)
{
	return export_direct(tempfile, filename);
}
GMEXPORT double export_raw(const char* tempfile, const char* filename)
{
	return export_direct(tempfile, filename);
}