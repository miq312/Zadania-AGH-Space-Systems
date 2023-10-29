#ifndef INC_UART_H_
#define INC_UART_H_
#include <stdint.h>
#include <stdlib.h>


void UART_Init(void);
void callback(uint8_t len, uint8_t* payload);
void send_frame(uint8_t id, uint8_t length, uint8_t *payload);
void receive_frame(uint8_t *id, uint8_t *length, uint8_t *payload);


#endif /* INC_UART_H_ */
