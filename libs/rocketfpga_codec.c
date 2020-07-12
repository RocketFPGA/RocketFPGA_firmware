#include <stdint.h>
#include <stdbool.h>
#include "rocketfpga_codec.h"

uint16_t codec_reg_map[9] = {	0b010010111,
								0b010010111,
								0b001111001,
								0b001111001,
								0b000001010,
								0b000001000,
								0b010011111,
								0b010011111,
								0b000000000,
								0b000000000};

void init_codec(){
	reset_codec();

	set_codec_powerdown_all(0);
	set_codec_powerdown(OUTPD, 1);

	set_codec_line_volume(0);
	set_codec_headphone_volume(120);

	set_codec_analog_path(MICBOOST, 1);
	set_codec_analog_path(MUTEMIC, 0);
	set_codec_mic_input();
	set_codec_analog_path(BYPASS, 0);
	set_codec_analog_path(DACSEL, 1);
	set_codec_analog_path(SIDETONE, 0);

	set_codec_digital_path(ADCHPD, 0);
	set_codec_deemphasis(DEEMPH_DISABLE);
	set_codec_digital_path(DACMU, 0);
	set_codec_digital_path(HPOR, 0);

	set_codec_format(FORMAT_DSP);
	set_codec_iwl(IWL_16b);
	set_codec_digital_iface(LRP, 1);
	set_codec_digital_iface(LRSWAP, 0);
	set_codec_digital_iface(MS, 1);
	set_codec_digital_iface(BCLKINV, 0);

	set_codec_sampling_reg(USBNORMAL, 0);
	set_codec_sampling_reg(BOSR, 0);
	set_codec_sampling(SR_48K_48K_12_288M);
	set_codec_sampling_reg(CLKIDIV2, 0);
	set_codec_sampling_reg(CLKODIV2, 0);

	set_codec_active(1);
	set_codec_powerdown(OUTPD, 0);
}

uint16_t get_codec_reg(uint8_t reg){
	return codec_reg_map[reg];
}

void set_codec_reg(uint8_t reg, uint16_t value){
	codec_reg_map[reg] = value;
	write_codec_command(reg, value);
}

void reset_codec(){
	set_codec_reg(CODEC_RESET_REG, 0);
}

void set_codec_left_line_volume(uint8_t vol){
	vol = (vol > 31) ? 31 : vol; 
	set_codec_reg(CODEC_LEFT_LINE_REG, 0b000000000 | (vol & 0x1F));
}

void set_codec_right_line_volume(uint8_t vol){
	vol = (vol > 31) ? 31 : vol; 
	set_codec_reg(CODEC_RIGHT_LINE_REG, 0b000000000 | (vol & 0x1F));
}

void set_codec_line_volume(uint8_t vol){
	vol = (vol > 31) ? 31 : vol; 
	set_codec_reg(CODEC_LEFT_LINE_REG, 0b100000000 | (vol & 0x1F));
}

void set_codec_left_headphone_volume(uint8_t vol){
	vol = (vol > 127) ? 127 : vol; 
	set_codec_reg(CODEC_LEFT_HP_REG, 0b000000000 | (vol & 0x7F));
}

void set_codec_right_headphone_volume(uint8_t vol){
	vol = (vol > 127) ? 127 : vol; 
	set_codec_reg(CODEC_RIGHT_HP_REG, 0b000000000 | (vol & 0x7F));
}

void set_codec_headphone_volume(uint8_t vol){
	vol = (vol > 127) ? 127 : vol; 
	set_codec_reg(CODEC_LEFT_HP_REG, 0b100000000 | (vol & 0x7F));
}

// CODEC_POWER_CTL_REG
void set_codec_powerdown(uint8_t section, bool value){
	uint16_t aux = get_codec_reg(CODEC_POWER_CTL_REG);
	
	if (value){
		set_bit(aux, section);
	}else{
		unset_bit(aux, section);
	}
	
	set_codec_reg(CODEC_POWER_CTL_REG, aux);
}

void set_codec_powerdown_all(bool value){
	set_codec_power(LINEINPD, value);
	set_codec_power(MICPD, value);
	set_codec_power(ADCPD, value);
	set_codec_power(DACPD, value);
	set_codec_power(OUTPD, value);
	set_codec_power(OSCPD, value);
	set_codec_power(CLKOUTPD, value);
	set_codec_power(POWEROFFPD, value);
}

// CODEC_ANALOGUE_PATH_REG
void set_codec_analog_path(uint8_t section, bool value){
	uint16_t aux = get_codec_reg(CODEC_ANALOGUE_PATH_REG);
	
	if (value){
		set_bit(aux, section);
	}else{
		unset_bit(aux, section);
	}
	
	set_codec_reg(CODEC_ANALOGUE_PATH_REG, aux);
}

void set_codec_mic_input(){
	set_codec_analog_path(INSEL, 1);
}

void set_codec_line_input(){
	set_codec_analog_path(INSEL, 0);
}

void set_codec_sidetone_att(uint8_t attenuation){
	uint16_t aux = get_codec_reg(CODEC_ANALOGUE_PATH_REG);
	aux &= ~(0x3 << SIDETONE_ATT);
	aux |= attenuation << SIDETONE_ATT;

	set_codec_reg(CODEC_ANALOGUE_PATH_REG, aux);
}

// CODEC_DIGITAL_PATH_REG
void set_codec_digital_path(uint8_t section, bool value){
	uint16_t aux = get_codec_reg(CODEC_DIGITAL_PATH_REG);
	
	if (value){
		set_bit(aux, section);
	}else{
		unset_bit(aux, section);
	}
	
	set_codec_reg(CODEC_DIGITAL_PATH_REG, aux);
}

void set_codec_deemphasis(uint8_t deemphasis){
	uint16_t aux = get_codec_reg(CODEC_DIGITAL_PATH_REG);
	aux &= ~(0x3 << DEEMPH);
	aux |= deemphasis << DEEMPH;

	set_codec_reg(CODEC_DIGITAL_PATH_REG, aux);
}

// CODEC_DIGITAL_IFACE_REG
void set_codec_digital_iface(uint8_t section, bool value){
	uint16_t aux = get_codec_reg(CODEC_DIGITAL_IFACE_REG);
	
	if (value){
		set_bit(aux, section);
	}else{
		unset_bit(aux, section);
	}
	
	set_codec_reg(CODEC_DIGITAL_IFACE_REG, aux);
}

void set_codec_format(uint8_t format){
	uint16_t aux = get_codec_reg(CODEC_DIGITAL_IFACE_REG);
	aux &= ~(0x3 << FORMAT);
	aux |= format << FORMAT;

	set_codec_reg(CODEC_DIGITAL_IFACE_REG, aux);
}

void set_codec_iwl(uint8_t lenght){
	uint16_t aux = get_codec_reg(CODEC_DIGITAL_IFACE_REG);
	aux &= ~(0x3 << IWL);
	aux |= lenght << IWL;

	set_codec_reg(CODEC_DIGITAL_IFACE_REG, aux);
}


// CODEC_SAMPLING_REG
void set_codec_sampling_reg(uint8_t section, bool value){
	uint16_t aux = get_codec_reg(CODEC_SAMPLING_REG);
	
	if (value){
		set_bit(aux, section);
	}else{
		unset_bit(aux, section);
	}
	
	set_codec_reg(CODEC_SAMPLING_REG, aux);
}

void set_codec_sampling(uint8_t sampling){
	uint16_t aux = get_codec_reg(CODEC_DIGITAL_IFACE_REG);
	aux &= ~(0xF << SR);
	aux |= sampling << SR;

	set_codec_reg(CODEC_DIGITAL_IFACE_REG, aux);
}

// CODEC_ACTIVE_REG 
void set_codec_active(bool value){
	uint16_t aux = get_codec_reg(CODEC_ACTIVE_REG);
	
	if (value){
		set_bit(aux, 0);
	}else{
		unset_bit(aux, 0);
	}
	
	set_codec_reg(CODEC_ACTIVE_REG, aux);
}