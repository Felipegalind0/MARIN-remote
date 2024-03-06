So I modified Play_Melody to use a ptr to the melody array, but now the melody does not play. Please help me fix this.


// void Play_Melody() {
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

int remoteMelody[] = {1200, 1500, 1200, 1500, 1200}; // A sequence of higher-pitched tones
void Play_Melody(int * remoteMelody) {
    
    const int duration = 50; // Short duration for each tone

    // for (int i = 0; i < sizeof(*remoteMelody) / sizeof(*remoteMelody[0]); i++) {
    for (int i = 0; i < sizeof(&remoteMelody) / sizeof(remoteMelody); i++) {
        ledcWriteTone(SPEAKER_CH, *(remoteMelody));
        vTaskDelay(duration / portTICK_PERIOD_MS);

        remoteMelody++;
    }

    ledcWriteTone(SPEAKER_CH, 0); // Turn off the sound
}


void StartUp_Sound( void * pvParameters ){

    Play_Melody(remoteMelody);

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
