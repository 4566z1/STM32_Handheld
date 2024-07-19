#include "bluetooth.h"

extern "C" {
#include "bsp_USART.h"
}

void BLE::send(const char* name, const char* code, const int& mode) 
{ 
    char buf[20] = {0};
    sprintf(buf, ":%s:%s:%d", name, code, mode);

    USART3_SendString(buf); 
}