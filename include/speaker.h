#ifndef _SPEAKER_H
#define _SPEAKER_H

extern int clickMelody[];

void Setup_Speaker();
void Play_Wireless_Melody(int *remoteMelody, int duration);
void Play_Melody(int *remoteMelody, int duration);
void StartUp_Sound( void * pvParameters );

void scheduleSoundTask(void (*task)(void * pvParameters));

void playClickSound();

void Click_DENIED_Sound( void * pvParameters );
void Click_Sound( void * pvParameters );
void Wireless_Sound( void * pvParameters );
void Shutdown_Sound();

#endif