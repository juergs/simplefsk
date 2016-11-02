#ifndef simplefsk_h
#define simplefsk_h

#include <Arduino.h>

class FSK {
    public:
        FSK(uint8_t _trigger_pin);
        int process();
        template <typename Data> void send_data_locking(Data d);
        void set_buffer(char* buf, uint8_t buffer_size);
    private:
        bool _has_buffer;

        uint8_t _trigger_pin;
        bool _trigger_state = 0;
        uint8_t _buffer[]; //Byte array buffer
        uint8_t _buf_size = 0; //Length of buffer
        uint8_t _type_size = 0; //Size of type in buffer

        uint8_t _buffer_ptr = 0;
        uint8_t _bit_ptr = 0;

        uint8_t _fsk_counter = 0;
        unsigned long _bit_time;
        uint8_t _bit_counter = 0;
        unsigned long _transmission_time;
};


/*
 * Naive style that blocks the Arduino for signal timing
 */
template <typename Data> void FSK::send_data_locking(Data d){
    int size = sizeof(d) * 8; //Get size of type

    for (int i = 0; i < size; i++) {
        bool bit = bitRead(d, i);
        if(bit){
            for (int i = 0; i < 10; i++) {
                digitalWrite(_trigger_pin, HIGH);
                delayMicroseconds(100);
                digitalWrite(_trigger_pin, LOW);
                delayMicroseconds(100);
            }
        }else{
            for (int i = 0; i < 5; i++) {
                digitalWrite(_trigger_pin, HIGH);
                delayMicroseconds(200);
                digitalWrite(_trigger_pin, LOW);
                delayMicroseconds(200);
            }
        }
    }
    delay(1);
}
#endif
