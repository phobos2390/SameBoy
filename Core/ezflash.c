#include "ezflash.h"

char* backup_image_filename = NULL;
FILE* sd_card_image = NULL;
GB_cartridge_t ezflash_cartridge_type = {
    GB_EZFL, GB_EZFLASH_JR, true, true, true, true
};
uint8_t* sd_card_low_buffer = NULL;
uint8_t* sd_card_high_buffer = NULL;
uint8_t sd_card_status = 0;

void GB_ezflash_init(GB_gameboy_t* gb)
{
    GB_ezflash_load_image(gb, backup_image_filename);
    gb->cartridge_type = &ezflash_cartridge_type;
    sd_card_low_buffer = malloc(0x1000);
    sd_card_high_buffer = malloc(0x1000);
}

void GB_ezflash_free(GB_gameboy_t* gb)
{
    if(sd_card_high_buffer)
    {
        free(sd_card_high_buffer);
    }
    if(sd_card_low_buffer)
    {
        free(sd_card_low_buffer);
    }
    gb->cartridge_type = 0;
    GB_ezflash_close_image(gb);
}

void GB_ezflash_lock_register(GB_gameboy_t* gb, uint8_t value)
{
    uint8_t new_lock_value = (value & 0x3);
    if((value & 0xF0) == 0xE0)
    {
        switch(value & 0x0F)
        {
            case 0x1:
            case 0x2:
            case 0x3:
                gb->ezflash_jr.lock
                    = (gb->ezflash_jr.lock == (new_lock_value - 1)) 
                      ? new_lock_value : 0;
                break;
            default:
                gb->ezflash_jr.lock = 0;
                break;
        }
    }
    GB_log(gb, "%s Lock level set to %x", __EZFLASH_LOG__, gb->ezflash_jr.lock);
}

void GB_ezflash_set_sector(GB_gameboy_t* gb, uint8_t addr_offset, uint8_t value)
{
    uint8_t index;
#ifdef GB_BIG_ENDIAN
    index = sizeof(uint32_t) - addr_offset - 1;
#else
    index = addr_offset;
#endif
    gb->ezflash_jr.sd_card.sector_array[index] = value;
}

uint8_t GB_ezflash_read_sram(GB_gameboy_t* gb, uint16_t address)
{
    GB_log(gb, "%s Attempted read of SRAM (addr %x)", __EZFLASH_LOG__, address);
    return 0xFF;
}

void GB_ezflash_sd_map(GB_gameboy_t* gb, uint8_t value)
{
    switch(value)
    {
        case 0:
            GB_log(gb, "%s SD mapping disabled", __EZFLASH_LOG__);
            break;
        case 1:
            GB_log(gb, "%s SD sector data set to 0xA0000", __EZFLASH_LOG__);
            fseek(sd_card_image, 0, gb->ezflash_jr.sd_card.sector * 0x200);
            gb->mbc_ram = sd_card_low_buffer;
            gb->mbc_ram_size = 0x800;
            fread(sd_card_low_buffer, 1, gb->mbc_ram_size, sd_card_image);
            break;
        case 3:
            GB_log(gb, "%s SD status mapped to 0xA0000", __EZFLASH_LOG__);
            sd_card_status = 0x1;
            gb->mbc_ram = &sd_card_status;
            gb->mbc_ram_size = 1;
            break;
        default:
            GB_log(gb, "%s SD mapping set to %x", __EZFLASH_LOG__, value);
            break;
    }
}

void GB_ezflash_sram_map(GB_gameboy_t* gb, uint8_t value)
{
    GB_log(gb, "%s SRAM mapping set to %x", __EZFLASH_LOG__, value);
}

void GB_ezflash_load_image(GB_gameboy_t* gb, char* image_filename)
{
    if((sd_card_image == NULL) && (image_filename != NULL))
    {
        GB_log(gb, "%s Reading image filename %s", __EZFLASH_LOG__, image_filename);
        sd_card_image = fopen(image_filename, "rwb");
    }
    backup_image_filename = image_filename;
}

void GB_ezflash_close_image(GB_gameboy_t* gb)
{
    GB_log(gb, "%s Closing image file", __EZFLASH_LOG__);
    if(sd_card_image != NULL){
        fclose(sd_card_image);
    }
}