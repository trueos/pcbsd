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
 *
 * This code was shamelessly taken from pciconf
 */

#include "hwdetect.h"
#include <sys/types.h>
#include <sys/fcntl.h>
#include <ctype.h>
#include <err.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/pciio.h>
#include <sys/queue.h>

#include <dev/pci/pcireg.h>

#include "pathnames.h"
#include "pciconf.h"
#include "hwdetect.h"
#include "xmlparse.h"

struct pci_device_info
{
    TAILQ_ENTRY(pci_device_info)	link;
    int					id;
    char				*desc;
};

struct pci_vendor_info
{
    TAILQ_ENTRY(pci_vendor_info)	link;
    TAILQ_HEAD(,pci_device_info)	devs;
    int					id;
    char				*desc;
};

TAILQ_HEAD(,pci_vendor_info)	pci_vendors;

 void list_bars(int fd, struct pci_conf *p);
 void list_devs(int verbose, int bars, int caps, int errors);
 void list_verbose(struct pci_conf *p);
 const char *guess_class(struct pci_conf *p);
 const char *guess_subclass(struct pci_conf *p);
 int load_vendors(void);

static int exitstatus = 0;
 
void list_devs_plain()
{
	int fd;
	struct pci_conf_io pc;
	struct pci_conf conf[255], *p;
/* search by vendor and chip in xml files */
	char v[32],d[32];
	char kmodule[32];
	char prevkmodule[32];
	char xml[32];
	const char* dp;

	load_vendors();

	fd = open(_PATH_DEVPCI, O_RDONLY, 0);
	if (fd < 0)
		err(1, "%s", _PATH_DEVPCI);

	bzero(&pc, sizeof(struct pci_conf_io));
	pc.match_buf_len = sizeof(conf);
	pc.matches = conf;

	do {
		if (ioctl(fd, PCIOCGETCONF, &pc) == -1)
			err(1, "ioctl(PCIOCGETCONF)");

		/*
		 * 255 entries should be more than enough for most people,
		 * but if someone has more devices, and then changes things
		 * around between ioctls, we'll do the cheesy thing and
		 * just bail.  The alternative would be to go back to the
		 * beginning of the list, and print things twice, which may
		 * not be desirable.
		 */
		if (pc.status == PCI_GETCONF_LIST_CHANGED) {
			warnx("PCI device list changed, please try again");
			exitstatus = 1;
			close(fd);
			return;
		} else if (pc.status ==  PCI_GETCONF_ERROR) {
			warnx("error returned from PCIOCGETCONF ioctl");
			exitstatus = 1;
			close(fd);
			return;
		}
		for (p = conf; p < &conf[pc.num_matches]; p++) {

			sprintf(v,"x%x",p->pc_vendor);
			sprintf(d,"0x%08x",p->pc_device <<16);
	                sprintf(kmodule,"%s",parse_XML("/usr/local/share/pcbsd/pc-hwdetect/network.xml",v,d));

		if ((dp = guess_class(p)) != NULL){
			#ifdef _DEBUG
			printf("    loading  %s.xml\n", dp);
			#endif

			memset(xml,'\0',sizeof xml);
			sprintf(xml,"/usr/local/share/pcbsd/pc-hwdetect/%s.xml",dp);

	              if(file_exists(xml))
	   		  sprintf(kmodule,"%s",parse_XML(xml,v,d));
			else
			{ 
			printf("** No XML definitions for this class <%s>**\n",dp);
			printf("** Not performing module search**\n\n\n");
			continue;
			}
		
		if(!memcmp(kmodule,"UNKNOWN",7))
		  {
		    printf("\nDevice not supported!\n ");
		    printf("\nPlease report the make & model to support@lists.pcbsd.org\n\n ");
		    printf("-------------------------------------------------------------\n");
		    list_verbose(p);
  		    list_bars(fd,p);
		    printf("    chip       = %s\n    vendor     = 0%s\n",d,v);
		    printf("-------------------------------------------------------------\n");
		    continue;
		 } 

		if(!memcmp(prevkmodule, kmodule, 16) || ( kld_isloaded(kmodule) != 0 ) )
		  continue;
	       
		if(kld_load(kmodule) <0){
		  warn("\nError loading module...%s",kmodule);
		} else { 
		 	printf("\n\n");
			printf("Kernel Module: %s loaded for this device.\n",kmodule);
			printf("-------------------------------------------------------------\n");
			list_verbose(p);
			list_bars(fd,p);
			strncpy(prevkmodule, kmodule,16);
			printf("-------------------------------------------------------------\n");
		}
	   }
	}
	} while (pc.status == PCI_GETCONF_MORE_DEVS);

	close(fd);
}

 void
list_bars(int fd, struct pci_conf *p)
{
	struct pci_bar_io bar;
	uint64_t base;
	const char *type;
	int i, range, max;

	switch (p->pc_hdr & PCIM_HDRTYPE) {
	case PCIM_HDRTYPE_NORMAL:
		max = PCIR_MAX_BAR_0;
		break;
	case PCIM_HDRTYPE_BRIDGE:
		max = PCIR_MAX_BAR_1;
		break;
	case PCIM_HDRTYPE_CARDBUS:
		max = PCIR_MAX_BAR_2;
		break;
	default:
		return;
	}

	for (i = 0; i <= max; i++) {
		bar.pbi_sel = p->pc_sel;
		bar.pbi_reg = PCIR_BAR(i);
		if (ioctl(fd, PCIOCGETBAR, &bar) < 0)
			continue;
		if (PCI_BAR_IO(bar.pbi_base)) {
			type = "I/O Port";
			range = 32;
			base = bar.pbi_base & PCIM_BAR_IO_BASE;
		} else {
			if (bar.pbi_base & PCIM_BAR_MEM_PREFETCH)
				type = "Prefetchable Memory";
			else
				type = "Memory";
			switch (bar.pbi_base & PCIM_BAR_MEM_TYPE) {
			case PCIM_BAR_MEM_32:
				range = 32;
				break;
			case PCIM_BAR_MEM_1MB:
				range = 20;
				break;
			case PCIM_BAR_MEM_64:
				range = 64;
				break;
			default:
				range = -1;
			}
			base = bar.pbi_base & ~((uint64_t)0xf);
		}
		printf("    bar   [%02x] = type %s, range %2d, base %#jx, ",
		    PCIR_BAR(i), type, range, (uintmax_t)base);
		printf("size %ju, %s\n", (uintmax_t)bar.pbi_length,
		    bar.pbi_enabled ? "enabled" : "disabled");
	}
}

 void
list_verbose(struct pci_conf *p)
{
	struct pci_vendor_info	*vi;
	struct pci_device_info	*di;
	const char *dp;

	
	TAILQ_FOREACH(vi, &pci_vendors, link) {
		if (vi->id == p->pc_vendor) {
			printf("    vendor     = '%s'\n", vi->desc);
			break;
		}
	}
	if (vi == NULL) {
		di = NULL;
	} else {
		TAILQ_FOREACH(di, &vi->devs, link) {
			if (di->id == p->pc_device) {
				printf("    device     = '%s'\n", di->desc);
				break;
			}
		}
	}
	if ((dp = guess_class(p)) != NULL)
		printf("    class      = %s\n", dp);
	if ((dp = guess_subclass(p)) != NULL)
		printf("    subclass   = %s\n", dp);
}

/*
 * This is a direct cut-and-paste from the table in sys/dev/pci/pci.c.
 */
 struct
{
	int	class;
	int	subclass;
	const char *desc;
} pci_nomatch_tab[] = {
	{PCIC_OLD,		-1,			"old"},
	{PCIC_OLD,		PCIS_OLD_NONVGA,	"non-VGA display device"},
	{PCIC_OLD,		PCIS_OLD_VGA,		"VGA-compatible display device"},
	{PCIC_STORAGE,		-1,			"mass storage"},
	{PCIC_STORAGE,		PCIS_STORAGE_SCSI,	"SCSI"},
	{PCIC_STORAGE,		PCIS_STORAGE_IDE,	"ATA"},
	{PCIC_STORAGE,		PCIS_STORAGE_FLOPPY,	"floppy disk"},
	{PCIC_STORAGE,		PCIS_STORAGE_IPI,	"IPI"},
	{PCIC_STORAGE,		PCIS_STORAGE_RAID,	"RAID"},
	{PCIC_STORAGE,		PCIS_STORAGE_ATA_ADMA,	"ATA (ADMA)"},
	{PCIC_STORAGE,		PCIS_STORAGE_SATA,	"SATA"},
	{PCIC_STORAGE,		PCIS_STORAGE_SAS,	"SAS"},
	{PCIC_STORAGE,		PCIS_STORAGE_NVM,	"NVM"},
	{PCIC_NETWORK,		-1,			"network"},
	{PCIC_NETWORK,		PCIS_NETWORK_ETHERNET,	"ethernet"},
	{PCIC_NETWORK,		PCIS_NETWORK_TOKENRING,	"token ring"},
	{PCIC_NETWORK,		PCIS_NETWORK_FDDI,	"fddi"},
	{PCIC_NETWORK,		PCIS_NETWORK_ATM,	"ATM"},
	{PCIC_NETWORK,		PCIS_NETWORK_ISDN,	"ISDN"},
	{PCIC_DISPLAY,		-1,			"display"},
	{PCIC_DISPLAY,		PCIS_DISPLAY_VGA,	"VGA"},
	{PCIC_DISPLAY,		PCIS_DISPLAY_XGA,	"XGA"},
	{PCIC_DISPLAY,		PCIS_DISPLAY_3D,	"3D"},
	{PCIC_MULTIMEDIA,	-1,			"multimedia"},
	{PCIC_MULTIMEDIA,	PCIS_MULTIMEDIA_VIDEO,	"video"},
	{PCIC_MULTIMEDIA,	PCIS_MULTIMEDIA_AUDIO,	"audio"},
	{PCIC_MULTIMEDIA,	PCIS_MULTIMEDIA_TELE,	"telephony"},
	{PCIC_MULTIMEDIA,	PCIS_MULTIMEDIA_HDA,	"HDA"},
	{PCIC_MEMORY,		-1,			"memory"},
	{PCIC_MEMORY,		PCIS_MEMORY_RAM,	"RAM"},
	{PCIC_MEMORY,		PCIS_MEMORY_FLASH,	"flash"},
	{PCIC_BRIDGE,		-1,			"bridge"},
	{PCIC_BRIDGE,		PCIS_BRIDGE_HOST,	"HOST-PCI"},
	{PCIC_BRIDGE,		PCIS_BRIDGE_ISA,	"PCI-ISA"},
	{PCIC_BRIDGE,		PCIS_BRIDGE_EISA,	"PCI-EISA"},
	{PCIC_BRIDGE,		PCIS_BRIDGE_MCA,	"PCI-MCA"},
	{PCIC_BRIDGE,		PCIS_BRIDGE_PCI,	"PCI-PCI"},
	{PCIC_BRIDGE,		PCIS_BRIDGE_PCMCIA,	"PCI-PCMCIA"},
	{PCIC_BRIDGE,		PCIS_BRIDGE_NUBUS,	"PCI-NuBus"},
	{PCIC_BRIDGE,		PCIS_BRIDGE_CARDBUS,	"PCI-CardBus"},
	{PCIC_BRIDGE,		PCIS_BRIDGE_RACEWAY,	"PCI-RACEway"},
	{PCIC_SIMPLECOMM,	-1,			"simple comms"},
	{PCIC_SIMPLECOMM,	PCIS_SIMPLECOMM_UART,	"UART"},	/* could detect 16550 */
	{PCIC_SIMPLECOMM,	PCIS_SIMPLECOMM_PAR,	"parallel port"},
	{PCIC_SIMPLECOMM,	PCIS_SIMPLECOMM_MULSER,	"multiport serial"},
	{PCIC_SIMPLECOMM,	PCIS_SIMPLECOMM_MODEM,	"generic modem"},
	{PCIC_BASEPERIPH,	-1,			"base peripheral"},
	{PCIC_BASEPERIPH,	PCIS_BASEPERIPH_PIC,	"interrupt controller"},
	{PCIC_BASEPERIPH,	PCIS_BASEPERIPH_DMA,	"DMA controller"},
	{PCIC_BASEPERIPH,	PCIS_BASEPERIPH_TIMER,	"timer"},
	{PCIC_BASEPERIPH,	PCIS_BASEPERIPH_RTC,	"realtime clock"},
	{PCIC_BASEPERIPH,	PCIS_BASEPERIPH_PCIHOT,	"PCI hot-plug controller"},
	{PCIC_BASEPERIPH,	PCIS_BASEPERIPH_SDHC,	"SD host controller"},
	{PCIC_INPUTDEV,		-1,			"input device"},
	{PCIC_INPUTDEV,		PCIS_INPUTDEV_KEYBOARD,	"keyboard"},
	{PCIC_INPUTDEV,		PCIS_INPUTDEV_DIGITIZER,"digitizer"},
	{PCIC_INPUTDEV,		PCIS_INPUTDEV_MOUSE,	"mouse"},
	{PCIC_INPUTDEV,		PCIS_INPUTDEV_SCANNER,	"scanner"},
	{PCIC_INPUTDEV,		PCIS_INPUTDEV_GAMEPORT,	"gameport"},
	{PCIC_DOCKING,		-1,			"docking station"},
	{PCIC_PROCESSOR,	-1,			"processor"},
	{PCIC_SERIALBUS,	-1,			"serial bus"},
	{PCIC_SERIALBUS,	PCIS_SERIALBUS_FW,	"FireWire"},
	{PCIC_SERIALBUS,	PCIS_SERIALBUS_ACCESS,	"AccessBus"},
	{PCIC_SERIALBUS,	PCIS_SERIALBUS_SSA,	"SSA"},
	{PCIC_SERIALBUS,	PCIS_SERIALBUS_USB,	"USB"},
	{PCIC_SERIALBUS,	PCIS_SERIALBUS_FC,	"Fibre Channel"},
	{PCIC_SERIALBUS,	PCIS_SERIALBUS_SMBUS,	"SMBus"},
	{PCIC_WIRELESS,		-1,			"wireless controller"},
	{PCIC_WIRELESS,		PCIS_WIRELESS_IRDA,	"iRDA"},
	{PCIC_WIRELESS,		PCIS_WIRELESS_IR,	"IR"},
	{PCIC_WIRELESS,		PCIS_WIRELESS_RF,	"RF"},
	{PCIC_INTELLIIO,	-1,			"intelligent I/O controller"},
	{PCIC_INTELLIIO,	PCIS_INTELLIIO_I2O,	"I2O"},
	{PCIC_SATCOM,		-1,			"satellite communication"},
	{PCIC_SATCOM,		PCIS_SATCOM_TV,		"sat TV"},
	{PCIC_SATCOM,		PCIS_SATCOM_AUDIO,	"sat audio"},
	{PCIC_SATCOM,		PCIS_SATCOM_VOICE,	"sat voice"},
	{PCIC_SATCOM,		PCIS_SATCOM_DATA,	"sat data"},
	{PCIC_CRYPTO,		-1,			"encrypt/decrypt"},
	{PCIC_CRYPTO,		PCIS_CRYPTO_NETCOMP,	"network/computer crypto"},
	{PCIC_CRYPTO,		PCIS_CRYPTO_NETCOMP,	"entertainment crypto"},
	{PCIC_DASP,		-1,			"dasp"},
	{PCIC_DASP,		PCIS_DASP_DPIO,		"DPIO module"},
	{0, 0,		NULL}
};

const char *
guess_class(struct pci_conf *p)
{
	int	i;

	for (i = 0; pci_nomatch_tab[i].desc != NULL; i++) {
		if (pci_nomatch_tab[i].class == p->pc_class)
			return(pci_nomatch_tab[i].desc);
	}
	return(NULL);
}

 const char *
guess_subclass(struct pci_conf *p)
{
	int	i;

	for (i = 0; pci_nomatch_tab[i].desc != NULL; i++) {
		if ((pci_nomatch_tab[i].class == p->pc_class) &&
		    (pci_nomatch_tab[i].subclass == p->pc_subclass))
			return(pci_nomatch_tab[i].desc);
	}
	return(NULL);
}

 int
load_vendors(void)
{
	const char *dbf;
	FILE *db;
	struct pci_vendor_info *cv;
	struct pci_device_info *cd;
	char buf[1024], str[1024];
	char *ch;
	int id, error;

	/*
	 * Locate the database and initialise.
	 */
	TAILQ_INIT(&pci_vendors);
	if ((dbf = getenv("PCICONF_VENDOR_DATABASE")) == NULL)
		dbf = _PATH_PCIVDB;
	if ((db = fopen(dbf, "r")) == NULL)
		return(1);
	cv = NULL;
	cd = NULL;
	error = 0;

	/*
	 * Scan input lines from the database
	 */
	for (;;) {
		if (fgets(buf, sizeof(buf), db) == NULL)
			break;

		if ((ch = strchr(buf, '#')) != NULL)
			*ch = '\0';
		ch = strchr(buf, '\0') - 1;
		while (ch > buf && isspace(*ch))
			*ch-- = '\0';
		if (ch <= buf)
			continue;

		/* Can't handle subvendor / subdevice entries yet */
		if (buf[0] == '\t' && buf[1] == '\t')
			continue;

		/* Check for vendor entry */
		if (buf[0] != '\t' && sscanf(buf, "%04x %[^\n]", &id, str) == 2) {
			if ((id == 0) || (strlen(str) < 1))
				continue;
			if ((cv = malloc(sizeof(struct pci_vendor_info))) == NULL) {
				warn("allocating vendor entry");
				error = 1;
				break;
			}
			if ((cv->desc = strdup(str)) == NULL) {
				free(cv);
				warn("allocating vendor description");
				error = 1;
				break;
			}
			cv->id = id;
			TAILQ_INIT(&cv->devs);
			TAILQ_INSERT_TAIL(&pci_vendors, cv, link);
			continue;
		}

		/* Check for device entry */
		if (buf[0] == '\t' && sscanf(buf + 1, "%04x %[^\n]", &id, str) == 2) {
			if ((id == 0) || (strlen(str) < 1))
				continue;
			if (cv == NULL) {
				warnx("device entry with no vendor!");
				continue;
			}
			if ((cd = malloc(sizeof(struct pci_device_info))) == NULL) {
				warn("allocating device entry");
				error = 1;
				break;
			}
			if ((cd->desc = strdup(str)) == NULL) {
				free(cd);
				warn("allocating device description");
				error = 1;
				break;
			}
			cd->id = id;
			TAILQ_INSERT_TAIL(&cv->devs, cd, link);
			continue;
		}

		/* It's a comment or junk, ignore it */
	}
	if (ferror(db))
		error = 1;
	fclose(db);

	return(error);
}

uint32_t
read_config(int fd, struct pcisel *sel, long reg, int width)
{
	struct pci_io pi;

	pi.pi_sel = *sel;
	pi.pi_reg = reg;
	pi.pi_width = width;

	if (ioctl(fd, PCIOCREAD, &pi) < 0)
		err(1, "ioctl(PCIOCREAD)");

	return (pi.pi_data);
}

