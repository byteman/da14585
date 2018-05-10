#ifndef AUDIO_H
#define AUDIO_H

#include "types.h"
#include "sc5040b.h"

typedef enum {
	AUDIO_BLE_CONNECTED,
	AUDIO_BLE_DISCONNECTED,
	
}audio_message_t;
void  audio_isr(void);
void  audio_queue_digtal(const char* text);
void  audio_queue_message(audio_message_t msg);
#endif

