////////////////////////////////////////////////////////////////////////////////
// UtXML.cpp

#include "BtString.h"
#include "ErrorLog.h"
#include "UtXML.h"

UtXML::~UtXML()
{
	// Free the document
	xmlFreeDoc(doc);
}

xmlNode *UtXML::Open(BtChar *name, BtChar *rootName)
{
	xmlNode *cur;

	doc = xmlParseFile(name);
	if (doc == NULL) {
		fprintf(stderr, "Document not parsed successfully. \n");
		return BtNull;
	}
	cur = xmlDocGetRootElement(doc);
	if (cur == NULL) {
		fprintf(stderr, "empty document\n");
		xmlFreeDoc(doc);
		return BtNull;
	}
	if (xmlStrcmp(cur->name, (const xmlChar *)rootName )) {
		fprintf(stderr, "document of the wrong type, root node != story");
		xmlFreeDoc(doc);
		return BtNull;
	}
	return cur;
}

xmlNode *UtXML::FindXMLNodeByUint(xmlNode *pNode, const BtChar *name, BtU32 uInt)
{
    xmlNode *cur_node = NULL;
    
    for (cur_node = pNode; cur_node; cur_node = cur_node->next)
    {
        if (cur_node->type == XML_ELEMENT_NODE)
        {
            if (strcmp((const char*)cur_node->name, (const char*)name) == 0)
            {
                if( UtXML::GetUintFromChildNode(cur_node) == uInt )
                {
                    return cur_node;
                }
            }
        }
        
        xmlNode *pFind = FindXMLNodeByUint(cur_node->children, name, uInt);
        
        if (pFind != BtNull)
        {
            return pFind;
        }
    }
    return BtNull;
}

char *UtXML::GetStringFromChildNode(xmlNode *pNode)
{
    char *pString = (char *)xmlNodeListGetString(doc, pNode->xmlChildrenNode, 1);
    return pString;
}

xmlNode *UtXML::FindXMLNode(xmlNode *pNode, const BtChar *name)
{
	xmlNode *cur_node = NULL;

	for (cur_node = pNode; cur_node; cur_node = cur_node->next)
	{
		if (cur_node->type == XML_ELEMENT_NODE)
		{
			if (strcmp((const char*)cur_node->name, (const char*)name) == 0)
			{
				return cur_node;
			}
		}

		xmlNode *pFind = FindXMLNode(cur_node->children, name);

		if (pFind != BtNull)
		{
			return pFind;
		}
	}
	return BtNull;
}

void UtXML::PrintElementNames(xmlNode * a_node)
{
	xmlNode *cur_node = NULL;

	for (cur_node = a_node; cur_node; cur_node = cur_node->next)
	{
		if (cur_node->type == XML_ELEMENT_NODE)
		{
			ErrorLog::Printf("node type: Element, name: %s\n", cur_node->name);
		}
		PrintElementNames(cur_node->children);
	}
}

BtU32 UtXML::GetUintFromChildNode(xmlNode *pNode)
{
    const char *pUInt = (const char *)xmlNodeListGetString(doc, pNode->xmlChildrenNode, 1);
    return atoi(pUInt);
}

BtU32 UtXML::GetUintFromNode(xmlNode *pNode, const BtChar *name)
{
	xmlNode *pFind = FindXMLNode( pNode, name);
	if (pFind)
	{
		const char *pUInt = (const char *)xmlNodeListGetString(doc, pFind->xmlChildrenNode, 1);
		return atoi(pUInt);
	}
	BtAssert(0);
	return 0;
}

BtChar *UtXML::GetStringFromNode(xmlNode *pNode, const BtChar *name)
{
	xmlNode *pFind = FindXMLNode(pNode, name);
	if (pFind)
	{
		return (char *)xmlNodeListGetString(doc, pFind->xmlChildrenNode, 1);
	}
	BtAssert(0);
	return 0;
}
