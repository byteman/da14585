#ifndef AUDIO_H
#define AUDIO_H

#include "types.h"


typedef enum {
	AUDIO_BLE_CONNECTED,
	AUDIO_BLE_DISCONNECTED,
	
}audio_message_t;

typedef uint8 (*audio_play_func)(char label);
typedef uint8 (*audio_init_func)(void);
typedef uint8 (*audio_busy_func)(void);

typedef struct audio_soc{
	audio_init_func init;
	audio_play_func play;
	audio_busy_func busy;
}audio_soc_t;
void  audio_queue_init(audio_soc_t *soc);
void  audio_isr(void);
void  audio_queue_message(const char* text);
#endif

