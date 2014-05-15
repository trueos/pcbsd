/*-
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

/*Really lame acpi "probe",I'll use the proper interfaces in the future*/
#include "acpi_probe.h"
#include "hwdetect.h"

int acpi_probe(char** ids,char* ko)
{
  FILE *fp;
  char path[1280];
  path[1279]='\0';
  char**t = ids;
#ifdef _DEBUG
  printf("Dumping acpidump\n");
#endif
  fp = popen("/usr/sbin/acpidump -td  ", "r");

#ifdef _DEBUG
  printf("popen fp %p\n",fp);
#endif
  if (fp == NULL) {
    printf("Failed to run command\n" );
    return -1;
  }
#ifdef _DEBUG
  printf("Dumping acpidump\n");
#endif

  while (fgets(path, sizeof(path)-2, fp) != NULL) {
    if(*t==NULL) {
      t=ids;

    }
    while(*t!=NULL)
      {
#ifdef _DEBUG
	printf(" t %s\n",*t);
#endif

	if(strstr(path,*t) != NULL) 
	  {
#ifdef _DEBUG
	    printf("found %s\n",*t);
#endif
	    pclose(fp);
	    load_acpi(ko);
	    return 0;
	  }
	*t++;
      }
 
  }
  pclose(fp);
  return 1;

}

int look_for_acpi()
{
  static   char* asus_id[]={"ATK0100", "ASUS010",NULL};

  static   char* asus_wmi[]={"97845ED0-4E6D-11DE-8A39-0800200C9A66",
			     "0B3CBB35-E3C2-45ED-91C2-4C5A6D195D1C",
			     "ABBC0F72-8EA1-11D1-00A0-C90629100000",NULL};

  static char *fujitsu_ids[] = { "FUJ02B1", NULL };
  static char* hp_ids[]={"95F24279-4D7B-4334-9387-ACCDC67EF61C",
			 "5FB7F034-2C63-45E9-BE91-3D44E2C707E4",
			 "2D114B49-2DFB-4130-B8FE-4A3C09E75133",NULL};
  static char    *ibm_ids[] = {"IBM0068", "LEN0068", NULL};
  static char *mat_ids[] = { "MAT0019", NULL };
  static char    *rapidstart_ids[] = {"INT3392", NULL};
  static char    *sny_id[] = {"SNY5001", NULL};
  static char *tosh_ids[] = { "TOS6200", "TOS6207", "TOS6208", NULL };
#ifdef _DEBUG
  static char *dummy[] = { "VBOX",NULL};
#endif

  printf("Searching for compatible acpi modules\n");

  if(acpi_probe(asus_id,"acpi_asus") ==0 ) return 0; 
  if(acpi_probe(asus_wmi,"acpi_wmi") ==0 ) return 0; 
  if(acpi_probe(fujitsu_ids,"acpi_fujitsu") ==0 ) return 0; 
  if(acpi_probe(hp_ids,"acpi_hp") ==0 ) return 0; 
  if(acpi_probe(ibm_ids,"acpi_ibm") ==0 ) return 0; 
  if(acpi_probe(mat_ids,"acpi_panasonic") ==0 ) return 0; 
  if(acpi_probe(rapidstart_ids,"acpi_rapidstart") ==0 ) return 0; 
  if(acpi_probe(sny_id,"acpi_sony") ==0 ) return 0; 
  if(acpi_probe(tosh_ids,"acpi_toshiba") ==0 ) return 0; 
#ifdef _DEBUG
  if(acpi_probe(dummy,"nullko") ==0 ) return 0; 
#endif
  return 1;
}

int load_acpi(char* ko)
{
  if(kld_load(ko) <0){
    warn("\nError loading module...%s\n",ko);
    return -1;
  } else { 
    printf("Kernel Module:%s Loaded.\n",ko);
    return 0;
  }
#ifdef _DEBUG
  printf("Acpi module not found\n");
#endif
  return -2;
}
