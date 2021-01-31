#ifndef EZFLASH_H
#define EZFLASH_H

#define __EZFLASH_LOG__ "\n[EZ FLASH JR]"

#include "gb.h"
#include <stdio.h>

void GB_ezflash_init(GB_gameboy_t* gb);

void GB_ezflash_lock_register(GB_gameboy_t* gb, uint8_t value);

void GB_ezflash_set_sector(GB_gameboy_t* gb, uint8_t addr_offset, uint8_t value);

uint8_t GB_ezflash_read_sram(GB_gameboy_t* gb, uint16_t address);

void GB_ezflash_sd_map(GB_gameboy_t* gb, uint8_t value);

void GB_ezflash_sram_map(GB_gameboy_t* gb, uint8_t value);

void GB_ezflash_load_image(GB_gameboy_t* gb, char* image_filename);

void GB_ezflash_close_image(GB_gameboy_t* gb);

#endif /* EZFLASH_H */

