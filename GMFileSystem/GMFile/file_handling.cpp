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
#include "filestream_organizer.h"
#define NOMINMAX
#include <Windows.h>



static std::string RetString;
#ifdef _WIN32
static std::string DefEndl("\r\n");
#else
static std::string DefEndl("\n");
#endif
std::deque<pfstream_t>& OpenFilestreams() {
	static std::deque<pfstream_t>* result = new std::deque<pfstream_t>;
	return *result;
}
std::deque<int>& OpenSpots() {
	static std::deque<int>* result = new std::deque<int>;
	return *result;
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



int InsertFileStreamToVector(pfstream_t&& file) 
{
	int ind;
	if (OpenSpots().empty()) {
			ind = OpenFilestreams().size();
			OpenFilestreams().push_back(std::forward<pfstream_t>(file));
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

int _file_text_open_read(const boost::filesystem::path& filename, filestream_organizer::encoding enc, bool forced) 
{
	os_string_type fname_string = path_to_string(filename);
	return _file_text_open_read(fname_string, enc, forced);
}
int _file_text_open_read(const os_string_type& filename, filestream_organizer::encoding enc, bool forced) 
{
	int newindex(-1);
	pfstream_t file(new fstream_t(filename, std::ios_base::in, DefEndl, enc, forced));
	if (!file->fail()) {
		newindex = InsertFileStreamToVector(std::move(file));
	}
	return newindex;
}
std::string _file_text_read_string(int file)
{
	std::string retString;
	if (isValidIndex(file) && OpenFilestreams()[file]->good()) {
		retString = OpenFilestreams()[file]->getline();
	}
	return retString;
}
std::string _file_text_read_char(int file, int number)
{
	std::string retString;
	if (isValidIndex(file) && OpenFilestreams()[file]->good()) {
		retString = OpenFilestreams()[file]->read(number);
	}
	return retString;
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
	auto& d(OpenFilestreams());
	pfstream_t& pfstr(d.at(file));
	if (isValidIndex(file) && pfstr->good()) {
		std::string tmp;
		std::string lastchr;
		bool start(false);
		auto backtrack(pfstr->tell());
		while (lastchr = pfstr->get(), lastchr.size() > 0  && lastchr != "\n" && pfstr->good() ) {
			start = start || !std::isspace(lastchr[0], std::locale());
			if (start) {
				tmp += lastchr;
				if (!(lastchr == "-" || lastchr == "+" || lastchr == "e" || lastchr == "E" || lastchr == "." || lastchr == ",")) {
					try {
						ret = boost::lexical_cast<double>(tmp);
						backtrack = pfstr->tell();
					} catch(boost::bad_lexical_cast&) {
						break;
					}
				}
			}
		}
		pfstr->seek(backtrack);
	}
	return ret;
}

void _file_text_readln(int file) 
{
	if (isValidIndex(file) && OpenFilestreams()[file]->good()) {
		OpenFilestreams()[file]->readln();
	}
}

int _file_text_open_append(const boost::filesystem::path& filename, filestream_organizer::encoding enc, bool forced) 
{
	os_string_type fname_string = path_to_string(filename);
	return _file_text_open_append(fname_string, enc, forced);
}
int _file_text_open_append(const os_string_type& filename, filestream_organizer::encoding enc, bool forced) 
{
	int newindex(-1);
	pfstream_t file(new fstream_t(filename, std::ios_base::app, DefEndl, enc, forced));
	if (!file->fail()) {
		newindex = InsertFileStreamToVector(std::move(file));
	}
	return newindex;
}
int _file_text_open_write(const boost::filesystem::path& filename, filestream_organizer::encoding enc, bool forced) 
{
	os_string_type fname_string = path_to_string(filename);
	return _file_text_open_write(fname_string, enc, forced);
}
int _file_text_open_write(const os_string_type& filename, filestream_organizer::encoding enc, bool forced) 
{
	int newindex(-1);
	pfstream_t file(new fstream_t(filename, std::ios_base::out, DefEndl, enc, forced));
	if (!file->fail()) {
		newindex = InsertFileStreamToVector(std::move(file));
	}
	return newindex;
}
void _file_text_write_string(int file, const std::string& input)
{
	if (isValidIndex(file) && OpenFilestreams()[file]->good()) {
		OpenFilestreams()[file]->write_string(input);
	}
}
void _file_text_write_real(int file, double input) 
{
	_file_text_write_string(file, boost::lexical_cast<std::string>(input));
}
void _file_text_writeln(int file) 
{
	if (isValidIndex(file) && OpenFilestreams()[file]->good()) {
		OpenFilestreams()[file]->writeln();
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
		return OpenFilestreams()[file]->eoln();
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
void _file_clear_fail(int file)
{
	if (isValidIndex(file)) {
		OpenFilestreams()[file]->clear_fail();
	}
}
void _file_clear_bad(int file)
{
	if (isValidIndex(file)) {
		OpenFilestreams()[file]->clear_bad();
	}
}
void _file_text_close(int file)
{
	DeleteFilestream(file);
}

void _file_write_bom(int file)
{
	if (isValidIndex(file)) {
		OpenFilestreams()[file]->write_BOM();
	}
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
	return _file_text_open_read(MakeRichPath(os_filename));
}
GMEXPORT double file_text_open_read_ext(const char* filename, double encoding, double forced)
{
	auto os_filename(string_convert<os_string_type>(filename));
	filestream_organizer::encoding enc(static_cast <filestream_organizer::encoding>(static_cast<int>(encoding)));
	bool f(forced != 0.0);
	return _file_text_open_read(MakeRichPath(os_filename), enc, f);
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
	RetString = _file_text_read_char(static_cast<int>(file), static_cast<int>(num));
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
GMEXPORT double file_text_open_append_ext(const char* filename, double encoding, double forced)
{
	auto os_filename(string_convert<os_string_type>(filename));
	filestream_organizer::encoding enc(static_cast <filestream_organizer::encoding>(static_cast<int>(encoding)));
	bool f(forced != 0.0);
	return _file_text_open_append(MakeRichPath(os_filename), enc, f);
}
GMEXPORT double file_text_open_write_ext(const char* filename, double encoding, double forced)
{
	auto os_filename(string_convert<os_string_type>(filename));
	filestream_organizer::encoding enc(static_cast <filestream_organizer::encoding>(static_cast<int>(encoding)));
	bool f(forced != 0.0);
	return _file_text_open_write(MakeRichPath(os_filename), enc, f);
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




GMEXPORT double file_text_eof(double file)
{
	return _file_eof(static_cast<int>(file));
}
GMEXPORT double file_text_eoln(double file)
{
	return _file_eoln(static_cast<int>(file));
}
GMEXPORT double file_text_good(double file)
{
	return _file_good(static_cast<int>(file));
}
GMEXPORT double file_text_fail(double file)
{
	return _file_fail(static_cast<int>(file));
}
GMEXPORT double file_text_bad(double file)
{
	return _file_bad(static_cast<int>(file));
}
GMEXPORT double file_text_clear_fail(double file)
{
	_file_clear_fail(static_cast<int>(file));
	return 0;
}
GMEXPORT double file_text_clear_bad(double file)
{
	_file_clear_bad(static_cast<int>(file));
	return 0;
}
GMEXPORT double file_text_write_flush(double file)
{
	_file_write_flush(static_cast<int>(file));
	return 0.0;
}

GMEXPORT double file_close(double file)
{
	return file_text_close(file);
}
GMEXPORT double file_text_close(double file)
{
	_file_text_close(static_cast<int>(file));
	return 0.0;
}

GMEXPORT double file_text_write_bom(double file)
{
	_file_write_bom(static_cast<int>(file));
	return 0.0;
}
GMEXPORT double file_text_set_endl(const char* endl)
{
	DefEndl = endl;
	return 0.0;
}
GMEXPORT double file_text_set_endl_windows()
{
	DefEndl = "\r\n";
	return 0.0;
}

GMEXPORT double file_text_set_endl_posix()
{
	DefEndl= "\n";
	return 0.0;
}

