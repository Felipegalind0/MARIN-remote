#ifndef _SPEAKER_H
#define _SPEAKER_H

void Setup_Speaker();
void Play_Melody();
void StartUp_Sound( void * pvParameters );

void Click_DENIED_Sound( void * pvParameters );
void Click_Sound( void * pvParameters );
void Wireless_Sound( void * pvParameters );
void Shutdown_Sound();

#endif