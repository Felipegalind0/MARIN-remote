#include <M5StickCPlus.h>
#include "IO.h"
#include "variables.h"

void Setup_Speaker(){
    pinMode(SPEAKER, OUTPUT);
    ledcSetup(SPEAKER_CH, 5000, 8);
    ledcAttachPin(SPEAKER, SPEAKER_CH);
}


int remoteMelody[]   = {1200, 1500, 1200, 1500, 1200}; // A sequence of higher-pitched tones
int startupMelody[]  = {1000, 1200, 1400, 1400, 1400}; // A sequence of startup tones
int shutdownMelody[] = {1400, 1200, 1000, 1000, 1000}; // A sequence of shutdown tones

int clickDENIEDMelody[]   = {2000, 1000}; // A sequence of higher-pitched tones

int clickMelody[]   = {2000}; // A sequence of higher-pitched tones

int WirelessMelody[]   = {2000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // A sequence of higher-pitched tones

void scheduleSoundTask(void (*task)(void * pvParameters)){
    xTaskCreatePinnedToCore(
        task,   /* Function to implement the task */
        "SoundTask", /* Name of the task */
        10000,  /* Stack size in words */
        NULL,  /* Task input parameter */
        -2,  /* Priority of the task */
        NULL,  /* Task handle. */
        BackgroundCore); /* Core where the task should run */
}
// Play_Melody now takes an additional parameter for the size of the melody array
// void Play_Melody(int *remoteMelody, int duration = 50) {
//     // //const int duration = 50; // Short duration for each tone

//     int size = sizeof(*remoteMelody) / sizeof(remoteMelody[0]); // Calculate the size of the array
//     Serial.println("Melody size: " + String(size));

//     //get the size of the array


//     for (int i = 0; i < size; i++) {
//         ledcWriteTone(SPEAKER_CH, remoteMelody[i]); // Access the melody note directly
//         vTaskDelay(duration / portTICK_PERIOD_MS);
//     }

//     ledcWriteTone(SPEAKER_CH, 0); // Turn off the sound
// }
template <size_t N>
void Play_Melody(int (&melody)[N], int duration = 50) {
    for (size_t i = 0; i < N; i++) {
        ledcWriteTone(SPEAKER_CH, melody[i]); // Access the melody note directly
        vTaskDelay(duration / portTICK_PERIOD_MS);
    }
    ledcWriteTone(SPEAKER_CH, 0); // Turn off the sound
}

// void Play_Wireless_Melody(int *remoteMelody, int duration = 50) {
//     //const int duration = 50; // Short duration for each tone

//     //int size = sizeof(remoteMelody) / sizeof(remoteMelody[0]); // BAD, this will return the size of a pointer, not the array
//     // instead dereference the pointer and divide by the size of the first element
//     int size = sizeof(*remoteMelody) / sizeof(remoteMelody[0]); // Calculate the size of the array

//     while (Pairing_State == SCANNING_SSIDS){
//         for (int i = 0; i < size && (Pairing_State == SCANNING_SSIDS); i++) {
//             ledcWriteTone(SPEAKER_CH, remoteMelody[i]); // Access the melody note directly
//             vTaskDelay(duration / portTICK_PERIOD_MS);
//             if (remoteMelody[i] == 0){
//                 RED_LED(0);
//             }
//             else{
//                 RED_LED(1);
//             }
//         }

//     }

    

//     ledcWriteTone(SPEAKER_CH, 0); // Turn off the sound
//     vTaskDelete(NULL);
//     return;

// }

template <size_t N>
void Play_Wireless_Melody(int (&melody)[N], int duration = 50) {
    while (Pairing_State == SCANNING_SSIDS) {
        for (size_t i = 0; i < N && (Pairing_State == SCANNING_SSIDS); i++) {
            ledcWriteTone(SPEAKER_CH, melody[i]); // Access the melody note directly
            vTaskDelay(duration / portTICK_PERIOD_MS);
            if (melody[i] == 0) {
                RED_LED(0);
            } else {
                RED_LED(1);
            }
        }
    }
    ledcWriteTone(SPEAKER_CH, 0); // Turn off the sound
    vTaskDelete(NULL);
}




#define NUM_SEGMENTS 3  // Number of segments in the startup melody
#define SEGMENT_DURATION 500  // Duration of each segment in milliseconds
#define FREQUENCY_SWITCH_INTERVAL 10

void playClickSound(){
    Play_Melody(clickMelody, 15);
}


void Click_DENIED_Sound( void * pvParameters ){

    //int sizeOfClickMelody = sizeof(clickDENIEDMelody) / sizeof(clickMelody[0]); // Calculate the size of the array
    Play_Melody(clickDENIEDMelody); // Pass the melody array and its size

    vTaskDelete(NULL);

    return;
}

void Click_Sound( void * pvParameters ){

    //int sizeOfClickMelody = sizeof(clickMelody) / sizeof(clickMelody[0]); // Calculate the size of the array
    Play_Melody(clickMelody, 15); // Pass the melody array and its size

    vTaskDelete(NULL);

    return;
}

void Wireless_Sound( void * pvParameters ){

    //int sizeOfWirelessMelody = sizeof(WirelessMelody) / sizeof(WirelessMelody[0]); // Calculate the size of the array
    Play_Wireless_Melody(WirelessMelody, 15); // Pass the melody array and its size

    vTaskDelete(NULL);

    return;
}



void StartUp_Sound( void * pvParameters ){

    Play_Melody(startupMelody, 200); // Pass the melody array and its size

    vTaskDelete(NULL);

    return;
}

void Shutdown_Sound(){

    Play_Melody(remoteMelody); // Pass the melody array and its size

    vTaskDelay(200);

    Play_Melody(shutdownMelody, 200); // Pass the melody array and its size


}