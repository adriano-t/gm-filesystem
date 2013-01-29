#include <memory>
#include <string>
#include <fstream>
#include <ios>
#include <deque>

#include <boost/filesystem.hpp>
#include "file_handling.h"
#include "standard_functions.h"

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
	return _file_text_open_read(filename.string());
}
int _file_text_open_read(std::string filename) 
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
	std::string retString("");
	if (isValidIndex(file) && OpenFilestreams()[file]->good()) {
		std::getline(*OpenFilestreams()[file], retString);
		OpenFilestreams()[file]->unget();
	}
	return retString;
}
void _file_text_readln(int file) 
{
	if (isValidIndex(file) && OpenFilestreams()[file]->good()) {
		OpenFilestreams()[file]->ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
}

int _file_text_open_append(boost::filesystem::path filename) 
{
	return _file_text_open_append(filename.string());
}
int _file_text_open_append(std::string filename) 
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
	return _file_text_open_write(filename.string());
}
int _file_text_open_write(std::string filename) 
{
	int newindex(-1);
	std::unique_ptr<std::fstream> file(new std::fstream(filename, std::ios_base::out));
	if (!file->fail()) {
		newindex = InsertFileStreamToVector(std::move(file));
	}
	return newindex;
}
void _file_text_write_string(int file, std::string input)
{
	std::string retString("");
	if (isValidIndex(file) && OpenFilestreams()[file]->good()) {
		*OpenFilestreams()[file] << input;
	}
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
void _file_set_fail(int file, bool fail)
{
	if (isValidIndex(file)) {
		auto flags(OpenFilestreams()[file]->rdstate() & ~std::ios::failbit);
		OpenFilestreams()[file]->clear(flags);
	}
}
void _file_set_bad(int file, bool bad)
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

GMEXPORT double file_text_open_read(const char* filename)
{
	return _file_text_open_read(MakeRichPath(filename));
}
GMEXPORT const char* file_text_read_string(double file)
{
	RetString = _file_text_read_string(static_cast<int>(file));
	return RetString.c_str();
}
GMEXPORT double file_text_readln(double file)
{
	_file_text_readln(static_cast<int>(file));
	return 0.0;
}

GMEXPORT double file_text_open_append(const char* filename)
{
	return _file_text_open_append(MakeRichPath(filename));
}
GMEXPORT double file_text_open_write(const char* filename)
{
	return _file_text_open_write(MakeRichPath(filename));
}
GMEXPORT double file_text_write_string(double file, const char* input)
{
	_file_text_write_string(static_cast<int>(file), input);
	return 0.0;
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
GMEXPORT double file_set_fail(double file, double fail)
{
	_file_set_fail(static_cast<int>(file), static_cast<bool>(fail));
	return 0;
}
GMEXPORT double file_set_bad(double file, double bad)
{
	_file_set_bad(static_cast<int>(file), static_cast<bool>(bad));
	return 0;
}
GMEXPORT double file_set_bad(double file, double fail);
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

