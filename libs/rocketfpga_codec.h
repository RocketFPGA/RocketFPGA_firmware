#ifndef _ROCKETFPGA_CODEC_H
#define _ROCKETFPGA_CODEC_H

#define codec (*(volatile uint32_t*) 0x01000000)

#define write_codec_command(addr, value)    codec = ((addr & 0x7F) << 9) & (value & 0x1FF)

#endif  // _ROCKETFPGA_CODEC_H