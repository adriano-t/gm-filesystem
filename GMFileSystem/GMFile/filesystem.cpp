#include <string>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <memory>

#ifdef _WIN32
#include <Windows.h>
#endif

#include "standard_functions.h"
#include "possix_file_find.h"
#include "filesystem.h"


static std::string RETSTRING;

unsigned long _get_attributes(const boost::filesystem::path& file);
std::string _file_find_first(const std::string& directory, unsigned long attributes);
std::string _file_find_next();
void _file_find_close();


#ifdef _WIN32
::HANDLE win32_search_handle(nullptr);
unsigned long  file_find_attributes(0);
inline bool fits_attributelist(unsigned long attributes)
{
		bool subtest1 = (attributes & ::GMdirectory) != 0;
		bool test1 = ((file_find_attributes & ~GMdirectory) != 0 || subtest1);
		bool test2 = ((file_find_attributes & attributes) == 0);
		return !(test1 && test2);
}

unsigned long _get_attributes(const boost::filesystem::path& file)
{
	return GetFileAttributes(file.string().c_str());
}

std::string _file_find_first(const std::string& directory, unsigned long attributes)
{
	::WIN32_FIND_DATA outputData;
	//::HANDLE tmp_handle(::FindFirstFileEx("c:\\test.txt", ::FindExInfoBasic, &outputData, ::FindExSearchNameMatch, NULL, 0));
	::HANDLE tmp_handle(::FindFirstFile(MakeRichPath(directory).c_str(), &outputData));
	if (tmp_handle != INVALID_HANDLE_VALUE) {
		file_find_attributes = attributes;
		win32_search_handle = tmp_handle;
		BOOL finding(TRUE);
		if (!fits_attributelist(outputData.dwFileAttributes)) {
			std::string outstr(_file_find_next());
			if (outstr == "") {
				::_file_find_close();
			}
			return outstr;
		} else {
			return outputData.cFileName;
		}
	} else {
		return "";
	}
}
std::string _file_find_next()
{
	::WIN32_FIND_DATA outputData;
	BOOL finding(TRUE);
	do {
		finding = ::FindNextFile(win32_search_handle, &outputData);
	} while (finding && !fits_attributelist(outputData.dwFileAttributes) );
	if (finding) {
		return outputData.cFileName;
	} else {
		return "";
	}
}
void _file_find_close()
{
	if (::FindClose(win32_search_handle ) == 0){
		win32_search_handle = nullptr;
	}

}

#else
unsigned long _get_attributes(const boost::filesystem::path& file) 
{		
	boost::system::error_code err;
	auto status(boost::filesystem::status(file, err));
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

std::unique_ptr<possix_file_find> possix_search_handle(nullptr);

std::string _file_find_first(const std::string& directory, unsigned long attributes)
{
	possix_search_handle.reset(new possix_file_find(directory, attributes));
	return possix_search_handle->getCurrent().string();
}
std::string _file_find_next()
{
	if (possix_search_handle != nullptr) {
		possix_search_handle->advance();
		return possix_search_handle->getCurrent().string();
	} else {
		return "";
	}
}
void _file_find_close()
{
	possix_search_handle.reset(nullptr);
}

#endif



GMEXPORT double directory_exists(const char* dirname)
{
	boost::filesystem::path p(MakeRichPath(dirname));
	return boost::filesystem::exists(p);
}
GMEXPORT double directory_create(const char* dirname)
{
	boost::filesystem::path p(MakeRichPath(dirname));
	try {
		return boost::filesystem::create_directories(p);
	} catch (boost::filesystem::filesystem_error err) {
		return 0;
	}
}
GMEXPORT double directory_delete(const char* filename)
{
	boost::filesystem::path p(MakeRichPath(filename));
	boost::system::error_code err;
	boost::filesystem::remove_all(p, err);	
	return err.value();
}

GMEXPORT double file_delete(const char* filename)
{
	boost::filesystem::path p(MakeRichPath(filename));
	boost::system::error_code err;
	boost::filesystem::remove(p, err);	
	return err.value();
}
GMEXPORT double file_rename(const char* filename, const char* newname)
{
	boost::filesystem::path p(MakeRichPath(filename));
	boost::filesystem::path np(MakeRichPath(newname));
	boost::system::error_code err;
	boost::filesystem::rename(p, np, err);
	return err.value();
}
GMEXPORT double file_copy(const char* filename, const char* newname)
{
	boost::filesystem::path p(MakeRichPath(filename));
	boost::filesystem::path np(MakeRichPath(newname));
	boost::system::error_code err;
	boost::filesystem::copy(p, np, err);
	return err.value();
}
GMEXPORT const char* file_find_first(const char* mask, double attributes)
{
	RETSTRING = _file_find_first(mask, static_cast<unsigned long>(attributes));
	return ::RETSTRING.c_str();
}


GMEXPORT const char* file_find_next()
{
	RETSTRING = _file_find_next();
	return RETSTRING.c_str();
}
GMEXPORT double file_find_close() 
{
	_file_find_close();
	return 0;
}
//
GMEXPORT double file_attributes(const char* filename)
{
	boost::filesystem::path p(MakeRichPath(filename));
	return _get_attributes(p);

}

//GMEXPORT double file_exists(const char* filename);