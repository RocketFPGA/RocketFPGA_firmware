#include <stdint.h>
#include <stdbool.h>
#include "printf.h"
#include "rocketfpga.h"

char protocol_buffer[100];

typedef enum {
	OSC,
	ADSR,
	ECHO,
	MATRIX,
	ERROR
} cmd_type ; 

typedef struct err_cmd {
	char * error;
} err_cmd;

typedef struct osc_cmd {
	uint8_t index;
	uint8_t type;
	uint32_t freq;
} osc_cmd;

typedef struct adsr_cmd {
	uint8_t index;
	uint8_t attack;
	uint8_t decay;
	uint8_t sustain;
	uint8_t release;
} adsr_cmd;

typedef struct echo_cmd {
	uint8_t index;
	uint8_t feedback;
	uint16_t delay;
} echo_cmd;

typedef struct matrix_cmd {
	uint8_t in;
	uint8_t out;
} matrix_cmd;

typedef struct rocket_cmd {
	cmd_type type;
	union {
		osc_cmd osc;
		adsr_cmd adrs;
		echo_cmd echo;
		matrix_cmd matrix;
		err_cmd error;
	};
} rocket_cmd;

void parse_command(rocket_cmd * cmd);

void read_rocket_command(){
	uint16_t index = 0;
	do{
		protocol_buffer[index++] = getchar();
		_putchar(protocol_buffer[index-1]);
	} while (protocol_buffer[index-1] != '\n');

	rocket_cmd cmd;
	parse_command(&cmd);
	
	switch (cmd.type){
	case OSC:
		printf("Setting up osc %d at %d Hz\n", cmd.osc.index, cmd.osc.freq);
		oscs[cmd.osc.index-1] = phase_from_freq(cmd.osc.freq);
		break;
	case ECHO:
		printf("Setting up echo %d with feedback %d and delay %d\n", cmd.echo.feedback, cmd.echo.delay);
		echo_delay = cmd.echo.delay;
		break;
	case ADSR:
		printf("Setting up adsr\n");
		set_attack(adsr,cmd.adrs.attack);
		set_decay(adsr,cmd.adrs.decay);	
		set_sustain(adsr,cmd.adrs.sustain);
		set_release(adsr,cmd.adrs.release);
		break;
	case MATRIX:
		printf("Routing %s to %s\n", matrix_in_names[cmd.matrix.in], matrix_out_names[cmd.matrix.out]);
		set_matrix(cmd.matrix.in, cmd.matrix.out);
		break;
	case ERROR:
		printf("Error: %s\n", cmd.error.error);
		break;
	}
	
	// Clean the buffer
	protocol_buffer[0] = '\0';

}

bool check_cmd_header(char cmp[]){
	bool rv = true;
	for (uint8_t i = 0; i < 3; i++){
		if (protocol_buffer[i] != cmp[i]){
			rv = false;
			break;
		}
	}
	return rv;
}
	
uint32_t pow10(uint8_t exp){
	uint32_t res = 1;
	for (uint8_t i = 0; i < exp; i++)
	{
		res *= 10;
	}
	return res;
}

uint16_t parse_uint16(char s[], uint8_t len){
	uint16_t res = 0;
	for (uint8_t i = 0; i < len; i++)
	{
		res += (s[i] - '0') * pow10((len-1)-i);
	}
	return res;
}

void parse_command(rocket_cmd * cmd){
	if (check_cmd_header("OSC")){
		cmd->type = OSC;
		cmd->osc.index 	= parse_uint16(&protocol_buffer[3], 1);
		cmd->osc.freq 	= parse_uint16(&protocol_buffer[5], 5);
		cmd->osc.type 	= 0;

		if (cmd->osc.index >= 5 || cmd->osc.index == 0){
			cmd->type = ERROR;
			cmd->error.error = "Only available 1-4 oscillators";
		}
		
	} else if (check_cmd_header("ADS")){
		cmd->type = ADSR;
		cmd->adrs.index  	= parse_uint16(&protocol_buffer[3], 1);
		cmd->adrs.attack 	= parse_uint16(&protocol_buffer[4], 5);
		cmd->adrs.decay 	= parse_uint16(&protocol_buffer[9], 5);
		cmd->adrs.sustain 	= parse_uint16(&protocol_buffer[14], 5);
		cmd->adrs.release 	= parse_uint16(&protocol_buffer[19], 5);

		if (cmd->adrs.index >= 2 || cmd->adrs.index == 0){
			cmd->type = ERROR;
			cmd->error.error = "Only available 1 ADSR";
		}

	} else if (check_cmd_header("ECH")){
		cmd->type = ECHO;
		cmd->echo.index  	= parse_uint16(&protocol_buffer[3], 1);
		cmd->echo.feedback 	= parse_uint16(&protocol_buffer[4], 3);
		cmd->echo.delay 	= parse_uint16(&protocol_buffer[7], 4);

		if (cmd->echo.index >= 2 || cmd->echo.index == 0){
			cmd->type = ERROR;
			cmd->error.error = "Only available 1 echo";
		}

	} else if (check_cmd_header("MAT")){
		cmd->type = MATRIX;
		cmd->matrix.in  	= parse_uint16(&protocol_buffer[3], 2);
		cmd->matrix.out 	= parse_uint16(&protocol_buffer[5], 2);

	}else{
		cmd->type = ERROR;
		cmd->error.error = "Unknown header";
	}
}