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

bool Rfid::read()
{
    RFID_READ_HEADER header = {{0}, 0, {0}, 0, {0}};
    char* command = const_cast<char*>("\x52\x46\x00\x00\x00\x22\x00\x00\x46");

    // Read header
    USART2_SendData(reinterpret_cast<uint8_t*>(command), 9);
    uint8_t* rxdata = USART2_GetReceivedData();
    memcpy(&header, rxdata, sizeof(header));
    USART2_ClearReceived();

    if (header.ParamLength[1] / DATA_GROUP_LEN <= 0) return false;

    // Copy
    for (int i = 0; i < DATA_GROUP_LEN - 1; ++i) {
        this->m_data[i] = *(rxdata + sizeof(header) + i);
    }

    // Write str data
    memset(this->m_name, 0, 5);
    memset(this->m_code, 0, 5);

    sprintf(this->m_name, "%x", sign(this->m_data, DATA_GROUP_LEN + 1));
    sprintf(this->m_code, "%d", m_data[4]);
    return true;
}