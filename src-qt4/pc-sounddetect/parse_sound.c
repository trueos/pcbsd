/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <cneirabustos@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return,carlos neira bustos 
 * ----------------------------------------------------------------------------
 */ 

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <string.h>

const char* GetKo(xmlDocPtr doc, xmlNodePtr cur, char* chip_id); 
const char*  parse_Sound_CardXML(char *docname, char* Vendor_Name, char* chip_id ) ;



/* cnb 2008-01-01
    we get the kernel module to load according to search keys(vendor,chipid)
 */

const char* GetKo(xmlDocPtr doc, xmlNodePtr cur, char* chip_id) {

	xmlChar *key;
	static char tempkey[16];
	cur = cur->xmlChildrenNode ;
	while (cur!= NULL) {

		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
		    snprintf(tempkey,sizeof tempkey ,"%s",(char*)key);

		    if(!memcmp(tempkey,chip_id,strlen(tempkey))) { 
		        xmlFree(key);
			const char *name = cur->name;
			return name;
		    }
		    cur = cur->next;
	}
	xmlFree(key);
	return "UNKNOWN";

}

/* cnb 2008-01-01
   "validate" the xml and parse the soundcards.xml , searching with keys vendor 
   and chip_id
 */
const char* parse_Sound_CardXML(char *docname, char* Vendor_Name, char* chip_id ) 
{

	xmlDocPtr doc;
	xmlNodePtr cur;
	static char kernelmodule[16];

	doc = xmlParseFile(docname);
	sprintf(kernelmodule,"%s","UNKNOWN");
	
	if (doc == NULL ) {
		fprintf(stderr,"/usr/local/share/pcbsd/pc-sounddetect/soundcards.xml not found!\n");
		return kernelmodule;
	}
	
	cur = xmlDocGetRootElement(doc);
	
	if (cur == NULL) {
		fprintf(stderr,"empty document\n");
		xmlFreeDoc(doc);
		return kernelmodule;
	}
	
	if (xmlStrcmp(cur->name, (const xmlChar *) "SoundCard_vendors")) {
		fprintf(stderr,"This xml does not have information about sound cards ");
		xmlFreeDoc(doc);
		return kernelmodule;
	}
	cur = cur->xmlChildrenNode;
	while (cur!= NULL) 
	{            
		if ((!xmlStrcmp(cur->name, (const xmlChar *)Vendor_Name))){
		    snprintf(kernelmodule,sizeof kernelmodule,"%s",GetKo(doc, cur,chip_id));
			xmlFreeDoc(doc);
			return kernelmodule;
		}
		 
		cur = cur->next;
	}
	xmlFreeDoc(doc);
	return kernelmodule;
 }
