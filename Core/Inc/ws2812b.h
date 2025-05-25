/*
 * ws2812b.h
 *
 *  Anpassung für minimalistisches STM32-Projekt mit SK6812/WS2812-LEDs
 *  Author: Copilot angepasst für Willibot
 */

#ifndef INC_WS2812B_H_
#define INC_WS2812B_H_

#include "main.h"
#include <stdbool.h>
#include <stddef.h>

/* === Anpassung: Anzahl deiner LEDs === */
#define PIXELS_COUNT 12   // <<-- HIER Anzahl deiner LEDs eintragen!

/* === Interne Defines, meist nicht ändern === */
#define BITS_IN_PIXEL 24
#define TIME_TO_RST 50 // > 50us Reset-Zeit
#define LEDS_BUFFER_SIZE ((PIXELS_COUNT * BITS_IN_PIXEL) + TIME_TO_RST)

/* === Struktur für RGB-Farben === */
typedef struct {
    uint8_t Red;
    uint8_t Green;
    uint8_t Blue;
} RGB;

/* === Hauptfunktionen (minimal) === */

/**
 * Initialisiert die Library.
 * htim: Zeiger auf deinen Timer-Handle (z.B. &htim1)
 * TIMx: Hardware-Timer (z.B. TIM1)
 */
void Ws2812b_Init(TIM_HandleTypeDef *htim, TIM_TypeDef *TIMx);

/**
 * Setzt eine einzelne LED auf eine RGB-Farbe.
 * color: Zeiger auf RGB-Struct
 * pixel: Index der LED (0 ... PIXELS_COUNT-1)
 */
void Ws2812b_SetPixel_RGB(const RGB *color, uint16_t pixel);

/**
 * Setzt alle LEDs auf eine RGB-Farbe.
 * color: Zeiger auf RGB-Struct
 */
void Ws2812b_SetStrip_RGB(const RGB *color);

/**
 * Überträgt die aktuellen Farbwerte an den Stripe (ohne Delay).
 */
void Ws2812b_Show_without_Delay(void);

#endif /* INC_WS2812B_H_ */