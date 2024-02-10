#include <M5StickCPlus.h>
#include "IO.h"

void Setup_Speaker(){
    pinMode(SPEAKER, OUTPUT);
    ledcSetup(SPEAKER_CH, 5000, 8);
    ledcAttachPin(SPEAKER, SPEAKER_CH);
}

// void Remote_Sound(){
//     ledcWriteTone(SPEAKER_CH,2000);
//     vTaskDelay(10);
//     ledcWriteTone(SPEAKER_CH,0);
//     vTaskDelay(10);
//     ledcWriteTone(SPEAKER_CH,2000);
//     vTaskDelay(10);
//     ledcWriteTone(SPEAKER_CH,0);
//     vTaskDelay(10);
//     ledcWriteTone(SPEAKER_CH,2000);
//     vTaskDelay(10);
//     ledcWriteTone(SPEAKER_CH,0);
//     vTaskDelay(10);
//     ledcWriteTone(SPEAKER_CH,2000);
//     vTaskDelay(10);
//     ledcWriteTone(SPEAKER_CH,0);
//     vTaskDelay(10);
//     ledcWriteTone(SPEAKER_CH,2000);
//     vTaskDelay(10);
//     ledcWriteTone(SPEAKER_CH,0);
//     vTaskDelay(10);
//     ledcWriteTone(SPEAKER_CH,2000);
//     vTaskDelay(10);
//     ledcWriteTone(SPEAKER_CH,0);
//     vTaskDelay(10);
//     ledcWriteTone(SPEAKER_CH,2000);
//     vTaskDelay(10);
//     ledcWriteTone(SPEAKER_CH,0);
//     vTaskDelay(10);
//     ledcWriteTone(SPEAKER_CH,2000);
//     vTaskDelay(10);
//     ledcWriteTone(SPEAKER_CH,0);
//     vTaskDelay(10);
//     ledcWriteTone(SPEAKER_CH,2000);
//     vTaskDelay(10);
//     ledcWriteTone(SPEAKER_CH,0);
//     vTaskDelay(10);
//     ledcWriteTone(SPEAKER_CH,2000);
//     vTaskDelay(10);
//     ledcWriteTone(SPEAKER_CH,0);
//     vTaskDelay(10);
//     ledcWriteTone(SPEAKER_CH,2000);
//     vTaskDelay(10);
//     ledcWriteTone(SPEAKER_CH,0);
//     vTaskDelay(10);
//     ledcWriteTone(SPEAKER_CH,2000);
//     vTaskDelay(10);
//     ledcWriteTone(SPEAKER_CH,0);
//     vTaskDelay(10);
// }

// void Remote_Sound() {
//     const int note1 = 440; // A4
//     const int note2 = 523; // C5, forming a minor third interval with A4
//     const int duration = 50; // Short duration for a chirp-like sound

//     for (int i = 0; i < 3; i++) { // Three chirps to signify a command being sent
//         ledcWriteTone(SPEAKER_CH, note1);
//         vTaskDelay(duration / portTICK_PERIOD_MS);
//         ledcWriteTone(SPEAKER_CH, note2);
//         vTaskDelay(duration / portTICK_PERIOD_MS);
//     }

//     ledcWriteTone(SPEAKER_CH, 0); // Turn off the sound
// }
void Remote_Sound() {
    const int remoteMelody[] = {1200, 1500, 1200, 1500, 1200}; // A sequence of higher-pitched tones
    const int duration = 50; // Short duration for each tone

    for (int i = 0; i < sizeof(remoteMelody) / sizeof(remoteMelody[0]); i++) {
        ledcWriteTone(SPEAKER_CH, remoteMelody[i]);
        vTaskDelay(duration / portTICK_PERIOD_MS);
    }

    ledcWriteTone(SPEAKER_CH, 0); // Turn off the sound
}


// void StartUp_Sound( void * pvParameters ){

//     Remote_Sound();
    
//     ledcWriteTone(SPEAKER_CH,1000);
//     vTaskDelay(200);
//     ledcWriteTone(SPEAKER_CH,1200);
//     vTaskDelay(200);
//     ledcWriteTone(SPEAKER_CH,1400);
//     vTaskDelay(600);
//     ledcWriteTone(SPEAKER_CH,0);

//     vTaskDelete(NULL);

//     return;
// }

// void StartUp_Sound(void *pvParameters) {
//     //const int startupMelody[] = {1046, 1175, 1318, 1397, 1568, 1760, 1976, 2093}; // C6, D6, E6, F6, G6, A6, B6, C7
//     const int startupMelody[] = {
//     660, 670, 660, 670, 670, 220, 670, 220, 670, 670,
//     670, 670, 1060, 1060, 1060, 1780, 800, 800, 800, 800,
//     7130, 7130, 5610, 5600, 5600, 3560, 3560, 800, 800, 790,
//     260, 260, 7120, 7120, 5610, 5610, 6400, 260, 260, 260,
//     140, 160, 100, 260, 260, 260, 260, 140, 254
//     };

//     const int noteDurations = 100; // Duration for each note in milliseconds

//     //Remote_Sound(); // Play a chirp-like sound to signify the start of the REMOTE startup sound

//     vTaskDelay(500 / portTICK_PERIOD_MS); // Delay for 0.5 second before playing the startup melody

//     for (int thisNote = 0; thisNote < 8; thisNote++) {
//         ledcWriteTone(SPEAKER_CH, startupMelody[thisNote]);
//         vTaskDelay(noteDurations / portTICK_PERIOD_MS);
//     }

//     ledcWriteTone(SPEAKER_CH, 0); // Turn off the sound
//     vTaskDelete(NULL);
// }


#define NUM_SEGMENTS 3  // Number of segments in the startup melody
#define SEGMENT_DURATION 500  // Duration of each segment in milliseconds
#define FREQUENCY_SWITCH_INTERVAL 10
  // Time in milliseconds to switch between frequencies within a segment

// An array of arrays, where each sub-array is a segment of frequencies
// const int startupMelody[NUM_SEGMENTS][10] = {
//     {500, 1000, 1500, 2000, 3000, 4000, 5000, 7500, 8000, 9000},  // Frequencies for segment 1
//     {600, 1800, 3000, 3600, 5040, 6000, 7100, 8300, 9000, 9500}, // Frequencies for segment 2
//     {800, 1600, 2400, 3200, 4000, 5000, 6500, 7200, 8000, 9600} // Frequencies for segment 3
// };

// const int startupMelody[NUM_SEGMENTS][8] = {
//     {1500, 2000, 3000, 4000, 5000, 7500, 8000, 9000},  // Frequencies for segment 1
//     {3000, 3600, 5040, 6000, 7100, 8300, 9000, 9500}, // Frequencies for segment 2
//     {2400, 3200, 4000, 5000, 6500, 7200, 8000, 9600} // Frequencies for segment 3
// };

// const int startupMelody[NUM_SEGMENTS][1] = {
//     {1100},  // Frequencies for segment 1
//     {3500}, // Frequencies for segment 2
//     {7200} // Frequencies for segment 3
// };


// void StartUp_Sound(void *pvParameters) {
//     for (int segment = 0; segment < NUM_SEGMENTS; segment++) {
//         long segmentStartTime = millis();
//         while (millis() - segmentStartTime < SEGMENT_DURATION) {
//             // Cycle through each frequency within the segment
//             for (int freqIndex = 0; freqIndex < 4; freqIndex++) {
//                 ledcWriteTone(SPEAKER_CH, startupMelody[segment][freqIndex]);
//                 // Wait for the frequency switch interval
//                 vTaskDelay(FREQUENCY_SWITCH_INTERVAL / portTICK_PERIOD_MS);
//             }
//         }
//     }

//     // Turn off the sound after the melody is done
//     ledcWriteTone(SPEAKER_CH, 0);
//     vTaskDelete(NULL); // Delete the task
// }

void StartUp_Sound( void * pvParameters ){

    Remote_Sound();

    vTaskDelay(200);
    
    ledcWriteTone(SPEAKER_CH,1000);
    vTaskDelay(200);
    ledcWriteTone(SPEAKER_CH,1200);
    vTaskDelay(200);
    ledcWriteTone(SPEAKER_CH,1400);
    vTaskDelay(600);
    ledcWriteTone(SPEAKER_CH,0);

    vTaskDelete(NULL);

    return;
}

void Shutdown_Sound(){

    Remote_Sound();

    vTaskDelay(200);

    ledcWriteTone(SPEAKER_CH,1400);
    vTaskDelay(200);
    ledcWriteTone(SPEAKER_CH,1200);
    vTaskDelay(200);
    ledcWriteTone(SPEAKER_CH,1000);
    vTaskDelay(400);
    ledcWriteTone(SPEAKER_CH,0);
}