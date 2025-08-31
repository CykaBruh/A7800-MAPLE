#include "tone.hpp"
#include "notes.hpp"

#define VALUE_DEFAULT 0

#define MAX_TONES 4

#define SET(x,y) (x |=(1<<y))

unsigned char currentSample = VALUE_DEFAULT;

Tone tones[MAX_TONES];

void initialize() {
    TCCR1A = 0x00;
    TCCR1B = 0x0A;
    TCCR1C = 0x00;
    OCR1A = 150;
    SET(TIMSK1, OCIE1A);
    sei();

    TCCR2A = 0xB3;
    TCCR2B = 0x01;
    OCR2A = OCR2B = 0;
    SET(DDRB, 3);
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

int main ()
{
    DDRD = DDRD | 0b00001000;

    initialize();

    tones[0].setPitch(PITCH(NOTE_C3));
    tones[0].setVolume(15);
    tones[0].setNoise(0);

    const char temporaryWavetable[] = {
        15,15,0,0,15,15,0,0,0,0,15,15,15,15,0,0,15,15,15,15,15,15,0,0,0,0,0,0,0,0,0,0
    };

    unsigned char i = -1;

    while (++i < 16){

        unsigned char samples = (temporaryWavetable[(i<<1) + 1] << 4) + temporaryWavetable[i<<1];
        tones[0].setWavetableSamples(i, samples);
    }
    
    while(1){
    }
}