#pragma once

#include <string>
#include <memory>
#include <deque>
#include <boost/property_tree/ptree.hpp>
#include "../GMFile/standard_functions.h"
#define GMEXPORT extern "C" __declspec (dllexport)

class ini_data {
public:
	typedef std::string key_type;
	typedef std::string value_type;
	typedef boost::property_tree::basic_ptree<key_type, value_type> ini_tree;
	typedef std::unique_ptr<ini_tree> upini_tree;

	ini_data(const os_string_type& fname);

	~ini_data();

	value_type read_string(const key_type& section, const key_type& key, const value_type& def) const;
	void write_string(const key_type& section, const key_type& key, const value_type& value);
	double read_real(const key_type& section, const key_type& key, double def) const;
	void write_real(const key_type& section, const key_type& key, double value);
	bool section_exists(const key_type& section) const;
	void section_delete(const key_type& section);
	bool key_exists(const key_type& section, const key_type& key) const;
	void key_delete(const key_type& section, const key_type& key);

	inline static std::deque<std::unique_ptr<ini_data> >& OpenIniFiles();
	inline static std::deque<int>& OpenSpots();
	inline static int InsertIniDataToVector(std::unique_ptr<ini_data>&& ini);
	inline static void DeleteIniData(int index);
	inline static bool IsValidIndex(int index);


private:
	ini_data(const ini_data& other);
	ini_data& operator=(const ini_data& other);

	

	static ini_tree::path_type GetPath(const key_type& section, const key_type& key);

	void load_ini();
	void save_ini();


	upini_tree data;
	os_string_type filename;
	bool constructed;
};


inline const char* _ini_read_string(const ini_data& ini,const char* section, const char* key, const char* def);
inline double _ini_read_real(const ini_data& ini, const char* section, const char* key, double def);
inline double _ini_write_string(ini_data& ini, const char* section, const char* key, const char* value);
inline double _ini_write_real(ini_data& ini, const char* section, const char* key, double value);
inline double _ini_key_exists(const ini_data& ini, const char* section, const char* key);
inline double _ini_key_delete(ini_data& ini, const char* section, const char* key);
inline double _ini_section_exists(const ini_data& ini, const char* section);
inline double _ini_section_delete(ini_data& ini, const char* section);



GMEXPORT double ini_open_ext(const char* filename);
GMEXPORT double ini_close_ext(double ini);
GMEXPORT const char* ini_read_string_ext(double ini,const char* section, const char* key, const char* def);
GMEXPORT double ini_read_real_ext(double ini, const char* section, const char* key, double def);
GMEXPORT double ini_write_string_ext(double ini, const char* section, const char* key, const char* value);
GMEXPORT double ini_write_real_ext(double ini, const char* section, const char* key, double value);
GMEXPORT double ini_key_exists_ext(double ini, const char* section, const char* key);
GMEXPORT double ini_key_delete_ext(double ini, const char* section, const char* key);
GMEXPORT double ini_section_exists_ext(double ini, const char* section);
GMEXPORT double ini_section_delete_ext(double ini, const char* section);

GMEXPORT double ini_open(const char* filename);
GMEXPORT double ini_close();
GMEXPORT const char* ini_read_string(const char* section, const char* key, const char* def);
GMEXPORT double ini_read_real(const char* section, const char* key, double def);
GMEXPORT double ini_write_string(const char* section, const char* key, const char* value);
GMEXPORT double ini_write_real(const char* section, const char* key, double value);
GMEXPORT double ini_key_exists(const char* section, const char* key);
GMEXPORT double ini_key_delete(const char* section, const char* key);
GMEXPORT double ini_section_exists(const char* section);
GMEXPORT double ini_section_delete(const char* section);