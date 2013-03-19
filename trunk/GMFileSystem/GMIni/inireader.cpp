
#include <string>
#include <memory>

#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#ifdef _DEBUG
#include <Windows.h>
#endif

#include "inireader.h"
#include "../GMFile/standard_functions.h"

static std::string RetString;
static std::unique_ptr<ini_data> ini_file = nullptr;
std::deque<std::unique_ptr<ini_data> >& ini_data::OpenIniFiles() {
	static std::deque<std::unique_ptr<ini_data> > result;
	return result;
}
std::deque<int>& ini_data::OpenSpots() {
	static std::deque<int> result;
	return result;
}

bool ini_data::IsValidIndex(int index) 
{
	return index >= 0 && OpenIniFiles().size() > (unsigned int)index && OpenIniFiles()[index] != nullptr;
}



inline const char* ReturnString(const std::string& val)
{
	RetString = val;
	return RetString.c_str();
}
int ini_data::InsertIniDataToVector(std::unique_ptr<ini_data>&& ini)
{
	int ind;
	if (OpenSpots().empty()) {
			ind = OpenIniFiles().size();
			OpenIniFiles().push_back(std::forward< decltype(ini) >(ini));
	} else {
			ind = OpenSpots().back();
			OpenSpots().pop_back();
			OpenIniFiles()[ind] = std::move(ini);
	}
	return ind;

}
void ini_data::DeleteIniData(int ind)
{
	if (IsValidIndex(ind)) { //safety measure
		OpenIniFiles()[ind].reset(nullptr);
		OpenSpots().push_back(ind);
	}
}

ini_data::ini_data(const std::string& fname)
	: filename(fname),
	constructed(false),
	data(new ini_tree())
{
	load_ini();
}
ini_data::~ini_data()
{
	if (constructed) {
		save_ini();
	}
}


void ini_data::load_ini()
{
	try {
		boost::property_tree::read_ini(filename, *data);
		constructed = true;
	} catch (boost::property_tree::ini_parser_error& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "ini loading fail",MB_ICONERROR);
#endif
	}
	
}
void ini_data::save_ini()
{
	boost::property_tree::write_ini(this->filename, *this->data);
}

ini_data::ini_tree::path_type ini_data::GetPath(const std::string& section, const std::string& key)
{
	return ini_tree::path_type(section + "." + key);
}

std::string ini_data::read_string(const std::string& section, const std::string& key, const std::string& def) const
{
	ini_tree::path_type p(GetPath(section, key));
	return data->get(p, def);
}
double ini_data::read_real(const std::string& section, const std::string& key, double def) const 
{

	try {
		std::string d(boost::lexical_cast<std::string>(def));
		std::string str(data->get(GetPath(section,key), d));
		return boost::lexical_cast<double>(str);
	} catch (boost::bad_lexical_cast& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "data conversion fail",MB_ICONERROR);
#endif
	}
	return 0;
}
void ini_data::write_string(const std::string& section, const std::string& key, const std::string& value)
{
	try {
		data->put(GetPath(section, key), value);
	} catch (boost::property_tree::ptree_bad_data& err) {
		
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "ini string writing fail",MB_ICONERROR);
#endif
	}
}
void ini_data::write_real(const std::string& section, const std::string& key, double value)
{
	
	try {
		
		data->put(GetPath(section, key), boost::lexical_cast<std::string>(value));
	} catch (boost::property_tree::ptree_bad_data& err) {
		
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "ini real writing fail",MB_ICONERROR);
#endif
	} catch (boost::bad_lexical_cast& err) {
#ifdef _DEBUG
		std::string tstr = std::string("  file: ") + __FILE__ + "\n  line: " + boost::lexical_cast<std::string>(__LINE__) +"\n\n";
		tstr += err.what();
		::MessageBoxA(0, tstr.c_str(), "data conversion fail",MB_ICONERROR);
#endif
	}
}
bool ini_data::key_exists(const std::string& section, const std::string& key) const 
{
	auto child(data->get_child_optional(GetPath(section, key)));
	return !!child;
}
void ini_data::key_delete(const std::string& section, const std::string& key)
{
	if (key_exists(section, key)) {
		auto& s(data->get_child(section));
		s.erase(key);
	}
}
bool ini_data::section_exists(const std::string& section) const 
{
	auto child(data->get_child_optional(section));
	return !!child;
}
void ini_data::section_delete(const std::string& section)
{
	data->erase(section);
}




const char* _ini_read_string(const ini_data& ini, const char* section, const char* key, const char* def)
{
	return ReturnString(ini.read_string(section, key, def));
}
double _ini_read_real(const ini_data& ini, const char* section, const char* key, double def)
{
	return ini.read_real(section, key, def);
}
double _ini_write_string(ini_data& ini, const char* section, const char* key, const char* value)
{
	ini.write_string(section, key, value);
	return 0;
}
double _ini_write_real(ini_data& ini, const char* section, const char* key, double value)
{
	ini.write_real(section, key, value);
	return 0;
}
double _ini_key_exists(const ini_data& ini, const char* section, const char* key)
{
	return ini.key_exists(section, key);
}
double _ini_key_delete(ini_data& ini, const char* section, const char* key)
{
	ini.key_delete(section, key);
	return 0;
}
double _ini_section_exists(const ini_data& ini, const char* section)
{
	return ini.section_exists(section);
}
double _ini_section_delete(ini_data& ini, const char* section)
{
	ini.section_delete(section);
	return 0;
}



GMEXPORT double ini_open(const char* filename)
{
	std::string path(MakeRichPath(filename));
	ini_file.reset(new ini_data(path));
	return 0;
}
GMEXPORT double ini_close()
{
	ini_file.reset(nullptr);
	return 0;
}
GMEXPORT const char* ini_read_string(const char* section, const char* key, const char* def)
{
	if (ini_file)
		return _ini_read_string(*ini_file, section, key, def);
	return "";
}
GMEXPORT double ini_read_real(const char* section, const char* key, double def)
{
	if (ini_file)
		return _ini_read_real(*ini_file, section, key, def);
	return 0;
}
GMEXPORT double ini_write_string(const char* section, const char* key, const char* value)
{
	if (ini_file)
		return _ini_write_string(*ini_file, section, key, value);
	return 0;
}
GMEXPORT double ini_write_real(const char* section, const char* key, double value)
{
	if (ini_file)
		return _ini_write_real(*ini_file, section, key, value);
	return 0;
}
GMEXPORT double ini_key_exists(const char* section, const char* key)
{
	if (ini_file)
		return _ini_key_exists(*ini_file, section, key);
	return 0;
}
GMEXPORT double ini_key_delete(const char* section, const char* key)
{
	if (ini_file)
		return _ini_key_delete(*ini_file, section, key);
	return 0;
}
GMEXPORT double ini_section_exists(const char* section)
{
	if (ini_file)
		return _ini_section_exists(*ini_file, section);
	return 0;
}
GMEXPORT double ini_section_delete(const char* section)
{
	if (ini_file)
		return _ini_section_delete(*ini_file, section);
	return 0;
}



GMEXPORT double ini_open_ext(const char* filename)
{
	std::string path(MakeRichPath(filename));
	return ini_data::InsertIniDataToVector(std::unique_ptr<ini_data>(new ini_data(path)));
}

GMEXPORT double ini_close_ext(double ini)
{
	ini_data::DeleteIniData(static_cast<int>(ini));
	return 0;
}
GMEXPORT const char* ini_read_string_ext(double ini,const char* section, const char* key, const char* def)
{
	int ind = static_cast<int>(ini);
	if (ini_data::IsValidIndex(ind)) {
		return _ini_read_string(*ini_data::OpenIniFiles()[ind], section, key, def);
	}
	return ReturnString(def);
}
GMEXPORT double ini_read_real_ext(double ini, const char* section, const char* key, double def)
{
	int ind = static_cast<int>(ini);
	if (ini_data::IsValidIndex(ind)) {
		return _ini_read_real(*ini_data::OpenIniFiles()[ind], section, key, def);
	}
	return def;
}
GMEXPORT double ini_write_string_ext(double ini, const char* section, const char* key, const char* value)
{
	int ind = static_cast<int>(ini);
	if (ini_data::IsValidIndex(ind)) {
		_ini_write_string(*ini_data::OpenIniFiles()[ind], section, key, value);
	}
	return 0;
}
GMEXPORT double ini_write_real_ext(double ini, const char* section, const char* key, double value)
{
	int ind = static_cast<int>(ini);
	if (ini_data::IsValidIndex(ind)) {
		_ini_write_real(*ini_data::OpenIniFiles()[ind], section, key, value);
	}
	return 0;
}
GMEXPORT double ini_key_exists_ext(double ini, const char* section, const char* key)
{
	int ind = static_cast<int>(ini);
	if (ini_data::IsValidIndex(ind)) {
		return _ini_key_exists(*ini_data::OpenIniFiles()[ind], section, key);
	}
	return 0;
}
GMEXPORT double ini_key_delete_ext(double ini, const char* section, const char* key)
{
	int ind = static_cast<int>(ini);
	if (ini_data::IsValidIndex(ind)) {
		_ini_key_delete(*ini_data::OpenIniFiles()[ind], section, key);
	}
	return 0;
}
GMEXPORT double ini_section_exists_ext(double ini, const char* section)
{
	int ind = static_cast<int>(ini);
	if (ini_data::IsValidIndex(ind)) {
		return _ini_section_exists(*ini_data::OpenIniFiles()[ind], section);
	}
	return 0;
}
GMEXPORT double ini_section_delete_ext(double ini, const char* section)
{
	int ind = static_cast<int>(ini);
	if (ini_data::IsValidIndex(ind)) {
		_ini_section_delete(*ini_data::OpenIniFiles()[ind], section);
	}
	return 0;
}