#pragma once

#include <memory>
#include <deque>
#include <boost/filesystem.hpp>
#include <string>
#include "standard_functions.h"
#include "filestream_organizer.h"
typedef ::filestream_organizer fstream_t;
typedef std::unique_ptr<fstream_t> pfstream_t;


#define GMEXPORT extern "C" __declspec (dllexport)

std::deque<pfstream_t>& OpenFilestreams();
std::deque<int>& OpenSpots();

int InsertFileStreamToVector(pfstream_t&& file);
void DeleteFilestream(int ind);
inline bool isValidIndex(int index);


bool _setLocale(const std::string& localestr);



int _file_text_open_read(const os_string_type& filename, filestream_organizer::encoding enc = filestream_organizer::ENC_UTF8, bool forced = false);
int _file_text_open_read(const boost::filesystem::path& filename, filestream_organizer::encoding enc = filestream_organizer::ENC_UTF8, bool forced = false);
std::string _file_text_read_string(int file);
double _file_text_read_real(int file);
std::string _file_text_read_char(int file, int number);
void _file_text_unread(int file);
void _file_text_readln(int file);




int _file_text_open_append(const os_string_type& filename, filestream_organizer::encoding enc = filestream_organizer::ENC_UTF8, bool forced = false);
int _file_text_open_append(const boost::filesystem::path& filename, filestream_organizer::encoding enc = filestream_organizer::ENC_UTF8, bool forced = false);
int _file_text_open_write(const os_string_type& filename, filestream_organizer::encoding enc = filestream_organizer::ENC_UTF8, bool forced = false);
int _file_text_open_write(const boost::filesystem::path& filename, filestream_organizer::encoding enc = filestream_organizer::ENC_UTF8, bool forced = false);
void _file_text_write_string(int file, const std::string& input);
void _file_text_write_real(int file, double input);
void _file_text_writeln(int file);

bool _file_eof(int file);
bool _file_eoln(int file);
bool _file_good(int file);
bool _file_fail(int file);
bool _file_bad(int file);
void _file_clear_fail(int file);
void _file_clear_bad(int file);
void _file_write_flush(int file);
void _file_text_close(int file);

void _file_write_bom(int file);


GMEXPORT double max_open();
GMEXPORT double set_locale(const char* locale);
GMEXPORT double set_linebreak_string(const char* str);

GMEXPORT double file_text_open_read(const char* filename);
GMEXPORT double file_text_open_read_ext(const char* filename, double encoding, double forced);
GMEXPORT const char* file_text_read_string(double file);
GMEXPORT const char* file_text_read_char(double file, double num);
GMEXPORT double file_text_unread(double file);
GMEXPORT double file_text_read_real(double file);
GMEXPORT double file_text_readln(double file);


GMEXPORT double file_text_open_append(const char* filename);
GMEXPORT double file_text_open_write(const char* filename);
GMEXPORT double file_text_open_append_ext(const char* filename, double encoding, double forced);
GMEXPORT double file_text_open_write_ext(const char* filename, double encoding, double forced);
GMEXPORT double file_text_write_string(double file, const char* input);
GMEXPORT double file_text_write_real(double file, double input);
GMEXPORT double file_text_writeln(double file);


GMEXPORT double file_eof(double file);
GMEXPORT double file_eoln(double file);
GMEXPORT double file_good(double file);
GMEXPORT double file_fail(double file);
GMEXPORT double file_bad(double file);
GMEXPORT double file_clear_fail(double file);
GMEXPORT double file_clear_bad(double file);
GMEXPORT double file_write_flush(double file);
GMEXPORT double file_close(double file);
GMEXPORT double file_text_close(double file);

GMEXPORT double file_text_write_bom(double file);
GMEXPORT double file_text_set_endl(const char* endl);
GMEXPORT double file_text_set_endl_windows();
GMEXPORT double file_text_set_endl_posix();