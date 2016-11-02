#include "simplefsk.h"

FSK::FSK(uint8_t trigger_pin) :
    _has_buffer(false),
    _trigger_pin(trigger_pin){
        pinMode(_trigger_pin, OUTPUT);
        if(Serial){
            Serial.println("FSK initialized");
        }
    }

void FSK::set_buffer(char* buf, uint8_t buf_size){
    _has_buffer = true;
    _buf_size = 0; //WTF????
    strcpy(_buffer, buf);
    _type_size = sizeof(_buffer[0]) * 8;
        if(Serial){
            Serial.println("Buffer is of size: ");
            Serial.print(_type_size * buf_size);
            Serial.println(" bits");
        }

}


/*
 * Non-blocking FSK-transmission
 * Rolls through the _buffer
 */
int FSK::process(){
    if(!_has_buffer){
        if(Serial){
            Serial.println("FSK does not have a buffer, returned -1");
        }
        return -1;
    }
    unsigned long current_time = millis();
    if(current_time - _transmission_time > 2){ //transmission spacing
        _transmission_time = current_time;
        if(_bit_ptr >= _type_size){ //end of byte
            _bit_ptr = 0;
            _buffer_ptr++;
        }else{
            if(bitRead(_buffer[_buffer_ptr], _bit_ptr)){ //if it's a one
                if(current_time - _bit_time > 0.2){
                    _bit_time = current_time;
                    _trigger_state = !_trigger_state; //NOT-operator to flipflop trigger
                    digitalWrite(_trigger_pin, _trigger_state);
                    _fsk_counter++;
                }
            }else{ //if it's a zero
                if(current_time - _bit_time > 0.4){
                    _bit_time = current_time;
                    _trigger_state = !_trigger_state; //NOT-operator to flipflop trigger
                    digitalWrite(_trigger_pin, _trigger_state);
                    _fsk_counter += 2;
                }
            }
        }

        if(_buffer_ptr >= _buf_size){ //End of buffer
            _buffer_ptr = 0;
        }
    if(_fsk_counter >= 5){
        _bit_ptr++;
    }
    }
    return (int) _buffer_ptr;
    }
