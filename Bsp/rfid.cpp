#include "rfid.h"

extern "C" {
#include "bsp_USART.h"
}

struct RFID_READ_HEADER {
    char Header[2];
    char FrameType;
    char Address[2];
    char FrameCode;
    char ParamLength[2];
};

int sign(const char* buf, const int& size)
{
    int res = 0;
    for (int i = 0; i < size; ++i) {
        res += buf[i] ^ 0xE5;
    }
    return res;
}

int Rfid::read()
{
    char* command = const_cast<char*>("\x52\x46\x00\x00\x00\x22\x00\x00\x46");
    USART2_SendData(reinterpret_cast<uint8_t*>(command), 9);

    uint8_t* rxdata = USART2_GetReceivedData();
    uint16_t rxNum = USART2_GetReceivedNum();
    if (!rxNum) return false;

    RFID_READ_HEADER header = {{0}, 0, {0}, 0, {0}};
    int num = 0;

    memcpy(&header, rxdata, sizeof(header));

    num = header.ParamLength[1] / DATA_GROUP_LEN;
    num = num > MAX_RFID_READ_LEN ? MAX_RFID_READ_LEN : num;
    if (num <= 0) {
        USART2_ClearReceived();
        return 0;
    }

    // Copy rfid data
    uint8_t* p = rxdata + sizeof(header);
    for (int n = 0; n < num; n++) {
        char data[DATA_GROUP_LEN + 1] = {0};
        for (int i = 0; i < DATA_GROUP_LEN - 1; ++i) {
            data[i] = *(p + i);
        }
        p += DATA_GROUP_LEN;

        // Decode
        memset(this->m_rfid_data[n].name, 0, 5);
        memset(this->m_rfid_data[n].code, 0, 5);

        sprintf(this->m_rfid_data[n].name, "%x", sign(data, DATA_GROUP_LEN + 1));
        sprintf(this->m_rfid_data[n].code, "%d", data[4]);
    }

    USART2_ClearReceived();
    return num;
}