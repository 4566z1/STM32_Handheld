#include "bluetooth.h"
#include "common_inc.h"
#include "i2c.h"
#include "rfid.h"
#include "ssd1306.h"

extern "C" {
#include "bsp_USART.h"
}

Rfid rfid;
BLE ble;

int stm32_main(void)
{
    ssd1306_Init(&hi2c1);

    while (1) {
        // RFID
        int mode = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6);
        {
            if (rfid.read()) {
                LOG("stm32_main => name: %s code: %s mode: %d \r\n", rfid.get_name(), rfid.get_code(), mode);

                ble.send(rfid.get_name(), rfid.get_code(), mode);

                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
                HAL_Delay(100);
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
            }
        }

        // SCREEN
        {
            char str[20] = {0};

            !mode ? sprintf(str, "ADD MODE") : sprintf(str, "DEL MODE");
            ssd1306_SetCursor(0, 32);
            ssd1306_WriteString(str, Font_16x26, White);
            ssd1306_UpdateScreen(&hi2c1);
        }

        HAL_Delay(1);
    }
    return 0;
}

/**
 * @brief 按键中断
 * 
 */
extern "C" void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_4) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
    }
}