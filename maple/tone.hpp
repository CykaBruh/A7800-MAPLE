#ifndef __TONE_H__
#define __TONE_H__

#define MAX_VOLUME 15
#define WAVETABLE_WIDTH 32
#define WAVETABLE_HEIGHT 15

class Tone {
    char phase;
    unsigned int phaseTimer;
    int noiseData = 511;
    char noiseOutput = 0;
    char index = 0;

    unsigned int pitch;
    unsigned char volume;
    bool noise;
    unsigned char wavetable[WAVETABLE_WIDTH];

    const PROGMEM byte precomputedVolumeTable[16][16] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2},
        {0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4},
        {0, 0, 1, 1, 2, 2, 2, 3, 3, 4, 4, 4, 5, 5, 6, 6},
        {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8},
        {0, 1, 1, 2, 3, 3, 4, 5, 5, 6, 7, 7, 8, 9, 9, 10},
        {0, 1, 2, 2, 3, 4, 5, 6, 6, 7, 8, 9, 10, 10, 11, 12},
        {0, 1, 2, 3, 4, 5, 6, 7, 7, 8, 9, 10, 11, 12, 13, 14},
        {0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 13, 14, 15, 16},
        {0, 1, 2, 4, 5, 6, 7, 8, 10, 11, 12, 13, 14, 16, 17, 18},
        {0, 1, 3, 4, 5, 7, 8, 9, 11, 12, 13, 15, 16, 17, 19, 20},
        {0, 1, 3, 4, 6, 7, 9, 10, 12, 13, 15, 16, 18, 19, 21, 22},
        {0, 2, 3, 5, 6, 8, 10, 11, 13, 14, 16, 18, 19, 21, 22, 24},
        {0, 2, 3, 5, 7, 9, 10, 12, 14, 16, 17, 19, 21, 23, 24, 26},
        {0, 2, 4, 6, 7, 9, 11, 13, 15, 17, 19, 21, 22, 24, 26, 28},
        {0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30}
    };

public:

    Tone() {
        pitch = 0;
        volume = 0;
        noise = 0;
        for(char i = 0; i < WAVETABLE_WIDTH; i++) {
            wavetable[i] = 0;
        }
        phase = 0;
        phaseTimer = 0;
    }

    char generateSample(){
        if (noise) return precomputedVolumeTable[volume][noiseOutput];
        return precomputedVolumeTable[volume][wavetable[phase]];
    }

    void advancePhase() {
        if (((phaseTimer & 0b0000011111111111) + pitch) > 0b0000011111111111){
            noiseOutput = (noiseData & 1) ? 15 : 0;

            char x = (noiseData & 0b01000000) >> 6;
            char y = noiseData & 1;
            char z = x ^ y;

            noiseData >>= 1;

            noiseData += z << 15;
        }

        phaseTimer += pitch;

        char index = ((phaseTimer & 0b1111100000000000) >> 11);
        phase = index;
    }

    void setPitchHigh(unsigned char newPitch) {
        pitch &= 0b0000000011111111;
        pitch += newPitch << 8;
    }

    void setPitchLow(unsigned char newPitch) {
        pitch &= 0b1111111100000000;
        pitch += newPitch;
    }

    void setVolume(char newVolume) {
        volume = newVolume;
        if(volume > MAX_VOLUME) {
            volume = MAX_VOLUME;
        }
    }

    void setWavetableSamples(unsigned char value) {

        wavetable[index<<1] = value & 0b00001111;
        wavetable[(index<<1)+1] = value & 0b11110000;

        ++index;

        index &= 0b00001111;
    }

    void setIndex(unsigned char value){
        index = value & 0b00001111;
    }

    void setNoise(bool newNoise) {
        noise = newNoise;
    }
};

#endif
