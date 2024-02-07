#include <M5StickCPlus.h>
#include "IO.h"

void Remote_Sound(){
    ledcWriteTone(SPEAKER_CH,2000);
    vTaskDelay(10);
    ledcWriteTone(SPEAKER_CH,0);
    vTaskDelay(10);
    ledcWriteTone(SPEAKER_CH,2000);
    vTaskDelay(10);
    ledcWriteTone(SPEAKER_CH,0);
    vTaskDelay(10);
    ledcWriteTone(SPEAKER_CH,2000);
    vTaskDelay(10);
    ledcWriteTone(SPEAKER_CH,0);
    vTaskDelay(10);
    ledcWriteTone(SPEAKER_CH,2000);
    vTaskDelay(10);
    ledcWriteTone(SPEAKER_CH,0);
    vTaskDelay(10);
    ledcWriteTone(SPEAKER_CH,2000);
    vTaskDelay(10);
    ledcWriteTone(SPEAKER_CH,0);
    vTaskDelay(10);
    ledcWriteTone(SPEAKER_CH,2000);
    vTaskDelay(10);
    ledcWriteTone(SPEAKER_CH,0);
    vTaskDelay(10);
    ledcWriteTone(SPEAKER_CH,2000);
    vTaskDelay(10);
    ledcWriteTone(SPEAKER_CH,0);
    vTaskDelay(10);
    ledcWriteTone(SPEAKER_CH,2000);
    vTaskDelay(10);
    ledcWriteTone(SPEAKER_CH,0);
    vTaskDelay(10);
    ledcWriteTone(SPEAKER_CH,2000);
    vTaskDelay(10);
    ledcWriteTone(SPEAKER_CH,0);
    vTaskDelay(10);
    ledcWriteTone(SPEAKER_CH,2000);
    vTaskDelay(10);
    ledcWriteTone(SPEAKER_CH,0);
    vTaskDelay(10);
    ledcWriteTone(SPEAKER_CH,2000);
    vTaskDelay(10);
    ledcWriteTone(SPEAKER_CH,0);
    vTaskDelay(10);
    ledcWriteTone(SPEAKER_CH,2000);
    vTaskDelay(10);
    ledcWriteTone(SPEAKER_CH,0);
    vTaskDelay(10);
}

void StartUp_Sound(){

    Remote_Sound();
    
    ledcWriteTone(SPEAKER_CH,1000);
    vTaskDelay(200);
    ledcWriteTone(SPEAKER_CH,1200);
    vTaskDelay(200);
    ledcWriteTone(SPEAKER_CH,1400);
    vTaskDelay(600);
    ledcWriteTone(SPEAKER_CH,0);
}
void Shutdown_Sound(){
    ledcWriteTone(SPEAKER_CH,1400);
    vTaskDelay(200);
    ledcWriteTone(SPEAKER_CH,1200);
    vTaskDelay(200);
    ledcWriteTone(SPEAKER_CH,1000);
    vTaskDelay(400);
    ledcWriteTone(SPEAKER_CH,0);
}