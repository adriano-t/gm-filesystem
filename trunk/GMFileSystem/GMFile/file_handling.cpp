#include <memory>
#include <string>
#include <fstream>
#include <ios>
#include <deque>
#include <locale>
#include <codecvt>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#include "file_handling.h"
#include "standard_functions.h"
#define NOMINMAX
#include <Windows.h>


static std::string RetString;
std::deque<std::unique_ptr<std::fstream>>& OpenFilestreams() {
	static std::deque<std::unique_ptr<std::fstream>> result;
	return result;
}
std::deque<int>& OpenSpots() {
	static std::deque<int> result;
	return result;
}

inline bool isValidIndex(int index) 
{
	return index >= 0 && OpenFilestreams().size() > (unsigned int)index && OpenFilestreams()[index] != nullptr;
}

bool _setLocale(const std::string& localestr)
{
	try {
		std::locale::global(std::locale(localestr));
		return true;
	} catch (std::exception&) {
		return false;
	}
}



int InsertFileStreamToVector(std::unique_ptr<std::fstream>&& file) 
{
	int ind;
	if (OpenSpots().empty()) {
			ind = OpenFilestreams().size();
			OpenFilestreams().push_back(std::forward<std::unique_ptr<std::fstream>>(file));
	} else {
			ind = OpenSpots().back();
			OpenSpots().pop_back();
			OpenFilestreams()[ind] = std::move(file);
	}
	return ind;

}

void DeleteFilestream(int ind) {
	if (isValidIndex(ind)) { //safety measure
			OpenFilestreams()[ind].reset(nullptr);
			OpenSpots().push_back(ind);
	}
}

int _file_text_open_read(boost::filesystem::path filename) 
{
	os_string_type fname_string = path_to_string(filename);
	return _file_text_open_read(fname_string);
}
int _file_text_open_read(const os_string_type& filename) 
{
	int newindex(-1);
	std::unique_ptr<std::fstream> file(new std::fstream(filename, std::ios_base::in));
	if (!file->fail()) {
		newindex = InsertFileStreamToVector(std::move(file));
	}
	return newindex;
}
std::string _file_text_read_string(int file)
{
	std::string retString;
	if (isValidIndex(file) && OpenFilestreams()[file]->good()) {
		std::getline(*OpenFilestreams()[file], retString);
		OpenFilestreams()[file]->unget();
	}
	return retString;
}
char* _file_text_read_char(int file, int number)
{
	char* buffer = new char[number + 1]();
	if (isValidIndex(file) && OpenFilestreams()[file]->good()) {
		OpenFilestreams()[file]->get(buffer, number+1);
		return buffer;
	}
	return buffer;
}
void _file_text_unread(int file)
{
	if (isValidIndex(file) && OpenFilestreams()[file]->good()) {
		OpenFilestreams()[file]->unget();
	}
}
double _file_text_read_real(int file)
{
	double ret(0);
	//boost::regex realtest("^$")
	if (isValidIndex(file) && OpenFilestreams()[file]->good()) {
		std::string tmp;
		char lastchr;
		bool start(false);
		int backtrack = 0;
		while (lastchr = OpenFilestreams()[file]->get(), lastchr != '\n' && OpenFilestreams()[file]->good() ) {
			start = start || !std::isspace(lastchr, std::locale());
			if (start) {
				tmp += lastchr;
				if (lastchr == '-' || lastchr == '+' || lastchr == 'e' || lastchr == 'E' || lastchr == '.') {
					++backtrack;
				} else {
					try {
						ret = boost::lexical_cast<double>(tmp);
						backtrack = 0;
					} catch(boost::bad_lexical_cast&) {
						break;
					}
				}
			}
		}
		for (int i(backtrack + 1); i > 0; --i) {
			OpenFilestreams()[file]->unget();
		}
	}
	return ret;
}

void _file_text_readln(int file) 
{
	if (isValidIndex(file) && OpenFilestreams()[file]->good()) {
		OpenFilestreams()[file]->ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

int _file_text_open_append(boost::filesystem::path filename) 
{
	os_string_type fname_string = path_to_string(filename);
	return _file_text_open_append(fname_string);
}
int _file_text_open_append(const os_string_type& filename) 
{
	int newindex(-1);
	std::unique_ptr<std::fstream> file(new std::fstream(filename, std::ios_base::app));
	if (!file->fail()) {
		newindex = InsertFileStreamToVector(std::move(file));
	}
	return newindex;
}
int _file_text_open_write(boost::filesystem::path filename) 
{
	os_string_type fname_string = path_to_string(filename);
	return _file_text_open_write(fname_string);
}
int _file_text_open_write(const os_string_type& filename) 
{
	int newindex(-1);
	std::unique_ptr<std::fstream> file(new std::fstream(filename, std::ios_base::out));
	if (!file->fail()) {
		newindex = InsertFileStreamToVector(std::move(file));
	}
	return newindex;
}
void _file_text_write_string(int file, const std::string& input)
{
	if (isValidIndex(file) && OpenFilestreams()[file]->good()) {
		*OpenFilestreams()[file] << input;
	}
}
void _file_text_write_real(int file, double input) 
{
	_file_text_write_string(file, boost::lexical_cast<std::string>(input));
}
void _file_text_writeln(int file) 
{
	if (isValidIndex(file) && OpenFilestreams()[file]->good()) {
		*OpenFilestreams()[file] << '\n';
	}
}
void _file_write_flush(int file) 
{
	if (isValidIndex(file) && OpenFilestreams()[file]->good()) {
		OpenFilestreams()[file]->flush();
	}
}


bool _file_eof(int file)
{
	if (isValidIndex(file)) {
		return OpenFilestreams()[file]->eof();
	}
	return false;
}
bool _file_eoln(int file)
{
	if (isValidIndex(file)) {
		return OpenFilestreams()[file]->peek() == '\n';
	}
	return false;
}
bool _file_good(int file)
{
	if (isValidIndex(file)) {
		return OpenFilestreams()[file]->good();
	}
	return false;
}
bool _file_fail(int file)
{
	if (isValidIndex(file)) {
		return OpenFilestreams()[file]->fail();
	}
	return false;
}
bool _file_bad(int file)
{
	if (isValidIndex(file)) {
		return OpenFilestreams()[file]->bad();
	}
	return false;
}
void _file_set_fail(int file)
{
	if (isValidIndex(file)) {
		auto flags(OpenFilestreams()[file]->rdstate() & ~std::ios::failbit);
		OpenFilestreams()[file]->clear(flags);
	}
}
void _file_set_bad(int file)
{
	if (isValidIndex(file)) {
		auto flags(OpenFilestreams()[file]->rdstate() & ~std::ios::badbit);
		OpenFilestreams()[file]->clear(flags);
	}
}
void _file_close(int file)
{
	DeleteFilestream(file);
}


GMEXPORT double max_open()
{
	auto s1(OpenFilestreams().max_size());
	auto s2(::_getmaxstdio());
	return std::min(int(s1),s2);
}
GMEXPORT double set_locale(const char* locale)
{
	return _setLocale(locale);
}


GMEXPORT double file_text_open_read(const char* filename)
{

	auto os_filename(string_convert<os_string_type>(filename));
	//int l = strlen(filename);
	//std::wstring display(L"");
	//for (int i(0); i < l; ++i) {
	//	unsigned char c = filename[i];
	//	// c = 0xD0;
	//	unsigned int ic = static_cast<unsigned int> (c);
	//	auto char_code = boost::lexical_cast<std::wstring>(ic);
	//	display += char_code + L"   ";
	//}
	//LPCWSTR out = (LPCWSTR)display.c_str();
	//::MessageBox(0, out, os_filename.c_str() ,MB_ICONERROR);

	//boost::filesystem::path p(os_filename);
	//auto str = p.string();
	//os_string_type os_str = string_convert<os_string_type>(str);
	//auto wstr = p.wstring();
	//os_string_type os_wstr = string_convert<os_string_type>(str);

	return _file_text_open_read(MakeRichPath(os_filename));
}
GMEXPORT const char* file_text_read_string(double file)
{
	RetString = _file_text_read_string(static_cast<int>(file));
	///TODO: make all strings UTF8

	return RetString.c_str();
}
GMEXPORT double file_text_read_real(double file) 
{
	return _file_text_read_real(static_cast<int>(file));
	
}
GMEXPORT const char* file_text_read_char(double file, double num)
{
	auto tmp(_file_text_read_char(static_cast<int>(file), static_cast<int>(num)));
	RetString = tmp;
	delete tmp;
	return RetString.c_str();
}
GMEXPORT double file_text_unread(double file)
{
	_file_text_unread(static_cast<int>(file));
	return 0;
}
GMEXPORT double file_text_readln(double file)
{
	_file_text_readln(static_cast<int>(file));
	return 0.0;
}

GMEXPORT double file_text_open_append(const char* filename)
{
	auto os_filename(string_convert<os_string_type>(filename));
	return _file_text_open_append(MakeRichPath(os_filename));
}
GMEXPORT double file_text_open_write(const char* filename)
{
	auto os_filename(string_convert<os_string_type>(filename));
	return _file_text_open_write(MakeRichPath(os_filename));
}
GMEXPORT double file_text_write_string(double file, const char* input)
{
	_file_text_write_string(static_cast<int>(file), input);
	return 0.0;
}
GMEXPORT double file_text_write_real(double file, double input) 
{
	_file_text_write_real(static_cast<int>(file), input);
	return 0;
}
GMEXPORT double file_text_writeln(double file)
{
	_file_text_writeln(static_cast<int>(file));
	return 0.0;
}




GMEXPORT double file_eof(double file)
{
	return _file_eof(static_cast<int>(file));
}
GMEXPORT double file_eoln(double file)
{
	return _file_eoln(static_cast<int>(file));
}
GMEXPORT double file_good(double file)
{
	return _file_good(static_cast<int>(file));
}
GMEXPORT double file_fail(double file)
{
	return _file_fail(static_cast<int>(file));
}
GMEXPORT double file_bad(double file)
{
	return _file_bad(static_cast<int>(file));
}
GMEXPORT double file_clear_fail(double file)
{
	_file_set_fail(static_cast<int>(file));
	return 0;
}
GMEXPORT double file_clear_bad(double file)
{
	_file_set_bad(static_cast<int>(file));
	return 0;
}
GMEXPORT double file_write_flush(double file)
{
	_file_write_flush(static_cast<int>(file));
	return 0.0;
}

GMEXPORT double file_close(double file)
{
	_file_close(static_cast<int>(file));
	return 0.0;
}

