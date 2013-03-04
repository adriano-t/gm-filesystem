#pragma once
#include <deque>
#include <memory>
#include <string>
#include <stdexcept>

#include <tinyxml2.h>
#define GMEXPORT extern "C" __declspec (dllexport)


class xml_error : public std::runtime_error
{
    public:
        /// Instantiate a ptree_error instance with the given message.
        /// @param what The message to associate with this error.
        xml_error(const std::string &what)
			: std::runtime_error(what)
		{
		}
};


class xml_data {
public:
	enum ChildType {
		CT_UNKNOWN = 0,
		CT_TEXT,
		CT_ELEMENT,
		CT_DOCUMENT,
		CT_DECLARATION,
		CT_COMMENT
	};
	typedef tinyxml2::XMLDocument XMLDocument;
	typedef tinyxml2::XMLNode Node;
	typedef Node* pNode;
	typedef const Node* cpNode;
	typedef tinyxml2::XMLElement Elem;
	typedef Elem* pElem;
	typedef const Elem* cpElem;
	typedef tinyxml2::XMLAttribute Attr;
	typedef Attr* pAttr;
	typedef const Attr* cpAttr;

	xml_data(const char* fname, tinyxml2::Whitespace white = tinyxml2::PRESERVE_WHITESPACE);
	xml_data(const std::string& fname, tinyxml2::Whitespace white = tinyxml2::PRESERVE_WHITESPACE);

	~xml_data();


	inline bool NodeIsType(cpNode node, ChildType type) const;
	ChildType getNodeType(cpNode node) const;
	inline cpElem ConvertToElement(cpNode node) const;
	inline pElem ConvertToElement(pNode node);
	int NumChildElems(cpNode parent_node) const;
	int NumChildNodes(cpNode parent_node) const;
	//raw node access
	cpNode firstChild(cpNode parent_node) const;
	cpNode lastChild(cpNode parent_node) const;
	cpNode nextChild(cpNode node) const;
	cpNode prevChild(cpNode node) const;

	XMLDocument* getDoc() 
	{
		return &doc;
	}


	//element access
	cpElem topNode() const;
	cpElem firstChildElem(cpNode parent_node) const;
	cpElem lastChildElem(cpNode parent_node) const;
	cpElem nextChildElem(cpElem node) const;
	cpElem prevChildElem(cpElem node) const;
	cpElem firstNamedChildElem(cpNode parent_node, const std::string& name) const;
	cpElem lastNamedChildElem(cpNode parent_node, const std::string& name) const;
	cpElem nextNamedChildElem(cpElem node) const;
	cpElem prevNamedChildElem(cpElem node) const;
	
	cpElem getParentElem(cpNode node) const;

	//raw node access
	pNode firstChild(cpNode parent_node);
	pNode lastChild(cpNode parent_node);
	pNode nextChild(cpNode node);
	pNode prevChild(cpNode node);

	//element access
	pElem topNode();
	pElem firstChildElem(cpNode parent_node);
	pElem lastChildElem(cpNode parent_node);
	pElem nextChildElem(cpElem node);
	pElem prevChildElem(cpElem node);
	pElem firstNamedChildElem(cpNode parent_node, const std::string& name);
	pElem lastNamedChildElem(cpNode parent_node, const std::string& name);
	pElem nextNamedChildElem(cpElem node);
	pElem prevNamedChildElem(cpElem node);

	pElem getParentElem(cpNode node);

	//cpElem
	std::string NodeName(cpElem node) const;
	std::string NodeData(cpElem node) const;
	std::string NodeRawValue(cpNode node) const;
	void setName(pElem node, const std::string& name);
	void setValue(pElem node, const std::string& value);
	void setRawValue(pNode node, const std::string& value);
	pElem pushbackChildElem(pElem node, const std::string& name, const std::string& value);
	pElem pushfrontChildElem(pElem node, const std::string& name, const std::string& value);
	pElem pushChildElem(pElem node, pNode after, const std::string& name, const std::string& value);
	void deleteNode(pNode parent_node, pNode node);
	void deleteNodes(pNode parent_node);
	void setAttribute(pElem node, const std::string& name, const std::string& value);
	void deleteAttribute(pElem node, const std::string& name);
	//rawnode
	pNode pushbackChild(pNode node, ChildType type, const std::string& name);
	pNode pushfrontChild(pNode node, ChildType type, const std::string& name);
	pNode pushChild(pNode node, pNode after, ChildType type, const std::string& name);

	//attribute access
	int NumAttributes(cpElem node) const;
	std::string GetAttribute(cpElem node, const std::string& name) const;
	cpAttr firstAttribute(cpElem parent_node) const;
	cpAttr lastAttribute(cpElem parent_node) const;
	cpAttr nextAttribute(cpAttr attribute) const;
	pAttr firstAttribute(cpElem parent_node);
	pAttr lastAttribute(cpElem parent_node);
	pAttr nextAttribute(cpAttr attribute);

	//attribute
	std::string AttributeName(cpAttr attribute) const;
	std::string AttributeValue(cpAttr attribute) const;


	inline static std::deque<std::unique_ptr<xml_data> >& OpenXMLFiles();
	inline static std::deque<int>& OpenSpots();
	static int InsertXMLDataToVector(std::unique_ptr<xml_data>&& xml);
	static void DeleteXMLData(int index);
	inline static bool isValidXMLObject(int pos);
private:

	pNode createNewNode(ChildType type, const std::string& name);
	pElem CreateNewElem(const std::string& name, const std::string& value);


	std::string filename;
	XMLDocument doc;

	int AddNode(pNode node);
	int RemNode(pNode node);
};