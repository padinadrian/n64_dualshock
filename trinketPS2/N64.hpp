/**
 * File:    n64_dualshock.ino
 * Author:  padin.adrian@gmail.com
 *
 * Copyright 2019
 */

/* ===== Includes ====== */
#include <stdint.h>

/* ===== Functions ===== */

// Initialize digital pins
void SingleWireInit();

// Write up to 255 bytes
void SingleWireWrite(uint8_t *buf, uint8_t bufsize);

// Read a command from the N64 console.
// Returns the number of bytes read.
// (For now only supports reading a single byte)
uint8_t SingleWireRead(uint8_t *buf, const uint8_t bufsize);

