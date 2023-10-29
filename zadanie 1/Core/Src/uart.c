#include "uart.h"
#include "main.h"

void motor_set_mode(uint8_t len, uint8_t* payload);
void motor_set_speed(uint8_t len, uint8_t* payload);
void motor_set_pos(uint8_t len, uint8_t* payload);

UART_HandleTypeDef huart2;

void UART_Init(void)
{
	huart2.Instance = USART2; 						// Ustawienie instancji UART na USART2.
	huart2.Init.BaudRate = 115200; 					// Predkosc transmisji na 115200.
	huart2.Init.WordLength = UART_WORDLENGTH_8B;	// Dlugosc slowa na 8 bitow
	huart2.Init.StopBits = UART_STOPBITS_1;			// Ilosc bitow stopu na 1
	huart2.Init.Parity = UART_PARITY_NONE;			// Brak parzystosci
	huart2.Init.Mode = UART_MODE_TX_RX;				// Tryb UART transmit i receive
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;	// Wylaczenie kontroli przeplywu sprzetowego
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;// Ustawienie oversampling na 16
	HAL_UART_Init(&huart2);							// Inicjalizacja UART z ustawieniami konfiguracji
}

void send_frame(uint8_t id, uint8_t length, uint8_t *payload)
{
  uint8_t *frame;  							// Wskaznik do pola danych
  frame = (uint8_t *)malloc(length + 3); 	// Alokuje pamiec na pole danych
  if (frame == NULL) {
	  	  	  	  	  	  	  	  	  	    // Obsluga bledu alokacji pamieci
    Error_Handler();
  }

  frame[0] = 0x3C;  					// START
  frame[1] = id;    					// ID
  frame[2] = length;  					// LEN
  for (int i = 0; i < length; i++) {
    frame[3 + i] = payload[i];  		// PAYLOAD
  }

  HAL_UART_Transmit(&huart2, frame, length + 3, HAL_MAX_DELAY);

  free(frame); 							// Zwolnienie zaalokowanej pamieci po przeslaniu ramki
}

void receive_frame(uint8_t *id, uint8_t *length, uint8_t *payload)
{
  uint8_t frame[256]; 									// Bufor na odebrana ramke
  HAL_StatusTypeDef status;
  status = HAL_UART_Receive(&huart2, frame, 3, 10000);	// Odczyt danych START, ID i LEN z timeoutem
  if (status != HAL_OK) {
	  	  	  	  	  	  	  	  	  	  	  	  	    // Obsluga timeoutu
    Error_Handler();
  }

  *id = frame[1];
  *length = frame[2];

  status = HAL_UART_Receive(&huart2, payload, *length, 10000); // Odczyt PAYLOAD z timeoutem
  if (status != HAL_OK) {
	  	  	  	  	  	  	  	  	  	  	  	  	  	  	   // Obsluga timeoutu
	  Error_Handler();
  }

  callback(*length, payload);							// Wywolanie funkcji zwrotnej (callback) po otrzymaniu ramki
}

void callback(uint8_t len, uint8_t* payload)
{
	uint8_t id = payload[0];

	switch (id) {
	    case 0x11:
	        motor_set_mode(len - 1, payload + 1);
	        break;
	    case 0x12:
	        motor_set_speed(len - 1, payload + 1);
	        break;
	    case 0x13:
	        motor_set_pos(len - 1, payload + 1);
	        break;
	    default:
	        break;
	    }
}
