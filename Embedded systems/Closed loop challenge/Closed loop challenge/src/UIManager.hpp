#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include <iostream>

class UIManager {

private:
    const int msgBufSize = 120;
    const int input_Size = 1;   
    bool recievedMessage = false;

public:
    int getUserInput(UART_HandleTypeDef *huart);

    void handleUserInput(int choice, UART_HandleTypeDef *huart);

    void printMessage(const char* message, UART_HandleTypeDef *huart);
};

#endif