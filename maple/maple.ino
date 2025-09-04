#include "tone.hpp"
#include "notes.hpp"

#define VALUE_DEFAULT 0

#define MAX_TONES 4

unsigned char currentSample = VALUE_DEFAULT;

Tone tones[MAX_TONES];

void initialize() {
    TCCR1A = 0x00;
    TCCR1B = 0x0A;
    TCCR1C = 0x00;
    OCR1A = 150;
    TIMSK1 |= (1<<OCIE1A);

    TCCR2A = 0xB3;
    TCCR2B = 0x01;
    OCR2A = OCR2B = 0;

    DDRB = 0b00001000;
    
    PCICR = 0b00000001;
    PCMSK0 = 0b0010000;

    sei();
}

void calculateSound() {
    currentSample = VALUE_DEFAULT;

    char i = MAX_TONES;
    while (--i >= 0){
        currentSample += tones[i].generateSample();
        tones[i].advancePhase();
    }
}

SIGNAL(TIMER1_COMPA_vect){
    calculateSound(); 
    OCR2A = OCR2B = currentSample;
}

ISR(PCINT0_vect){

    if (PINB & 0b00100000) return;

    unsigned char address = PINC & 0b00011111;
    unsigned char value = PIND;

    switch(address){
        case 0: 
            tones[0].setIndex(value); // set the channel 0 sample index
            break;
        case 1:
            tones[0].setWavetableSamples(value); // set the channel 0 wavetable sample at index current index
            break;
        case 2:
            tones[1].setIndex(value); // set the channel 1 sample index
            break;
        case 3:
            tones[1].setWavetableSamples(value); // set the channel 1 wavetable sample at index current index
            break;
        case 4:
            tones[2].setIndex(value); // set the channel 2 sample index
            break;
        case 5:
            tones[2].setWavetableSamples(value); // set the channel 2 wavetable sample at index current index
            break;
        case 6:
            tones[3].setIndex(value); // set the channel 3 sample index
            break;
        case 7:
            tones[3].setWavetableSamples(value); // set the channel 2 wavetable sample at index current index
            break;
        case 8:
            tones[0].setPitchHigh(value); // set channel 0 pitch (high byte)
            break;
        case 9:
            tones[0].setPitchLow(value); // set channel 0 pitch (low byte)
            break;
        case 10:
            tones[1].setPitchHigh(value); // set channel 1 pitch (high byte)
            break;
        case 11:
            tones[1].setPitchLow(value); // set channel 1 pitch (low byte)
            break;
        case 12:
            tones[2].setPitchHigh(value); // set channel 2 pitch (high byte)
            break;
        case 13:
            tones[2].setPitchLow(value); // set channel 2 pitch (low byte)
            break;
        case 14:
            tones[3].setPitchHigh(value); // set channel 3 pitch (high byte)
            break;
        case 15:
            tones[3].setPitchLow(value); // set channel 3 pitch (low byte)
            break;
        case 16:
            tones[0].setVolume(value & 0b00001111); // set volume of channel 0 using the low 4 bits
            tones[1].setVolume((value & 0b11110000) >> 4); // set volume of channel 1 using the high 4 bits
            break;
        case 17:
            tones[2].setVolume(value & 0b00001111); // set volume of channel 2 using the low 4 bits
            tones[3].setVolume((value & 0b11110000) >> 4); // set volume of channel 3 using the high 4 bits
            break;
        case 18:
            tones[0].setNoise(value & 0b00000001); // set the noise mode of channel 0
            tones[1].setNoise(value & 0b00000010); // set the noise mode of channel 1
            tones[2].setNoise(value & 0b00000100); // set the noise mode of channel 2
            tones[3].setNoise(value & 0b00001000); // set the noise mode of channel 3
            break;
    }
}

int main ()
{
    initialize();

    while(1){
    }
}