# The MAPLE sound "chip"

Salutations, visitor! The following github repository is dedicated to a wavetable-based sound generator meant for use in Atari 7800 cartridges, designed for the ATmega328p microcontroller.

Keep in mind that, despite its presence here, it is currently unfinished and not fit for use. However, the emulation of the sound is implemented within the codebase, and its specifications are as follows:

- 4 channels of wavetables, consisting of 32 4-bit samples each.
- 16-bit tuning
- 4-bit volume control
- Any of the channels can be made to output an XOR-based noise algorithm.

My friends and I are currently working on getting the chip to communicate with external devices before we get to attempting proper implementation on a cartridge.

Thank you for stopping by, and have a nice day!
