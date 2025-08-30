#include "tone.h"
#include "notes.h"

#define VALUE_DEFAULT 0

#define MAX_TONES 4

#define SET(x,y) (x |=(1<<y))

char currentSample = VALUE_DEFAULT;

Tone tones[MAX_TONES];

const char temporaryWavetable0[] = {
    15,15,0,0,15,15,0,0,0,0,15,15,15,15,0,0,15,15,15,15,15,15,0,0,0,0,0,0,0,0,0,0
};

const char temporaryWavetable1[] = {
    0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13,14,14,15,15
};

const char temporaryWavetable2[] = {
    15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

const char temporaryWavetable3[] = {
    7,8,9,10,10,11,11,12,12,12,12,12,12,12,12,12,11,11,10,10,9,8,7,2,1,0,0,0,0,0,1,2
};

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
        tones[i].advancePhase(1);
    }
}

SIGNAL(TIMER1_COMPA_vect){
    calculateSound();
    OCR2A = OCR2B = currentSample;
}

int main ()
{
    DDRD = DDRD | B00001000;

    initialize();

    tones[0].setPitch(PITCH(NOTE_A4));
    tones[0].setVolume(15);
    tones[0].setWavetable(temporaryWavetable2);
    tones[0].setNoise(0);
    
    while(1){
        
    }
}