#ifndef BLUETOOTH_H
#define BLUETOOTH_H

class BLE
{
   public:
    explicit BLE() 
    {}

    void send(const char* name, const char* code, const int& mode);

   private:
};

#endif