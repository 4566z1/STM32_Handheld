#include "main.h"

Rfid rfid;
BLE ble;

volatile int mode = 0;
volatile bool rfid_flag = false;

int stm32_main(void)
{
    ssd1306_Init(&hi2c1);

    while (1) {
        mode = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6);

        // RFID
        if (rfid.read()) {
            if (rfid_flag) {
                ble.send(rfid.get_name(), rfid.get_code(), (const int&)mode);

                // 蜂鸣器
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
                HAL_Delay(100);
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
                rfid_flag = false;
            }
        }

        // Screen
        {
            char str[20] = {0};

            // 显示出库入库模式
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
    if (GPIO_Pin == GPIO_PIN_1) {
        rfid_flag = true;
    } else if (GPIO_Pin == GPIO_PIN_4) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
    }
}