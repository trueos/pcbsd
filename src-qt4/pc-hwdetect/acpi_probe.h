#ifndef ACPIPROBE_H
#define ACPIPROBE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/linker.h>
int acpi_probe(char** ids,char* ko);
int look_for_acpi();
int load_acpi(char* ko);
#endif
