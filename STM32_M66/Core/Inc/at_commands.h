/*
 * at_commands.h
 *
 *  Created on: Sep 15, 2023
 *      Author: sghute
 */

#ifndef INC_AT_COMMANDS_H_
#define INC_AT_COMMANDS_H_

#include "stm32f4xx_hal.h"
#include "main.h"
#include "stdbool.h"

typedef enum {
    AT_IDLE,
    AT_WAITING_RESPONSE,
    AT_RESPONSE_RECEIVED,
    AT_RESPONSE_TIMEOUT
} at_command_state_t;
extern at_command_state_t at_state;

// Function prototypes
void send_at_command(const char* command, const char* expected_resp);
at_command_state_t check_at_command_response(uint16_t timeout);
void UART_Send(const char* str);
uint32_t get_system_time(void);


#endif /* INC_AT_COMMANDS_H_ */
