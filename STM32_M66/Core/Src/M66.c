/*
 * M66.c
 *
 *  Created on: Aug 30, 2023
 *      Author: atiqueshaikh
 */

#include "M66.h"
#include "stm32f4xx_hal.h"
#include "main.h"
#include <string.h>
#include <stdio.h>
#include "stdbool.h"
#include "stm32f4xx_hal_uart.h"
#include "cJSON.h"
#include <stdint.h>
#include "stm32f4xx_hal_flash.h"

char APN[] = "airtelgprs.com";

#define MAX_FIRMWARE_SIZE 4096 // Adjust as needed

// Firmware buffer
uint8_t firmware_buffer[MAX_FIRMWARE_SIZE];
size_t firmware_size = 0;

#define CRC32_POLYNOMIAL 0xEDB88320u

void Delay(int delay){
	for (int i = 0; i < delay; i++);
}

void Initialize_Modem(void)
{
//	char rxData[100];
//	 do {
//		 HAL_UART_Transmit(&huart2, (uint8_t *)"AT\r\n", strlen("AT\r\n"), 300);
//		 HAL_Delay(300);
//		 HAL_UART_Receive(&huart2, (uint8_t *)rxData, sizeof(100), 300);
//	 }
//	 while(strstr(rxData, "OK") == NULL);
//	 do {
//		 HAL_UART_Transmit(&huart2, (uint8_t *)"AT+CPIN?\r\n", strlen("AT+CPIN?\r\n"), 300);
//		 HAL_Delay(300);
//		 HAL_UART_Receive(&huart2, (uint8_t *)rxData, sizeof(100), 300);
//	 }
//	 while(strstr(rxData, "+CPIN: READY") == NULL);
//	 HAL_Delay(300);
	HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QMTDISC=0\r\n", strlen("AT+QMTDISC=0\r\n"), 1000);
	HAL_Delay(1000);
	HAL_UART_Transmit(&huart2, (uint8_t *)"AT+CPIN?\r\n", strlen("AT+CPIN?\r\n"), 1000);
	HAL_Delay(3000);
	HAL_UART_Transmit(&huart2, (uint8_t *)"AT+CREG?\r\n", strlen("AT+CREG?\r\n"), 1000);
	HAL_Delay(1000);
	HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QICSGP=1,\"airtelgprs.com\"\r\n", strlen("AT+QICSGP=1,\"airtelgprs.com\"\r\n"), 1000);
//	HAL_UART_Transmit(&huart2, (uint8_t *)',"airtelgprs.com"\r\n', strlen(',"airtelgprs.com"\r\n'), HAL_MAX_DELAY);
	HAL_Delay(1000);
	HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QICSGP?\r\n", strlen("AT+QICSGP?\r\n"), 1000);
	HAL_Delay(1000);
	HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QIREGAPP\r\n", strlen("AT+QIREGAPP\r\n"), 1000);
	HAL_Delay(1000);
	HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QIACT\r\n", strlen("AT+QIACT\r\n"), 1000);
	HAL_Delay(1000);
	HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QILOCIP\r\n", strlen("AT+QILOCIP\r\n"), 1000);
	HAL_Delay(1000);

}
//Have to work on certificate configuration.
void SSL_Config(void){
	char CA_CERT[] =  "-----BEGIN CERTIFICATE-----\n\
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n\
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n\
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n\
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n\
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n\
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n\
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n\
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n\
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n\
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n\
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n\
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n\
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n\
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n\
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n\
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n\
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n\
rqXRfboQnoZsG4q5WTP468SQvvG5\n\
-----END CERTIFICATE-----\r\n";

	char Cli_CERT[] = "-----BEGIN CERTIFICATE-----\n\
MIIDWjCCAkKgAwIBAgIVAJucOsrmj0GkIimc5ToFo/rcZCIjMA0GCSqGSIb3DQEB\n\
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n\
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMzA4MjkwODM1\n\
MzlaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n\
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDHgFxLbb8kBihIeEUP\n\
p/AKYeyHtFq7NPtpVDRA/5GfybsOaASPFjeE/sVr/II4+XMAQCdOxnlJ1gdtEkjX\n\
j8n0nuNvIfX74QXX5UYVKRjHXuS7sktr5D0pRDcCBg1agPSZp/ohTD3Nr499Yc3b\n\
SdLFA6LFGjFjdNS8EAFQMpp87IEoRXAcV764xklFpw8vOvQS8X85QU+ApTTPIp/F\n\
dpNrpqz+UxO8ydO/edqXXEYdmlJhvHzmIfVevGjwvDJv3N8QKCPhmnIS/Co5leX+\n\
aPDHPx1BCdD1FaKeMErWzecpp1DCjn1PJbp+u3e2vY+72Ym9+Uj29erjkt8U3ifV\n\
0A0JAgMBAAGjYDBeMB8GA1UdIwQYMBaAFPmq0BI0G6csZf8a8H1JiCUZHtGsMB0G\n\
A1UdDgQWBBRIa9gnMDfGriZtNZZD0TjjdsLt8TAMBgNVHRMBAf8EAjAAMA4GA1Ud\n\
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAmrUdSd5X3uvcdHHWQch3VOUw\n\
MX7D4FW8cjJwJvi1i4rayeQlHAD1m1qiGx1V8KZuGCEK+xdyOaQwC0On03AQUItc\n\
AgNpCFFEbI8gIgEv084JoOUzABglBKFknlzPmadtOD4P8YIM80fvhvG4GWLnvuFa\n\
XVQPXmgLOxe4B1fk7aizrRpUV3aR/MauHhikpQHHUdZnuYaMpMCqJBpVQnAmLBsf\n\
BBavcWnfdVUWWmGSHgzaQ7WGznq9upcNEuGkwZkCZoQOV128+xf1m5S2JNhXrLHf\n\
L3is0Hy/KLSr2jOsgz5qKalAN1JTX1K6WGfRoEHRwT6juhZizkyrkrer87kr5A==\n\
-----END CERTIFICATE-----\r\n";

	char Usr_KEY[] = "-----BEGIN RSA PRIVATE KEY-----\n\
MIIEpQIBAAKCAQEAx4BcS22/JAYoSHhFD6fwCmHsh7RauzT7aVQ0QP+Rn8m7DmgE\n\
jxY3hP7Fa/yCOPlzAEAnTsZ5SdYHbRJI14/J9J7jbyH1++EF1+VGFSkYx17ku7JL\n\
a+Q9KUQ3AgYNWoD0maf6IUw9za+PfWHN20nSxQOixRoxY3TUvBABUDKafOyBKEVw\n\
HFe+uMZJRacPLzr0EvF/OUFPgKU0zyKfxXaTa6as/lMTvMnTv3nal1xGHZpSYbx8\n\
5iH1Xrxo8Lwyb9zfECgj4ZpyEvwqOZXl/mjwxz8dQQnQ9RWinjBK1s3nKadQwo59\n\
TyW6frt3tr2Pu9mJvflI9vXq45LfFN4n1dANCQIDAQABAoIBAQCBIkgblvUzZzzp\n\
89eYE7dfsRT+njJgnSHjgtqz+/mR+W04HQJecB4N2y+E/5R3NphG4D/RUM0viveR\n\
lVVLGsq5qaNDyzLDEFvM7J5Zkhl066qZCAaj1r/0FzCYAbzhCTnKwB07MnOK0QnL\n\
OeSa/gUM+5Z/iL3RR81v0dhXxfmTxAohKrgtkXeP4oxIGVwJKpqBTEsBwOCTLbyT\n\
UlUO1LkWct7ysD4qfk6QmGLNTNY91Nwzaveg56eiHwwmQq82MRwNiB4BB0FrFhM8\n\
pXNvAuJiHW9eOeXU+qEa8rWEIiL8vJu1z95laaasvjNsUAgjI3NOqxXMwtrakOpY\n\
SfkFDNalAoGBAOJ1BXJKrbFkxcRcMMiETHCF9zgCFseqTEGNehbYlomjQSNRneuX\n\
B+r88ABC8crL+mdxZZv5pNQ6uZ+DCxTd8e1FBRyfDQ3PvH/PH8/GAB4h8AY0gqIE\n\
uh7Z18uD2pI4DWXKTUKquDsvRTzU5mcNN4+WMrQ8bFChFNQZuj+J8Y6HAoGBAOGH\n\
GcV2/QFBqvUhVXhosMGZvgslNRn8iJDcDUbR1z7/BHLJmdH/PahKkvT2hgPWwbGq\n\
2rH/lL179Qm9KhjmXCq9UoAffSZCd5yj5VY9fSInTfVoQrmOynsrhduVjVTwQHx6\n\
HnrQaJ7mYSllfFhnZqvICcHHavE99i7kfvhJzlvvAoGBAKe/7cfrH91ve7xoWk6/\n\
Cey0oBwiGAPR1+kbRxxTVrggceka1XOmDlx4/8QB0yDZluU3sAApnnsEAWry5KuR\n\
eVu8q/u2qjHX95dps/PyhTXNCANgtIAWJlUiI+ODCc8UrIABqte+pPhanNxiDUa7\n\
GLRJm8e9lrLnkAlKAZh9s61TAoGAOoH8E5hc5riMi2+MZuJyCKfD/8w1THqRS1Nr\n\
NlY2CyfJlaXre+lFq6U9HsPd3J0YPuleNxNhGQ9RcKdchn0sqAHZp1eD9isEMOTs\n\
GEV3cybTOLlFpjH/IZkW7vKLad+yOxjLU31BxwdpaX3klmKs5h/NODJt8gvrFBs/\n\
PVaYbI0CgYEAkKdmKs/OinXgg+OZZKRP9NDdcAC6tU0z+hcJro5XvgPZyaPLHPZV\n\
EiBeaDGu2GFxU3xMC4Pco1+qoetbHse8Sw5FF4dgWLoPvax2694XQG2nPk4kZ1go\n\
FSFcEE0C9x2LB5Busc2qSAEaxuZOmQ4NJZX40Sh/23Bx5j/uR4nP9eY=\n\
-----END RSA PRIVATE KEY-----\r\n";


	HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QMTCFG=\"SSL\",0,1,2\r\n", strlen("AT+QMTCFG=\"SSL\",0,1,2\r\n"), 1000);
	HAL_Delay(1000);
//	HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QSECWRITE=\"RAM:cacert.pem\",1187,100\r\n", strlen("AT+QSECWRITE=\"RAM:cacert.pem\",1187,100\r\n"), HAL_MAX_DELAY);
//	HAL_Delay(300);
//	HAL_UART_Transmit(&huart2, (uint8_t *)CA_CERT, strlen(CA_CERT), HAL_MAX_DELAY);
////	HAL_UART_Transmit(&huart2, (uint8_t *)"\r\n", strlen("\r\n"), HAL_MAX_DELAY);
//	HAL_Delay(3000);
//	HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QSECWRITE=\"RAM:client.pem\",1224,100\r\n", strlen("AT+QSECWRITE=\"RAM:client.pem\",1224,100\r\n"), HAL_MAX_DELAY);
//	HAL_Delay(300);
//	HAL_UART_Transmit(&huart2, (uint8_t *)Cli_CERT, strlen(Cli_CERT), HAL_MAX_DELAY);
////	HAL_UART_Transmit(&huart2, (uint8_t *)"\r\n", strlen("\r\n"), HAL_MAX_DELAY);
//	HAL_Delay(300);
//	HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QSECWRITE=\"RAM:user_key.pem\",1679,100\r\n", strlen("AT+QSECWRITE=\"RAM:user_key.pem\",1679,100\r\n"), HAL_MAX_DELAY);
//	HAL_Delay(300);
//	HAL_UART_Transmit(&huart2, (uint8_t *)Usr_KEY, strlen(Usr_KEY), HAL_MAX_DELAY);
////	HAL_UART_Transmit(&huart2, (uint8_t *)"\r\n", strlen("\r\n"), HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QSSLCFG=\"cacert\",2,\"cacert.pem\"\r\n", strlen("AT+QSSLCFG=\"cacert\",2,\"cacert.pem\"\r\n"), 1000);
	HAL_Delay(1000);
	HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QSSLCFG=\"clientcert\",2,\"client.pem\"\r\n", strlen("AT+QSSLCFG=\"clientcert\",2,\"client.pem\"\r\n"), 1000);
	HAL_Delay(1000);
	HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QSSLCFG=\"clientkey\",2,\"user_key.pem\"\r\n", strlen("AT+QSSLCFG=\"clientkey\",2,\"user_key.pem\"\r\n"), 1000);
	HAL_Delay(1000);
	HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QSSLCFG=\"seclevel\",2,2\r\n", strlen("AT+QSSLCFG=\"seclevel\",2,2\r\n"), 1000);
	HAL_Delay(1000);
	HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QSSLCFG=\"sslversion\",2,4\r\n", strlen("AT+QSSLCFG=\"sslversion\",2,4\r\n"), 1000);
	HAL_Delay(1000);
	HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QSSLCFG=\"ciphersuite\",2,\"0xFFFF\"\r\n", strlen("AT+QSSLCFG=\"ciphersuite\",2,\"0xFFFF\"\r\n"), 1000);
	HAL_Delay(1000);
	HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QSSLCFG=\"ignorertctime\",1\r\n", strlen("AT+QSSLCFG=\"ignorertctime\",1\r\n"), 1000);
	HAL_Delay(1000);

}

const char* mqtt_broker = "a2lrvn2efyqxdn-ats.iot.us-east-1.amazonaws.com";
const char* client_id = "M66";

void AWS_MQTT(void){

//	HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QSSLCFG=\"seclevel\",2,2\r\n", strlen("AT+QSSLCFG=\"seclevel\",2,2\r\n"), HAL_MAX_DELAY);
//	HAL_Delay(300);
//	HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QSSLCFG=\"sslversion\",2,4\r\n", strlen("AT+QSSLCFG=\"sslversion\",2,4\r\n"), HAL_MAX_DELAY);
//	HAL_Delay(300);
//	HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QSSLCFG=\"ciphersuite\",2,\"0xFFFF\"\r\n", strlen("AT+QSSLCFG=\"ciphersuite\",2,\"0xFFFF\"\r\n"), HAL_MAX_DELAY);
//	HAL_Delay(300);
//	HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QSSLCFG=\"ignorertctime\",1\r\n", strlen("AT+QSSLCFG=\"ignorertctime\",1\r\n"), HAL_MAX_DELAY);
//	HAL_Delay(300);

//	HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QMTOPEN=0,\"a2lrvn2efyqxdn-ats.iot.us-east-1.amazonaws.com\",8883\r\n",74 , 1000);
	char mqtt_open_command[128];
	sprintf(mqtt_open_command, "AT+QMTOPEN=0,\"%s\",8883\r\n", mqtt_broker);
	send_at_command(mqtt_open_command);
	HAL_Delay(20000);
//	send_at_command("AT+QMTOPEN=0,\"%s\",1883\r\n", mqtt_broker);
//	send_at_command("AT+QMTCONN=0,\"%s\"\r\n", client_id);
	char mqtt_conn_command[128];
	sprintf(mqtt_conn_command, "AT+QMTCONN=0,\"%s\"\r\n", client_id);
	send_at_command(mqtt_conn_command);
	HAL_Delay(10000);
	const char* topic = "$aws/things/M66/shadow";
	char mqtt_sub_command[128];
	sprintf(mqtt_sub_command, "AT+QMTSUB=0,1,\"%s\",1\r\n", topic);
	send_at_command(mqtt_sub_command);
	HAL_Delay(1000);
}


//AT+QMTOPEN=0,"a2lrvn2efyqxdn-ats.iot.us-east-1.amazonaws.com",8883

// Function to send AT command to Quectel M66
void send_at_command(const char* command) {
    HAL_UART_Transmit(&huart2, (uint8_t*)command, strlen(command), 1000);
}

// Function to receive data from Quectel M66 module
// Updated receive_data function
size_t receive_data(uint8_t* buffer, uint16_t length) {

    size_t bytes_received = 0;

    HAL_StatusTypeDef status = HAL_UART_Receive(&huart2, buffer, length, 10000);

    if (status == HAL_OK) {
        // UART data received successfully, update the number of bytes received
        bytes_received = length;
    }

    // Return the actual number of bytes received
    return bytes_received;
}


// Implement a function to handle MQTT messages. When an update notification is received, extract the firmware URL.
void handle_mqtt_message(const char* message) {
//    // Parse the MQTT message (assumes it's in JSON format)
//    // Extract the firmware URL from the message
//
//    // Example JSON parsing (for demonstration purposes):
//     char* firmware_url = NULL;
//     char* message_type = NULL;
//
//    // Parse the JSON message to extract firmware_url and message_type
//    // You may need to use a JSON parsing library or write your parser
//    // Example: {"message_type":"update_available","firmware_url":"http://your-fota-server/firmware.bin"}
//    if (sscanf(message, "{\"message_type\":\"%[^\"]\",\"firmware_url\":\"%[^\"]\"}", message_type, firmware_url) == 2) {
//        if (strcmp(message_type, "update_available") == 0) {
//
//        	if(download_firmware(firmware_url));
//
//        }
//    }
    // Implement logic to handle other message types or errors.
	const char* json_start = strchr(message, '{'); // Find the first '{' character

	cJSON* root = cJSON_Parse(json_start);

	if (root == NULL) {
	    const char* error_ptr = cJSON_GetErrorPtr();
	    if (error_ptr != NULL) {
	        fprintf(stderr, "JSON parsing error before: %s\n", error_ptr);
	    }
	}
	    if (root != NULL) {


	        cJSON* messageTypeObj = cJSON_GetObjectItem(root, "message_type");
	        cJSON* firmwareUrlObj = cJSON_GetObjectItem(root, "firmware_url");

	        if (messageTypeObj != NULL && firmwareUrlObj != NULL) {
	            const char* message_type = messageTypeObj->valuestring;
	            const char* firmware_url = firmwareUrlObj->valuestring;
//	            send_at_command(firmware_url);

	            if (strcmp(message_type, "update_available") == 0) {
	                            // The message is of type "update_available"
	                            // You can now use the extracted firmware URL in the 'firmware_url' variable.
	            	if(download_firmware(firmware_url)){
	            		if(verify_firmware_update(firmware_buffer, firmware_size)){
	            			if(write_firmware_to_flash(firmware_buffer, firmware_size)){
	            				NVIC_SystemReset();

	            			}
	            		}
	            	}
	                        }
	                    }

	                    // Free the cJSON object when done
	                    cJSON_Delete(root);
	                }


}

bool download_firmware(const char* firmware_url) {
    // Send AT command to set up HTTP connection
//    send_at_command("AT+QHTTPCFG=\"contextid\",1\r\n");
//    send_at_command("AT+QSSLCFG=\"https\",1\r\n");
//    HAL_Delay(1000);
	HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QSSLCFG=\"https\",1\r\n", strlen("AT+QSSLCFG=\"https\",1\r\n"), 1000);
	Delay(10000);
//    send_at_command("AT+QSSLCFG=\"httpsctxi\",1\r\n");
//    HAL_Delay(1000);
    HAL_UART_Transmit(&huart2, (uint8_t *)"AT+QSSLCFG=\"httpsctxi\",1\r\n", strlen("AT+QSSLCFG=\"httpsctxi\",1\r\n"), 1000);
    Delay(10000);

    // Send AT command to set the URL for HTTP GET request
    char http_get_command[128];
    sprintf(http_get_command, "AT+QHTTPURL=%d,60\r\n", strlen(firmware_url));
    send_at_command(http_get_command);
    Delay(10000);
    send_at_command(firmware_url);


    // Send AT command to initiate HTTP GET request
    send_at_command("AT+QHTTPGET=60\r\n");
    Delay(50000);
    send_at_command("AT+QHTTPREAD=30\r\n");

    // Wait for the HTTP response (you may need to implement proper response handling)
    // Receive and store firmware data in the firmware_buffer
    // You should implement logic to handle HTTP response and store data here.

    // Example: Receive data until the end of HTTP response
    uint8_t receive_buffer[256];
    size_t bytes_received;
    while (true) {
        // Receive data from UART and get the number of bytes received
        bytes_received = receive_data(receive_buffer, sizeof(receive_buffer));

        // Check for the end of HTTP response and break if found
        char* end_of_response = strstr((char*)receive_buffer, "\r\nOK\r\n");
        if (end_of_response != NULL) {
            // Calculate the length of the HTTP response data
            size_t response_length = end_of_response - (char*)receive_buffer;

            // Ensure that the firmware_buffer has enough space
            if (firmware_size + response_length <= MAX_FIRMWARE_SIZE) {
                // Copy received data to the firmware_buffer
                memcpy(firmware_buffer + firmware_size, receive_buffer, response_length);
                firmware_size += response_length;
            } else {
                // Firmware buffer is full, handle the error as needed
                return false;
            }

            break; // End of HTTP response found
        } else {
            // Copy received data to the firmware_buffer
            if (firmware_size + bytes_received <= MAX_FIRMWARE_SIZE) {
                memcpy(firmware_buffer + firmware_size, receive_buffer, bytes_received);
                firmware_size += bytes_received;
            } else {
                // Firmware buffer is full, handle the error as needed
                return false;
            }
        }
    }
    // Check if firmware download was successful
    if (firmware_size > 0) {
        return true;
    } else {
        return false;
    }
}


// Verify firmware update with CRC
bool verify_firmware_update(uint8_t* firmware_data, size_t firmware_length) {
    // Read the CRC value from the end of the firmware data (adjust for CRC length)
    uint32_t* crc_ptr = (uint32_t*)(firmware_data + firmware_length - sizeof(uint32_t));
    uint32_t received_crc = *crc_ptr;

    // Calculate CRC for the received firmware data (excluding the CRC field)
    uint32_t calculated_crc = calculate_crc32(firmware_data, firmware_length - sizeof(uint32_t));

    // Compare the received and calculated CRC values
    if (received_crc == calculated_crc) {
        // CRC matches, firmware is valid
        return true;
    } else {
        // CRC does not match, firmware is corrupted
        return false;
    }
}

// Function to calculate CRC32 for a buffer
uint32_t calculate_crc32(uint8_t* data, size_t length) {
    uint32_t crc = 0xFFFFFFFFu; // Initial CRC value (all bits set)

    for (size_t i = 0; i < length; i++) {
        uint32_t byte = data[i];
        crc ^= byte; // XOR with each byte

        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ CRC32_POLYNOMIAL;
            } else {
                crc >>= 1;
            }
        }
    }

    // XOR with final value (bitwise negation of the initial value)
    crc ^= 0xFFFFFFFFu;

    return crc;
}

void write_firmware_to_flash(uint8_t* firmware_data, size_t firmware_length) {
    // Erase the entire TEMP sector
    HAL_StatusTypeDef status = HAL_FLASH_Unlock();
    if (status != HAL_OK) {
        // Handle flash unlock error
        return;
    }

    FLASH_EraseInitTypeDef erase_info;
    erase_info.TypeErase = FLASH_TYPEERASE_SECTORS;
    erase_info.Sector = FLASH_SECTOR_8; // TEMP sector on STM32F4
    erase_info.NbSectors = 1;
    erase_info.VoltageRange = FLASH_VOLTAGE_RANGE_3; // Adjust voltage range as needed

    uint32_t error_sector;
    status = HAL_FLASHEx_Erase(&erase_info, &error_sector);
    if (status != HAL_OK) {
        // Handle flash erase error
        return;
    }

    // Program the firmware data into flash memory
    uint32_t* flash_address = (uint32_t*)TEMP_START_ADDR;
    uint32_t* firmware_data_ptr = (uint32_t*)firmware_data;

    for (size_t i = 0; i < firmware_length; i += 4) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)flash_address, *firmware_data_ptr) != HAL_OK) {
            // Handle flash programming error
            return;
        }
        flash_address++;
        firmware_data_ptr++;
    }

    // Lock the flash memory after programming
    HAL_FLASH_Lock();
}








