#ifndef INC_UART_H_
#define INC_UART_H_
#include <stdint.h>
#include <stdlib.h>

//// Inicjalizuje modul UART
void UART_Init(void);

// Obsluguje przychodzaca ramke danych o okreslonej dlugosci
// Parametry:
// len - dlugosc danych w ramce
// payload - wskaznik do danych w ramce
void callback(uint8_t len, uint8_t* payload);

// Wysyla ramke danych za posrednictwem UART
// Parametry:
// id - identyfikator ramki
// length - długosc danych w ramce
// payload - wskaznik do danych w ramce
void send_frame(uint8_t id, uint8_t length, uint8_t *payload);

// Odbiera ramke danych za posrednictwem UART
// Parametry:
// id - wskaznik do zmiennej, w ktorej zostanie zapisany identyfikator ramki
// length - wskaznik do zmiennej, w ktorej zostanie zapisana dlugosc danych w ramce
// payload - wskaznik do danych w ramce
void receive_frame(uint8_t *id, uint8_t *length, uint8_t *payload);


#endif /* INC_UART_H_ */
