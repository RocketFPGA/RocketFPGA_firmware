#ifndef _ROCKETFPGA_CODEC_H
#define _ROCKETFPGA_CODEC_H


#define codec (*(volatile uint32_t*) 0x01000000)

#define write_codec_command(addr, value)    codec = ((addr & 0x7F) << 9) | (value & 0x1FF)
#define set_bit(data, bit)                  data |= 1UL << bit
#define unset_bit(data, bit)   		        data &= ~(1UL << bit)


// WM8731 Registers
#define CODEC_LEFT_LINE_REG        0x00  
#define CODEC_RIGHT_LINE_REG       0x01  
#define CODEC_LEFT_HP_REG          0x02  
#define CODEC_RIGHT_HP_REG         0x03  
#define CODEC_ANALOGUE_PATH_REG    0x04      
#define CODEC_DIGITAL_PATH_REG     0x05      
#define CODEC_POWER_CTL_REG        0x06  
#define CODEC_DIGITAL_IFACE_REG    0x07      
#define CODEC_SAMPLING_REG         0x08  
#define CODEC_ACTIVE_REG           0x09  
#define CODEC_RESET_REG            0x0F

// CODEC_POWER_CTL_REG
#define LINEINPD    0   
#define MICPD       1
#define ADCPD       2
#define DACPD       3
#define OUTPD       4
#define OSCPD       5
#define CLKOUTPD    6
#define POWEROFFPD  7

// CODEC_ANALOGUE_PATH_REG
#define MICBOOST        0   
#define MUTEMIC         1
#define INSEL           2
#define BYPASS          3
#define DACSEL          4
#define SIDETONE        5
#define SIDETONE_ATT    6

#define SIDETONE_ATT_M15DB    0b11
#define SIDETONE_ATT_M12DB    0b10
#define SIDETONE_ATT_M9DB     0b01
#define SIDETONE_ATT_M6DB     0b00

// CODEC_DIGITAL_PATH_REG
#define ADCHPD      0
#define DEEMPH      1
#define DACMU       3
#define HPOR        4

#define DEEMPH_48K            0b11
#define DEEMPH_44_1K          0b10
#define DEEMPH_32K            0b01
#define DEEMPH_DISABLE        0b00

// CODEC_DIGITAL_IFACE_REG
#define FORMAT      0
#define IWL         2
#define LRP         4
#define LRSWAP      5
#define MS          6
#define BCLKINV     7

#define FORMAT_DSP            0b11
#define FORMAT_I2S            0b10
#define FORMAT_LJ             0b01
#define FORMAT_RJ             0b00

#define IWL_32b               0b11
#define IWL_24b               0b10
#define IWL_20b               0b01
#define IWL_16b               0b00

// CODEC_SAMPLING_REG
#define USBNORMAL      0
#define BOSR           1
#define SR             2
#define CLKIDIV2       6
#define CLKODIV2       7

#define SR_48K_48K_12_288M    0b0000
#define SR_96K_96K_12_288M    0b0111
// Not completed SR, see page 41 in datasheet

#endif  // _ROCKETFPGA_CODEC_H