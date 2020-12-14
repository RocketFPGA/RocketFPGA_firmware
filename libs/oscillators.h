#ifndef _OSCILLATORS_LEDS_H
#define _OSCILLATORS_LEDS_H

// Memory mapped oscillators
#define oscs ((volatile uint32_t*)0x10000000)

#define osc_1 (*(volatile uint32_t*)0x10000000)
#define osc_2 (*(volatile uint32_t*)0x10000004)
#define osc_3 (*(volatile uint32_t*)0x10000008)
#define osc_4 (*(volatile uint32_t*)0x1000000C)

#define osc_type (*(volatile uint32_t*)0x10000028)
#define OSC1_TYPE 30
#define OSC2_TYPE 28
#define OSC3_TYPE 26
#define OSC4_TYPE 24

#define SINE_TYPE 0
#define RAMP_TYPE 1
#define SQUARE_TYPE 2
#define TRIANGLE_TYPE 3

#define set_osc_type(x,a) 	osc_type = 	(osc_type & ~(0x3 << x)) | ((a & 0x3) << x)

static uint32_t phase_from_freq(uint32_t freq){
	return freq * POWTWO(PHASE_SIZE) / SAMPLING_FREQ;  
}

static uint32_t freq_from_phase(uint32_t phase){
	return phase *  SAMPLING_FREQ / POWTWO(PHASE_SIZE);
}

// Scales

#define OCTAVES 10
#define NOTES_PER_OCTAVE 12

#define NOTE_C   0
#define NOTE_Cs  1
#define NOTE_Db  1
#define NOTE_D   2
#define NOTE_Ds  3
#define NOTE_Eb  3
#define NOTE_E   4
#define NOTE_F   5
#define NOTE_Fs  6
#define NOTE_Gb  6
#define NOTE_G   7
#define NOTE_Gs  8
#define NOTE_Ab  8
#define NOTE_A   9
#define NOTE_As  10
#define NOTE_Bb  10
#define NOTE_B   11

// Generated with:
// from math import pow
// C0 = 16.351
// a = pow(2, 1/12)
// for octave in range(10):
//     for note in range(12):
//         f = pow(2,octave)*C0*pow(a,note)

const static float notes[] = {16.351, 17.323281055868836, 18.353376951906558, 19.444725537409493, 20.600969086831075, 21.825966455534235, 23.12380595836248, 24.498819014010625, 25.955594600732038, 27.49899457162698, 29.13416988062539, 30.866577773316756, 32.702, 34.64656211173767, 36.706753903813116, 38.889451074818986, 41.20193817366215, 43.65193291106847, 46.24761191672496, 48.99763802802125, 51.911189201464076, 54.99798914325396, 58.26833976125078, 61.73315554663351, 65.404, 69.29312422347535, 73.41350780762623, 77.77890214963797, 82.4038763473243, 87.30386582213694, 92.49522383344993, 97.9952760560425, 103.82237840292815, 109.99597828650792, 116.53667952250156, 123.46631109326702, 130.808, 138.5862484469507, 146.82701561525246, 155.55780429927594, 164.8077526946486, 174.60773164427388, 184.99044766689985, 195.990552112085, 207.6447568058563, 219.99195657301584, 233.07335904500312, 246.93262218653405, 261.616, 277.1724968939014, 293.6540312305049, 311.1156085985519, 329.6155053892972, 349.21546328854777, 369.9808953337997, 391.98110422417, 415.2895136117126, 439.9839131460317, 466.14671809000623, 493.8652443730681, 523.232, 554.3449937878028, 587.3080624610099, 622.2312171971038, 659.2310107785944, 698.4309265770955, 739.9617906675994, 783.96220844834, 830.5790272234252, 879.9678262920634, 932.2934361800125, 987.7304887461362, 1046.464, 1108.6899875756055, 1174.6161249220197, 1244.4624343942075, 1318.4620215571888, 1396.861853154191, 1479.9235813351988, 1567.92441689668, 1661.1580544468504, 1759.9356525841267, 1864.586872360025, 1975.4609774922724, 2092.928, 2217.379975151211, 2349.2322498440394, 2488.924868788415, 2636.9240431143776, 2793.723706308382, 2959.8471626703977, 3135.84883379336, 3322.316108893701, 3519.8713051682535, 3729.17374472005, 3950.9219549845448, 4185.856, 4434.759950302422, 4698.464499688079, 4977.84973757683, 5273.848086228755, 5587.447412616764, 5919.694325340795, 6271.69766758672, 6644.632217787402, 7039.742610336507, 7458.3474894401, 7901.8439099690895, 8371.712, 8869.519900604844, 9396.928999376158, 9955.69947515366, 10547.69617245751, 11174.894825233529, 11839.38865068159, 12543.39533517344, 13289.264435574803, 14079.485220673014, 14916.6949788802, 15803.687819938179};

#if (SAMPLING_FREQ == 48000) && (PHASE_SIZE == 16)
const static uint32_t notes_acc[] = {22, 24, 25, 27, 28, 30, 32, 33, 35, 38, 40, 42, 45, 47, 50, 53, 56, 60, 63, 67, 71, 75, 80, 84, 89, 95, 100, 106, 113, 119, 126, 134, 142, 150, 159, 169, 179, 189, 200, 212, 225, 238, 253, 268, 284, 300, 318, 337, 357, 378, 401, 425, 450, 477, 505, 535, 567, 601, 636, 674, 714, 757, 802, 850, 900, 954, 1010, 1070, 1134, 1201, 1273, 1349, 1429, 1514, 1604, 1699, 1800, 1907, 2021, 2141, 2268, 2403, 2546, 2697, 2858, 3027, 3207, 3398, 3600, 3814, 4041, 4281, 4536, 4806, 5092, 5394, 5715, 6055, 6415, 6796, 7201, 7629, 8082, 8563, 9072, 9612, 10183, 10789, 11430, 12110, 12830, 13593, 14401, 15257, 16165, 17126, 18144, 19223, 20366, 21577};
#else
#error Please recalculate accumulator for notes
#endif

typedef enum{
	CHORD_FIRST_INVERSION = 0,
	CHORD_SECOND_INVERSION,
	CHORD_THIRD_INVERSION,
} inversion_t;

typedef struct {
	uint32_t notes[4];
	uint8_t len;	
} chord_t;

static chord_t generate_major_chord(uint8_t root, uint8_t octave, inversion_t inversion){
	chord_t chord;
	chord.len = 3;

	uint8_t root_offset = (octave*NOTES_PER_OCTAVE)+root;
	uint8_t note_index[3];
	switch (inversion){
		case CHORD_FIRST_INVERSION:
			note_index[0] = root_offset;
			note_index[1] = root_offset + 4;
			note_index[2] = root_offset + 7;
			break;
		
		case CHORD_SECOND_INVERSION:
			note_index[0] = root_offset + 4;
			note_index[1] = root_offset + 7;
			note_index[2] = root_offset + NOTES_PER_OCTAVE;
			break;	
		
		case CHORD_THIRD_INVERSION:
			note_index[0] = root_offset + 7;
			note_index[1] = root_offset + NOTES_PER_OCTAVE;
			note_index[2] = root_offset + 4 + NOTES_PER_OCTAVE;
			break;	
	}

	for (uint8_t i = 0; i < chord.len; i++){
		chord.notes[i] = notes_acc[note_index[i]];
	}
	
	return chord;
}

static chord_t generate_minor_chord(uint8_t root, uint8_t octave, inversion_t inversion){
	chord_t chord;
	chord.len = 3;

	uint8_t root_offset = (octave*NOTES_PER_OCTAVE)+root;
	uint8_t note_index[3];
	switch (inversion){
		case CHORD_FIRST_INVERSION:
			note_index[0] = root_offset;
			note_index[1] = root_offset + 3;
			note_index[2] = root_offset + 7;
			break;
		
		case CHORD_SECOND_INVERSION:
			note_index[0] = root_offset + 3;
			note_index[1] = root_offset + 7;
			note_index[2] = root_offset + NOTES_PER_OCTAVE;
			break;	
		
		case CHORD_THIRD_INVERSION:
			note_index[0] = root_offset + 7;
			note_index[1] = root_offset + NOTES_PER_OCTAVE;
			note_index[2] = root_offset + 3 + NOTES_PER_OCTAVE;
			break;	
	}

	for (uint8_t i = 0; i < chord.len; i++){
		chord.notes[i] = notes_acc[note_index[i]];
	}
	
	return chord;
}

static chord_t generate_dominant7_chord(uint8_t root, uint8_t octave, inversion_t inversion){
	chord_t chord;
	chord.len = 4;

	uint8_t root_offset = (octave*NOTES_PER_OCTAVE)+root;
	uint8_t note_index[4];
	switch (inversion){
		case CHORD_FIRST_INVERSION:
			note_index[0] = root_offset;
			note_index[1] = root_offset + 4;
			note_index[2] = root_offset + 7;
			note_index[3] = root_offset + 10;
			break;
		
		case CHORD_SECOND_INVERSION:
			note_index[0] = root_offset + 4;
			note_index[1] = root_offset + 7;
			note_index[2] = root_offset + 10;
			note_index[3] = root_offset + NOTES_PER_OCTAVE;
			break;	
		
		case CHORD_THIRD_INVERSION:
			note_index[0] = root_offset + 7;
			note_index[1] = root_offset + 10;
			note_index[2] = root_offset + NOTES_PER_OCTAVE;
			note_index[3] = root_offset + 4 + NOTES_PER_OCTAVE;
			break;	
	}

	for (uint8_t i = 0; i < chord.len; i++){
		chord.notes[i] = notes_acc[note_index[i]];
	}
	
	return chord;
}

static chord_t generate_maj7_chord(uint8_t root, uint8_t octave, inversion_t inversion){
	chord_t chord;
	chord.len = 4;

	uint8_t root_offset = (octave*NOTES_PER_OCTAVE)+root;
	uint8_t note_index[4];
	switch (inversion){
		case CHORD_FIRST_INVERSION:
			note_index[0] = root_offset;
			note_index[1] = root_offset + 4;
			note_index[2] = root_offset + 7;
			note_index[3] = root_offset + 11;
			break;
	}

	for (uint8_t i = 0; i < chord.len; i++){
		chord.notes[i] = notes_acc[note_index[i]];
	}
	
	return chord;
}

static chord_t generate_min7_chord(uint8_t root, uint8_t octave, inversion_t inversion){
	chord_t chord;
	chord.len = 4;

	uint8_t root_offset = (octave*NOTES_PER_OCTAVE)+root;
	uint8_t note_index[4];
	switch (inversion){
		case CHORD_FIRST_INVERSION:
			note_index[0] = root_offset;
			note_index[1] = root_offset + 3;
			note_index[2] = root_offset + 7;
			note_index[3] = root_offset + 10;
			break;
	}

	for (uint8_t i = 0; i < chord.len; i++){
		chord.notes[i] = notes_acc[note_index[i]];
	}
	
	return chord;
}

#endif  // _OSCILLATORS_LEDS_H