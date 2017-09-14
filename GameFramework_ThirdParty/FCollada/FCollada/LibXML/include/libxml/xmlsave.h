/*
 * Summary: the XML document serializer
 * Description: API to save document or subtree of document
 *
 * Copy: See Copyright for the status of this software.
 *
 * Author: Daniel Veillard
 */

#ifndef __XML_XMLSAVE_H__
#define __XML_XMLSAVE_H__

#include <libxml/xmlversion.h>
#include <libxml/tree.h>
#include <libxml/encoding.h>
#include <libxml/xmlIO.h>

#ifdef LIBXML_OUTPUT_ENABLED
#ifdef __cplusplus
extern "C" {
#endif

/**
 * xmlSaveOption:
 *
 * This is the set of XML save options that can be passed down
 * to the xmlSaveToFd() and similar calls.
 */
typedef enum {
    XML_SAVE_FORMAT     = 1<<0	/* format save output */
} xmlSaveOption;


typedef struct _xmlSaveCtxt xmlSaveCtxt;
typedef xmlSaveCtxt *xmlSaveCtxtPtr;

XMLPUBFUN xmlSaveCtxtPtr XMLCALL
		xmlSaveToFd		(int fd,
					 const char *encoding,
					 int options);
XMLPUBFUN xmlSaveCtxtPtr XMLCALL
		xmlSaveToFilename	(const char *filename,
					 const char *encoding,
					 int options);
/******
  Not yet implemented.

XMLPUBFUN xmlSaveCtxtPtr XMLCALL
		xmlSaveToBuffer		(xmlBufferPtr buffer,
					 const char *encoding,
					 int options);
 ******/
XMLPUBFUN xmlSaveCtxtPtr XMLCALL
		xmlSaveToIO		(xmlOutputWriteCallback iowrite,
					 xmlOutputCloseCallback ioclose,
					 void *ioctx,
					 const char *encoding,
					 int options);

XMLPUBFUN long XMLCALL
		xmlSaveDoc		(xmlSaveCtxtPtr ctxt,
					 xmlDocPtr doc);
XMLPUBFUN long XMLCALL
		xmlSaveTree		(xmlSaveCtxtPtr ctxt,
					 xmlNodePtr node);

XMLPUBFUN intptr_t XMLCALL
		xmlSaveFlush		(xmlSaveCtxtPtr ctxt);
XMLPUBFUN intptr_t XMLCALL
		xmlSaveClose		(xmlSaveCtxtPtr ctxt);
XMLPUBFUN int XMLCALL
		xmlSaveSetEscape	(xmlSaveCtxtPtr ctxt,
					 xmlCharEncodingOutputFunc escape);
XMLPUBFUN int XMLCALL
		xmlSaveSetAttrEscape	(xmlSaveCtxtPtr ctxt,
					 xmlCharEncodingOutputFunc escape);
#ifdef __cplusplus
}
#endif
#endif /* LIBXML_OUTPUT_ENABLED */
#endif /* __XML_XMLSAVE_H__ */


