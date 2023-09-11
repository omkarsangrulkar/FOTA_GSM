/*
 * M66.h
 *
 *  Created on: Aug 30, 2023
 *      Author: atiqueshaikh
 */

#ifndef INC_M66_H_
#define INC_M66_H_

#include "stm32f4xx_hal.h"
#include "main.h"
#include "stdbool.h"

extern UART_HandleTypeDef huart2;

// Addresses for the bootloader, config, app, and temp sections
#define BOOTLOADER_START_ADDR   0x08000000  // Start address of the bootloader section
#define CONFIG_START_ADDR       0x08020000  // Start address of the config section
#define APP_START_ADDR          0x08030000  // Start address of the app section
#define TEMP_START_ADDR         0x08100000  // Start address of the temp section


void send_at_command(const char* command);
size_t receive_data(uint8_t* buffer, uint16_t length);
uint32_t calculate_crc32(uint8_t* data, size_t length);
bool verify_firmware_update(uint8_t* firmware_data, size_t firmware_length);
bool download_firmware(const char* firmware_url);
void handle_mqtt_message(const char* message);
void Initialize_Modem(void);
void SSL_Config(void);
void AWS_MQTT(void);





#endif /* INC_M66_H_ */
