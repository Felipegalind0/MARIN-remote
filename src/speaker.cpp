#include <M5StickCPlus.h>
#include "IO.h"

void Setup_Speaker(){
    pinMode(SPEAKER, OUTPUT);
    ledcSetup(SPEAKER_CH, 5000, 8);
    ledcAttachPin(SPEAKER, SPEAKER_CH);
}


int remoteMelody[]   = {1200, 1500, 1200, 1500, 1200}; // A sequence of higher-pitched tones
int startupMelody[]  = {1000, 1200, 1400, 1400, 1400}; // A sequence of startup tones
int shutdownMelody[] = {1400, 1200, 1000, 1000, 1000}; // A sequence of shutdown tones


// Play_Melody now takes an additional parameter for the size of the melody array
void Play_Melody(int *remoteMelody, int size, int duration = 50) {
    //const int duration = 50; // Short duration for each tone

    for (int i = 0; i < size; i++) {
        ledcWriteTone(SPEAKER_CH, remoteMelody[i]); // Access the melody note directly
        vTaskDelay(duration / portTICK_PERIOD_MS);
    }

    ledcWriteTone(SPEAKER_CH, 0); // Turn off the sound
}




#define NUM_SEGMENTS 3  // Number of segments in the startup melody
#define SEGMENT_DURATION 500  // Duration of each segment in milliseconds
#define FREQUENCY_SWITCH_INTERVAL 10


void StartUp_Sound( void * pvParameters ){

    //Play_Melody(remoteMelody);
    int sizeOfRemoteMelody = sizeof(remoteMelody) / sizeof(remoteMelody[0]); // Calculate the size of the array
    Play_Melody(remoteMelody, sizeOfRemoteMelody); // Pass the melody array and its size


    vTaskDelay(200);

    int sizeOfStartupMelody = sizeof(startupMelody) / sizeof(startupMelody[0]); // Calculate the size of the array
    Play_Melody(startupMelody, sizeOfStartupMelody, 200); // Pass the melody array and its size

    vTaskDelete(NULL);

    return;
}

void Shutdown_Sound(){

    // Play_Melody(remoteMelody);
    int sizeOfRemoteMelody = sizeof(remoteMelody) / sizeof(remoteMelody[0]); // Calculate the size of the array
    Play_Melody(remoteMelody, sizeOfRemoteMelody); // Pass the melody array and its size


    vTaskDelay(200);

    int sizeOfShutdownMelody = sizeof(shutdownMelody) / sizeof(shutdownMelody[0]); // Calculate the size of the array
    Play_Melody(shutdownMelody, sizeOfShutdownMelody, 200); // Pass the melody array and its size


}