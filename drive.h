#ifndef DRIVE_H
#define DRIVE_H

#define HDA_SECTORSIZE 256
#define HDA_IRQ  14
#define HDA_CMDREG 0x3F6
#define HDA_DATAREGS 0x110  
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hardware.h"
#include <assert.h>

void read_sector(unsigned int cylinder, unsigned int sector, unsigned char *buffer);
void write_sector(unsigned int cylinder, unsigned int sector,const unsigned char *buffer);
void format_sector(unsigned int cylinder, unsigned int sector, unsigned int nsector, unsigned int value);
void check_hda (void);
void init_master(void);

#endif
