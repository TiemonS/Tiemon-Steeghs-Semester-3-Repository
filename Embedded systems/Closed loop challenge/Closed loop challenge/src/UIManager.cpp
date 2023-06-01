#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>

#include "UIManager.hpp"

void UIManager::showOptions(UART_HandleTypeDef *huart) {
    char msgBuf[MSGBUFSIZE];
    snprintf(msgBuf, MSGBUFSIZE, "Menu:\r\n1. Optie 1\r\n2. Optie 2\r\n3. Optie 3\r\n4. Afsluiten\r\n");
    HAL_UART_Transmit(huart, (uint8_t*)msgBuf, strlen(msgBuf), HAL_MAX_DELAY);
}

void UIManager::getUserInput(UART_HandleTypeDef *huart) {
    char msgBuf[MSGBUFSIZE];
    snprintf(msgBuf, MSGBUFSIZE, "Voer uw keuze in: ");
    HAL_UART_Transmit(huart, (uint8_t*)msgBuf, strlen(msgBuf), HAL_MAX_DELAY);

    char userInput[INPUT_SIZE];
    HAL_UART_Receive(huart, (uint8_t*)userInput, INPUT_SIZE, HAL_MAX_DELAY);

    int choice = atoi(userInput);
    handleUserInput(choice, huart);
}

void UIManager::handleUserInput(int choice, UART_HandleTypeDef *huart) {
    char msgBuf[MSGBUFSIZE];

    switch (choice) {
        case 1:
            // Optie 1 geselecteerd
            snprintf(msgBuf, MSGBUFSIZE, "Optie 1 geselecteerd.\r\n");
            break;
        case 2:
            // Optie 2 geselecteerd
            snprintf(msgBuf, MSGBUFSIZE, "Optie 2 geselecteerd.\r\n");
            break;
        case 3:
            // Optie 3 geselecteerd
            snprintf(msgBuf, MSGBUFSIZE, "Optie 3 geselecteerd.\r\n");
            break;
        case 4:
            // Afsluiten geselecteerd
            snprintf(msgBuf, MSGBUFSIZE, "Programma wordt afgesloten.\r\n");
            // Voer hier code uit om het programma af te sluiten
            break;
        default:
            snprintf(msgBuf, MSGBUFSIZE, "Ongeldige keuze.\r\n");
            break;
    }

    HAL_UART_Transmit(huart, (uint8_t*)msgBuf, strlen(msgBuf), HAL_MAX_DELAY);
}