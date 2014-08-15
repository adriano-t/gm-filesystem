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



os_path_type find_unique_fname(const os_string_type& extension) 
{
	return find_unique_fname(working_dir, extension);
}
boost::filesystem::path find_unique_fname(const os_path_type& directory, const os_string_type&  extension)
{
	os_path_type newfilename;
	os_string_type fname(_T("%%%%"));
	const os_string_type append(_T("%%%%")); 
	do {
		fname += append;
		newfilename = boost::filesystem::unique_path(directory / (fname + extension));
	} while (boost::filesystem::exists(newfilename));

	assert(boost::filesystem::exists(newfilename) == false);

	return newfilename;
}



boost::filesystem::path _copy_keep_extension(const boost::filesystem::path& filename, const os_string_type& extension) 
{
	boost::filesystem::path newfilename(find_unique_fname(extension));
	try {
		boost::filesystem::create_hard_link(filename, newfilename); //create hard link should work....
		//boost::filesystem::
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
				::MessageBox(0, string_convert<os_string_type>(tstr).c_str(), _T("copy fail"),MB_ICONERROR);
#endif
				return boost::filesystem::path();
			}
		} else {
#ifdef _DEBUG
			std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
			tstr += err.what();
			MessageBox(0, string_convert<os_string_type>(tstr).c_str(), _T("hardlink fail") ,MB_ICONERROR);
#endif
			return boost::filesystem::path();
		}
	}
	return newfilename;
}


bool export_direct(const boost::filesystem::path& temppath, const boost::filesystem::path& filename) 
{
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
	os_string_type file(MakeRichPath(string_convert<os_string_type>(filename)));
	os_string_type ext( file.substr(file.rfind('.')));
	return ReturnString(_copy_keep_extension(file, ext).string());
}
GMEXPORT const char*  find_unique_fname(const char* directory, const char* extension)
{
	auto dir(string_convert<os_string_type>(directory));
	auto ext(string_convert<os_string_type>(extension));
	os_path_type newpath;
	
	if (dir.empty()) {
		newpath = find_unique_fname(save_dir, ext);
	} else {
		newpath = find_unique_fname(os_path_type(dir), ext);
	}
	return ReturnString(string_convert<std::string>(newpath.string()));
}
GMEXPORT double set_working_directory(const char* filename) 
{
	working_dir = os_path_type(MakeRichPath(string_convert<os_string_type>(filename)));
	return 0;
}
GMEXPORT double set_gm_save_area(const char* directory)
{ //ONLY THANKS TO A MISSING FEATURE IN GM THIS HAS TO BE DONE
	save_dir = os_path_type(MakeRichPath(string_convert<os_string_type>(directory)));
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
		} catch (boost::filesystem::filesystem_error&) {
		}
		added_files.pop_back();
		++n;
	}
	return n;
}
GMEXPORT double export_sound(const char* tempfile, const char* filename)
{
	return export_direct(string_convert<os_string_type>(tempfile), MakeRichPath(string_convert<os_string_type>(filename)));
}
GMEXPORT double export_raw(const char* tempfile, const char* filename)
{
	return export_direct(string_convert<os_string_type>(tempfile), MakeRichPath(string_convert<os_string_type>(filename)));
}