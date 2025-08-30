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

    const PROGMEM char precomputedVolumeTable[16][16] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4},
        {0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8},
        {0, 1, 2, 2, 3, 4, 5, 6, 6, 7, 8, 9, 10, 10, 11, 12},
        {0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 13, 14, 15, 16},
        {0, 1, 3, 4, 5, 7, 8, 9, 11, 12, 13, 15, 16, 17, 19, 20},
        {0, 2, 3, 5, 6, 8, 10, 11, 13, 14, 16, 18, 19, 21, 22, 24},
        {0, 2, 4, 6, 7, 9, 11, 13, 15, 17, 19, 21, 22, 24, 26, 28},
        {0, 2, 4, 6, 9, 11, 13, 15, 17, 19, 21, 23, 26, 28, 30, 32},
        {0, 2, 5, 7, 10, 12, 14, 17, 19, 22, 24, 26, 29, 31, 34, 36},
        {0, 3, 5, 8, 11, 13, 16, 19, 21, 24, 27, 29, 32, 35, 37, 40},
        {0, 3, 6, 9, 12, 15, 18, 21, 23, 26, 29, 32, 35, 38, 41, 44},
        {0, 3, 6, 10, 13, 16, 19, 22, 26, 29, 32, 35, 38, 42, 45, 48},
        {0, 3, 7, 10, 14, 17, 21, 24, 28, 31, 35, 38, 42, 45, 49, 52},
        {0, 4, 7, 11, 15, 19, 22, 26, 30, 34, 37, 41, 45, 49, 52, 56},
        {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60},
    };

public:
    unsigned int pitch;
    char volume;
    bool noise;
    char wavetable[WAVETABLE_WIDTH];

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

    void advancePhase(long cycles) {
        while(cycles > 0) {

            if (((phaseTimer & 0b0000011111111111) + pitch) > 0b0000011111111111){
                noiseOutput = (noiseData & 1) ? 15 : 0;

                char x = (noiseData & 0b01000000) >> 6;
                char y = noiseData & 1;
                char z = x ^ y;

                noiseData >>= 1;

                noiseData += z << 15;
            }

            phaseTimer += pitch;
            --cycles;

            char index = ((phaseTimer & 0b1111100000000000) >> 11);
            phase = index;
        }
    }

    void setPitch(unsigned int newPitch) {
        pitch = newPitch;
    }

    void setVolume(char newVolume) {
        volume = newVolume;
        if(volume > MAX_VOLUME) {
            volume = MAX_VOLUME;
        }
    }

    void setWavetable(char* newWavetable) {

        int i = WAVETABLE_WIDTH;

        while (--i >= 0){
            wavetable[i] = newWavetable[i];
            if(wavetable[i] > WAVETABLE_HEIGHT) {
                wavetable[i] = WAVETABLE_HEIGHT;
            }
        }
    }

    void setNoise(bool newNoise) {
        noise = newNoise;
    }
};

#endif