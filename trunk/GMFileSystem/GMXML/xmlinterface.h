#pragma once
#include <string>
#include <memory>

#include <boost/tokenizer.hpp>
#include<boost/token_iterator.hpp>

#include "xmldata.h"



#define GMEXPORT extern "C" __declspec (dllexport)

double pointer_to_double(void* p);
double node_to_double(xml_data::pNode p);
double elem_to_double(xml_data::pElem p);
double attr_to_double(xml_data::pAttr p);
void* double_to_pointer(double v);
xml_data* double_to_xml(double xml);
xml_data::pNode double_to_node(double v);
xml_data::pElem double_to_elem(double v);
xml_data::pAttr double_to_attr(double v);

typedef boost::tokenizer<boost::escaped_list_separator<char>, std::string::const_iterator, std::string> tokenizer;
typedef boost::token_iterator<boost::escaped_list_separator<char>, std::string::const_iterator, std::string> token_iterator;

xml_data::pNode _xml_find_elem_under(xml_data* xml, xml_data::pNode node, 
											token_iterator begin, 
											const token_iterator& end);



GMEXPORT double xml_same_node(double node_left, double node_right);

GMEXPORT double xml_open(const char* filename);
GMEXPORT double xml_open_ext(const char* filename, double whitespace);
GMEXPORT double xml_close(double xml);
GMEXPORT double xml_get_node_type(double xml, double node);
GMEXPORT double xml_node_make_element(double xml, double node);
GMEXPORT double xml_num_elem(double xml, double parent_node);
GMEXPORT double xml_num_node(double xml, double parent_node);
	//raw node access
GMEXPORT double xml_node_first(double xml, double parent_node);
GMEXPORT double xml_node_last(double xml, double parent_node);
GMEXPORT double xml_node_next(double xml, double node);
GMEXPORT double xml_node_previous(double xml, double node);
GMEXPORT double xml_root_element(double xml );
GMEXPORT double xml_elem_first(double xml, double parent_node);
GMEXPORT double xml_elem_last(double xml, double parent_node);
GMEXPORT double xml_elem_next(double xml, double elem);
GMEXPORT double xml_elem_previous(double xml, double elem);
GMEXPORT double xml_named_elem_first(double xml, double parent_node, const char* name);
GMEXPORT double xml_named_elem_last(double xml, double parent_node, const char* name);
GMEXPORT double xml_named_elem_next(double xml, double elem);
GMEXPORT double xml_named_elem_previous(double xml, double elem);

GMEXPORT double xml_find_elem(double xml, const char* path);
GMEXPORT double xml_find_elem_under(double xml, double elem, const char* path);

GMEXPORT double xml_parent_elem(double xml, double node);


//cpElem
GMEXPORT const char* xml_get_elem_name(double xml, double elem);
GMEXPORT const char* xml_get_elem_data(double xml, double elem);
GMEXPORT const char* xml_get_node_raw_data(double xml, double node);

GMEXPORT double xml_set_elem_name(double xml, double elem, const char* name);
GMEXPORT double xml_set_elem_value(double xml, double elem, const char* value);
GMEXPORT double xml_set_node_raw_data(double xml, double elem, const char* data);

GMEXPORT double xml_insert_end_elem(double xml, double parent_elem, const char* name, const char* value);
GMEXPORT double xml_insert_begin_elem(double xml, double parent_elem, const char* name, const char* value);
GMEXPORT double xml_insert_elem(double xml, double parent_elem, double after_node, const char* name, const char* value);
GMEXPORT double xml_delete_node(double xml, double parent_node, double node);
GMEXPORT double xml_clear(double xml, double parent_node);
GMEXPORT double xml_set_attribute(double xml, double elem, const char* name, const char* value);
GMEXPORT double xml_delete_attribute(double xml, double elem, const char* name);
//rawnode
GMEXPORT double xml_insert_end_node(double xml, double node, double type, const char* name);
GMEXPORT double xml_insert_begin_node(double xml, double node, double type, const char* name);
GMEXPORT double xml_insert_node(double xml, double node, double after_node, double type, const char* name);


	//attribute access
GMEXPORT double xml_num_attributes(double xml, double elem);
GMEXPORT const char* xml_get_attribute(double xml, double elem, const char* name);
GMEXPORT double xml_attribute_first(double xml, double elem);
GMEXPORT double xml_attribute_last(double xml, double elem);
GMEXPORT double xml_attribute_next(double xml, double attribute);

	//attribute
GMEXPORT const char* xml_attribute_get_name(double xml, double attribute);
GMEXPORT const char* xml_attribute_get_value(double xml, double attribute);