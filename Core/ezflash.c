#include "ezflash.h"

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
}