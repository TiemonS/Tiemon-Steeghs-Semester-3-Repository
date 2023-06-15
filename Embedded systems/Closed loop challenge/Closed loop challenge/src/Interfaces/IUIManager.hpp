#include <iostream>
#include "main.h"
#include "cmsis_os.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <stdio.h>

#ifndef IUIMANAGER_HPP
#define IUIMANAGER_HPP


class IUIManager {
public:
    virtual ~IUIManager() {}

    virtual int getUserInput(UART_HandleTypeDef *huart) = 0;
    virtual void PrintUserInput(int choice, UART_HandleTypeDef *huart) = 0;
    virtual void printMessage(const char* message, UART_HandleTypeDef *huart) = 0;
    virtual void printDistance(int distance, UART_HandleTypeDef *huart) = 0;
};

#endif