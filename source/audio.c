#include "common.h"
#include "audio.h"

#define AUDSIZE 0x3a000

static AudioOutBuffer ao,
                  *aoPtr;

static u8 sAudio[AUDSIZE];

void initAudio() {
    audoutInitialize();
	audoutStartAudioOut();
}

/* 
    Reads in a signed 4-bit 24KHz mono PCM buffer, 
    expands it to 16-bit, interpolates it to 48KHz, 
    and repeats the samples into both channels for stereo.

    --------------------------------

    Why use 4-bit PCM?

    The scream is extremely loud, 
    thereby making 4-bit PCM's low SNR a non-issue, 
    saving us a ton of file space in the process.
*/
void loadAudio() {
    FILE *file = fopen("romfs:/scream", "rb");

    for (int i = 0; !feof(file); i += 0x10) {
        int c = fgetc(file);

        s16 samp1 = (c >> 4) * 0x1111,
            samp2 = (c & 15) * 0x1111;

        for (int j = 0; j < 4; j++)
            sAudio[i + j] = samp1;

        for (int j = 4; j < 8; j++)
            sAudio[i + j] = samp2;
    }

    fclose(file);
}

void playScream() {
    ao = (AudioOutBuffer) {
        .next        = NULL,
        .buffer      = sAudio,
        .buffer_size = AUDSIZE,
        .data_size   = AUDSIZE,
        .data_offset = 0
    };

	aoPtr = NULL;

	audoutPlayBuffer(&ao, &aoPtr);
}

void closeAudio() {
    audoutExit();
}