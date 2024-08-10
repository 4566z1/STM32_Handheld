#include "user.h"

Rfid rfid;
BLE ble;

volatile int mode = 0;
volatile bool rfid_flag = false;
volatile bool rfid_mode = false;    // 0为单读，1为扫读

/**
 * @brief 入口函数
 *
 * @return int
 */
int stm32_main(void)
{
    ssd1306_Init(&hi2c1);

    while (1) {
        mode = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6);

        // RFID
        int read_num = 0;
        if (rfid_flag) {
            read_num = rfid.read();
            if (read_num) {
                if (rfid_mode == 0) read_num = 1;

                // 服务器直接上传
                const rfid_s* rfid_data = rfid.get_data();
                for (int i = 0; i < read_num; i++) {
                    !mode ? server.product_add(rfid_data[i].name, rfid_data[i].code)
                          : server.product_del(rfid_data[i].name);
                }

                // 蜂鸣器
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
                HAL_Delay(100);
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);

                if(!rfid_mode)
                    rfid_flag = false;
            }
        }

        // Screen
        {
            char str1[20] = {0}, str2[20] = {0};

            !rfid_mode ? sprintf(str1, "SINGMODE") : sprintf(str1, "SCANMODE");
            !rfid_flag ? sprintf(str2, "STOP") : sprintf(str2, "WORK");

            ssd1306_SetCursor(0, 16);
            ssd1306_WriteString(str1, Font_16x26, White);
            ssd1306_SetCursor(36, 42);
            ssd1306_WriteString(str2, Font_11x18, White);
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
    if (GPIO_Pin == GPIO_PIN_5) {
        rfid_mode = !rfid_mode;
    } else if (GPIO_Pin == GPIO_PIN_9) {
        rfid_flag = !rfid_flag;
    } else if (GPIO_Pin == GPIO_PIN_4) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
    }
}