////////////////////////////////////////////////////////////////////////////////
// UtXML.h

// Include guard
#pragma once
#include <stdio.h>
#include "BtArray.h"
#include "BtTypes.h"
#include "libxml/tree.h"

class UtXML
{
public:
	~UtXML();

	xmlNode						   *Open(BtChar *name, BtChar *rootName);
	xmlNode						   *FindXMLNode(xmlNode *pNode, const BtChar *name);
    xmlNode                        *FindXMLNodeByUint(xmlNode *pNode, const BtChar *name, BtU32 uINT);
	void							PrintElementNames(xmlNode *pNode);
	BtU32							GetUintFromNode(xmlNode *pNode, const BtChar *name);
	BtChar						   *GetStringFromNode(xmlNode *pNode, const BtChar *name);
    
    BtU32                           GetUintFromChildNode(xmlNode *pNode);
    char                           *GetStringFromChildNode(xmlNode *pNode);

private:
	xmlDocPtr					    doc;
};
