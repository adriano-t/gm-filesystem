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

int _file_text_open_read(std::string filename);
int _file_text_open_read(boost::filesystem::path filename);
std::string _file_text_read_string(int file);
void _file_text_readln(int file);




int _file_text_open_append(std::string filename);
int _file_text_open_append(boost::filesystem::path filename);
int _file_text_open_write(std::string filename);
int _file_text_open_write(boost::filesystem::path filename);
void _file_text_write_string(int file, std::string input);
void _file_text_writeln(int file);

bool _file_eof(int file);
bool _file_eoln(int file);
bool _file_good(int file);
bool _file_fail(int file);
bool _file_bad(int file);
void _file_write_flush(int file);
void _file_close(int file);


GMEXPORT double max_open();

GMEXPORT double file_text_open_read(const char* filename);
GMEXPORT const char* file_text_read_string(double file);
GMEXPORT double file_text_readln(double file);


GMEXPORT double file_text_open_append(const char* filename);
GMEXPORT double file_text_open_write(const char* filename);
GMEXPORT double file_text_write_string(double file, const char* input);
GMEXPORT double file_text_writeln(double file);


GMEXPORT double file_eof(double file);
GMEXPORT double file_eoln(double file);
GMEXPORT double file_good(double file);
GMEXPORT double file_fail(double file);
GMEXPORT double file_bad(double file);
GMEXPORT double file_write_flush(double file);
GMEXPORT double file_close(double file);