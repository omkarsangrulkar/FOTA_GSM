/*
 * at_commands.c
 *
 *  Created on: Sep 15, 2023
 *      Author: sghute
 */

#include "at_commands.h"
#include "M66.h"
#include <string.h>
#include <stdio.h>
#include "stdbool.h"
#include "stm32f4xx_hal_uart.h"
// Any other necessary includes, e.g., for UART or other utilities

at_command_state_t at_state = AT_IDLE;
static const char* expected_at_response = NULL;
static uint32_t at_command_start_time = 0;

// Assuming extern definitions



// Function to send AT command to Quectel M66
void send_at_command(const char* command, const char* expected_resp) {
	if (at_state == AT_IDLE || AT_RESPONSE_RECEIVED) {
	        // Clear UART buffer
	        memset(uart_buffer.data, 0, UART_BUFFER_SIZE);

	        // Send the AT command
	        UART_Send(command);
	        UART_Send("\r\n");

	        // Set the state to waiting for a response
	        at_state = AT_WAITING_RESPONSE;
	        expected_at_response = expected_resp;

	        // Capture the current time (however you measure time in your system)
	        at_command_start_time = get_system_time();  // This function needs to be implemented based on your system
	    }

}
at_command_state_t check_at_command_response(uint16_t timeout) {
    if (at_state == AT_WAITING_RESPONSE) {
        if (data_received_flag) {
            if (strstr((char*)receive_buffer, expected_at_response) != NULL) {
                at_state = AT_RESPONSE_RECEIVED;

            }

            // Reset the flag after processing
            data_received_flag = false;
        } else {
            uint32_t current_time = get_system_time();
            if ((current_time - at_command_start_time) >= timeout) {
                at_state = AT_RESPONSE_TIMEOUT;
            }
        }
    }
    return at_state;
}

bool command_acknowledged = true;  // This flag is true when the last sent command was acknowledged

void send_at_command_blocking(const char* command, const char* expected_resp, uint16_t timeout) {
    // Reset the acknowledged flag
    command_acknowledged = false;

    // Send the AT command
    send_at_command(command, expected_resp);

    uint32_t start_time = get_system_time();

    // Block until the command is acknowledged (either responded or timed out)
    while (!command_acknowledged) {
        check_at_command_response(timeout);

        uint32_t current_time = get_system_time();
        if ((current_time - start_time) >= timeout) {
            break;
        }
    }
}

//at_command_state_t check_at_command_response(uint16_t timeout) {
//    if (at_state == AT_WAITING_RESPONSE) {
//        if (data_received_flag) {
//            if (strstr((char*)receive_buffer, expected_at_response) != NULL) {
//                at_state = AT_RESPONSE_RECEIVED;
//                command_acknowledged = true;  // Set the acknowledged flag when the response is received
//            }
//
//            // Reset the flag after processing
//            data_received_flag = false;
//        } else {
//            uint32_t current_time = get_system_time();
//            if ((current_time - at_command_start_time) >= timeout) {
//                at_state = AT_RESPONSE_TIMEOUT;
//                command_acknowledged = true;  // Set the acknowledged flag when the response is timed out
//            }
//        }
//    }
//    return at_state;
//}

void UART_Send(const char* str) {
    HAL_UART_Transmit(&huart2, (uint8_t *)str, strlen(str), 1000);  // Transmitting via UART2 in this example
}

uint32_t get_system_time(void) {
    return HAL_GetTick();  // Returns the system tick count since start-up
}
