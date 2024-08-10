#ifndef RFID_H
#define RFID_H

#define DATA_GROUP_LEN 19
#define MAX_RFID_READ_LEN 10

struct rfid_s {
    char name[5] = {0};
    char code[5] = {0};
};

class Rfid
{
   public:
    explicit Rfid() {}

    // 主动盘存
    // const char* get_name() { return this->m_name; }
    // const char* get_code() { return this->m_code; }
    const rfid_s* get_data() { return m_rfid_data; }
    int read();

   private:
    rfid_s m_rfid_data[MAX_RFID_READ_LEN] = {0};
};

#endif