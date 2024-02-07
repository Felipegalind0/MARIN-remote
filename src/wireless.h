#ifndef _WIRELESS_H_
#define _WIRELESS_H_

#include <Vector.h>
#include <variables.h>

// Function declarations
void Wireless_Setup();
void recvMsg(uint8_t *data, size_t len);
void processCharArray();
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len);
Vector<Network> get_ssids();

#endif // _WIRELESS_H_
