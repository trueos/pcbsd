/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <cneirabustos@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return, carlos neira bustos 
 * ----------------------------------------------------------------------------
 */

#include <sys/types.h>
#include <sys/fcntl.h>
#include <err.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <libutil.h>
#include <sys/pciio.h>
#include <sys/queue.h>
#include <sys/param.h>
#include <sys/linker.h>
#include </usr/src/sys/dev/pci/pcireg.h>
#include <curl/curl.h>

#define	_PATH_DEVPCI	"/dev/pci"
#define SOUNDCARD_XML  "/usr/local/share/pcbsd/pc-sounddetect/soundcards.xml"
#define ALREADY_CALLED 2 
#define FIRST_CALL  1

/* Prototipes */
extern const char* parse_Sound_CardXML(char *docname, char* Vendor_Name, char* chip_id ) ;
static void look_for_soundcard(short called);
static int update_soundcards_xml(void);
/* end proto*/

static void look_for_soundcard(short called)
{
	int fd;
	struct pci_conf_io pc;
	struct pci_conf conf[255], *p;
	int none_count = 0;
	char chip_id[12];
	char def_chip_id[12];
	char vendor[12];
	char kmodule[16];
	char prevkmodule[16];
	short kldload_status ;

	memset(chip_id,0,sizeof(chip_id));
	memset(def_chip_id,0,sizeof(def_chip_id));
	memset(kmodule,0,sizeof(kmodule));
	memset(prevkmodule,0,sizeof(prevkmodule));
	memset(vendor,0,sizeof(vendor));
	sprintf(prevkmodule,"UNSET");

	fd = open(_PATH_DEVPCI, O_RDONLY, 0);
	if (fd < 0)
		err(1, "%s", _PATH_DEVPCI);

	bzero(&pc, sizeof(struct pci_conf_io));
	pc.match_buf_len = sizeof(conf);
	pc.matches = conf;

	do {
		if (ioctl(fd, PCIOCGETCONF, &pc) == -1)
			err(1, "ioctl(PCIOCGETCONF)");

		if (pc.status == PCI_GETCONF_LIST_CHANGED) {
			warnx("PCI device list changed, please try again");
			close(fd);
			return;
		} else if (pc.status ==  PCI_GETCONF_ERROR) {
			warnx("error returned from PCIOCGETCONF ioctl");
			close(fd);
			return;
		}
		for (p = conf; p < &conf[pc.num_matches]; p++) {

				/* taken from pcbsd sound detect : 0x040100,  0x040300  
					multimedia class for pci.
				 */
		if(
			(( (p->pc_class << 16) | (p->pc_subclass << 8) | p->pc_progif) == 0x040100 ) 
			||
			(( (p->pc_class << 16) | (p->pc_subclass << 8) | p->pc_progif) == 0x040300 ) 
			  )
			{

				sprintf(chip_id,"0x%08x",((p->pc_device << 16) | p->pc_vendor)) ;
				sprintf(def_chip_id,"0xffff%x",p->pc_vendor);
				sprintf(vendor,"x%x",p->pc_vendor );
 			        sprintf(kmodule,"%s",parse_Sound_CardXML(SOUNDCARD_XML,vendor, chip_id));
				// Try find default_chip_id 
				if(!memcmp(kmodule,"UNKNOWN",7))
				{
					sprintf(kmodule,"%s",parse_Sound_CardXML(SOUNDCARD_XML,vendor, def_chip_id));
				}
      				// Check if we have a unknown card
				if(!memcmp(kmodule,"UNKNOWN",7))
 				{
					if (called != ALREADY_CALLED)
					{
					 update_soundcards_xml();	
					 exit(0);
					}

					printf("\nUnidentified sound card!");
					printf("\nPlease report the make & model to support@lists.pcbsd.org\n");
					sleep(3);
					exit(1);
				}

			        if(!memcmp(prevkmodule, kmodule, 16))
				    continue;

				if ( kld_isloaded(kmodule) != 0 )
				    continue;

				if(kld_load(kmodule) <0)
				{
				    warn("\nError loading module...%s",kmodule);
			            exit(1);
				}else{ 
			   	    printf("Loaded sound module: %s.\n",kmodule);
				    strncpy(prevkmodule, kmodule,16);
				}

			}
		}
	} while (pc.status == PCI_GETCONF_MORE_DEVS);

	close(fd);
}

static int copy_file(char* src , char* dest)
{

  int inF, ouF;
  char line[512];
  int bytes;

  if((inF = open((const char*) src, O_RDONLY)) == -1) {
    perror("open");
    return -1;
  }

  if((ouF = open((const char*) dest, O_WRONLY | O_CREAT)) == -1) {
    close(inF);
    perror("open");
    return -1;
  }

  while((bytes = read(inF, line, sizeof(line))) > 0)
    write(ouF, line, bytes);

  close(inF);
  close(ouF);
  return 0 ;
} 


static int update_soundcards_xml(void)
{
   
  CURL *curl_handle;
  static  char *new_soundcards_xml="/tmp/temp.soundcards.xml";
  static char* snd_detect_failed="/tmp/snd_detect_failed";
  FILE* bodyfile ;


    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();
  
    curl_easy_setopt(curl_handle, CURLOPT_URL, "updates.pcbsd.org/soundcards.xml");
  
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1);

    bodyfile = fopen(new_soundcards_xml,"w");

    if (bodyfile == NULL) {
      curl_easy_cleanup(curl_handle);
      return -1;
    }
  
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, bodyfile); 
  
  
    if(curl_easy_perform(curl_handle) == 0 )
{
    fclose(bodyfile);
    curl_easy_cleanup(curl_handle);

    if (copy_file(new_soundcards_xml,SOUNDCARD_XML) == 0)
	look_for_soundcard(ALREADY_CALLED);
	return 0 ;
}  
    fclose(bodyfile);
    curl_easy_cleanup(curl_handle);
    bodyfile = fopen(snd_detect_failed,"w");
    fprintf(bodyfile,"Reason : Could not connect to update server");
    fclose(bodyfile);

return -1 ;
}

int main()
{
	look_for_soundcard(FIRST_CALL);
	return 0;
}
