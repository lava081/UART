#include "main.h"

#define INMP441_BUFFER_SIZE 8128

extern uint16_t inmp441_buffer[2][INMP441_BUFFER_SIZE / 2]; // INMP441数据缓冲区
extern char tx_inmp441_buffer[INMP441_BUFFER_SIZE * 2 / 8]; // INMP441发送缓冲区
extern volatile uint8_t inmp441_buffer_index; // INMP441缓冲区索引

void inmp441_init(void);
void tx_inmp441_send(uint8_t buffer_index);
