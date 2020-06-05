#include <stdint.h>
#include "rocketfpga_codec.h"

void init_codec(){
	write_codec_command(0xFF, 0xFFF);

	// write_codec_command(0x0F, 0x00);
	// write_codec_command(0x06, 0x00);
	// write_codec_command(0x06, 0x10);
	// write_codec_command(0x00, 0x11F);
	// write_codec_command(0x02, 0x11F);
	// write_codec_command(0x04, 0x15);
	// write_codec_command(0x05, 0x01);
	// write_codec_command(0x07, 0x53);
	// write_codec_command(0x08, 0x0);
	// write_codec_command(0x09, 0x1);
	// write_codec_command(0x06, 0x00);
}