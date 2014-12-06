/*
 * Copyright (C) 2013-2014 Carlos Neira Bustos cneirabustos@gmail.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* cnb 2008-01-01
    we get the kernel module to load according to search keys(vendor,chipid)
 */

#include "xmlparse.h"

const char* GetKo(xmlDocPtr doc, xmlNodePtr cur, char* chip_id) {

  xmlChar *key;
  static char tempkey[16];
  cur = cur->xmlChildrenNode ;
  char* tsz;
  while (cur!= NULL) {

    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
    snprintf(tempkey,sizeof tempkey ,"%s",(char*)key);


    if(!memcmp(tempkey,chip_id,strlen(tempkey))) { 
      xmlFree(key);
      char *name = (char*)cur->name;
      
      /* run shell script found after - in the name*/
      if((tsz=strtok(name,"-"))!=NULL)
	{
	  if( (tsz = strtok(NULL,"-"))!=NULL)
	    run_task(tsz,name);
	}


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
  cnb 2014-03-31
  now we have one xml file for device class
  
*/
const char* parse_XML(char *docname, char* Vendor_Name, char* chip_id) 
{

  xmlDocPtr doc;
  xmlNodePtr cur;
  static char kernelmodule[16];



  doc = xmlParseFile(docname);
  sprintf(kernelmodule,"%s","UNKNOWN");
	
  if (doc == NULL ) {
    fprintf(stderr,"/usr/local/share/pcbsd/pc-hwprobe/%s.xml not found!\n"
	    ,docname);
    return kernelmodule;
  }
	
  cur = xmlDocGetRootElement(doc);
	
  if (cur == NULL) {
    fprintf(stderr,"empty document\n");
    xmlFreeDoc(doc);
    return kernelmodule;
  }
  cur = cur->xmlChildrenNode;
  while (cur!= NULL) 
    {            
      if ((!xmlStrcmp(cur->name, (const xmlChar *)Vendor_Name))){
#ifdef _DEBUG
	printf("GetKo looking for chip_id <%s>for vendor<%s>\n",chip_id,Vendor_Name);
#endif
	snprintf(kernelmodule,sizeof kernelmodule,"%s",GetKo(doc, cur,chip_id));
#ifdef _DEBUG
	printf("GetKo returned %s\n",kernelmodule);
#endif
	xmlFreeDoc(doc);
	return kernelmodule;
      }
		 
      cur = cur->next;
    }
  xmlFreeDoc(doc);
  return kernelmodule;
}

int run_task(char* szscript, char* ko)
{
  const char* sc = "./scripts/";
  char fullpath[32];

  printf("Driver %s needs additional tasks to perform\n",ko);
#ifdef _DEBUG
  printf(" running script %s%s\n",sc,szscript);
#endif
  sprintf(fullpath,"%s%s",sc,szscript);
  int rc =  system(fullpath);

  if(rc != 127) printf("Additional tasks for %s completed\n",ko);
  else printf("Additional tasks for %s failed\n",ko);

  return rc ;

}
