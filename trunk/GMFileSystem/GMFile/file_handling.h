#pragma once

#include <memory>
#include <deque>
#include <boost/filesystem.hpp>
#include <string>

#define GMEXPORT extern "C" __declspec (dllexport)

std::deque<std::unique_ptr<std::fstream>>& OpenFilestreams();
std::deque<int>& OpenSpots();

int InsertFileStreamToVector(std::unique_ptr<std::fstream>&& file);
void DeleteFilestream(int ind);
inline bool isValidIndex(int index);


bool _setLocale(const std::string& localestr);



int _file_text_open_read(const std::string& filename);
int _file_text_open_read(boost::filesystem::path filename);
std::string _file_text_read_string(int file);
double _file_text_read_real(int file);
char* _file_text_read_char(int file, int number);
void _file_text_unread(int file);
void _file_text_readln(int file);




int _file_text_open_append(const std::string& filename);
int _file_text_open_append(boost::filesystem::path filename);
int _file_text_open_write(const std::string& filename);
int _file_text_open_write(boost::filesystem::path filename);
void _file_text_write_string(int file, const std::string& input);
void _file_text_write_real(int file, double input);
void _file_text_writeln(int file);

bool _file_eof(int file);
bool _file_eoln(int file);
bool _file_good(int file);
bool _file_fail(int file);
bool _file_bad(int file);
void _file_set_fail(int file, bool fail);
void _file_set_bad(int file, bool bad);
void _file_write_flush(int file);
void _file_close(int file);


GMEXPORT double max_open();
GMEXPORT double set_locale(const char* locale);

GMEXPORT double file_text_open_read(const char* filename);
GMEXPORT const char* file_text_read_string(double file);
GMEXPORT const char* file_text_read_char(double file, double num);
GMEXPORT double file_text_unread(double file);
GMEXPORT double file_text_read_real(double file);
GMEXPORT double file_text_readln(double file);


GMEXPORT double file_text_open_append(const char* filename);
GMEXPORT double file_text_open_write(const char* filename);
GMEXPORT double file_text_write_string(double file, const char* input);
GMEXPORT double file_text_write_real(double file, double input);
GMEXPORT double file_text_writeln(double file);


GMEXPORT double file_eof(double file);
GMEXPORT double file_eoln(double file);
GMEXPORT double file_good(double file);
GMEXPORT double file_fail(double file);
GMEXPORT double file_bad(double file);
GMEXPORT double file_set_fail(double file, double fail);
GMEXPORT double file_set_bad(double file, double fail);
GMEXPORT double file_write_flush(double file);
GMEXPORT double file_close(double file);