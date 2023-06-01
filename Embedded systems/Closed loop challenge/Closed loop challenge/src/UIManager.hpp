#ifndef UIMANAGER_HPP
#define UIMANAGER_HPP

#include <iostream>

class UIManager {

private:
    const int MSGBUFSIZE = 80;
    const int INPUT_SIZE = 1;   

public:
    void showOptions(UART_HandleTypeDef *huart);

    void getUserInput(UART_HandleTypeDef *huart);

    void handleUserInput(int choice, UART_HandleTypeDef *huart);
};

#endif