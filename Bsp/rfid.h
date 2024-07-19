#ifndef RFID_H
#define RFID_H

const int DATA_GROUP_LEN = 19;

class Rfid
{
   public:
    explicit Rfid() {}

    // 主动盘存
    const char* get_name() { return this->m_name; }
    const char* get_code() { return this->m_code; }
    bool read();

   private:
    char m_data[DATA_GROUP_LEN + 1] = {0};
    char m_name[5] = {0};
    char m_code[5] = {0};
};

#endif