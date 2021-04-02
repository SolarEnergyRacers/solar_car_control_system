//
// Digital to Analog Converter
//

#include "../../include/definitions.h"

#include <I2CBus.h>
#include "DAC.h"

#include <Wire.h> // I2C
#include <inttypes.h>

#define BASE_ADDR_CMD 0xA8


void init_adc() {
    // nothing to do, i2c bus is getting initialized externally
}

byte get_cmd(pot_chan channel){
    byte command = BASE_ADDR_CMD;
    switch (channel) {
        case POT_CHAN0:
            command |= 0x01;
            break;
        case POT_CHAN1:
            command |= 0x10;
            break;
        case POT_CHAN_ALL:
            command |= 0x11;
            break;
        default:
            command |= 0x01;
            break;
    }
    return command;
}


void set_pot(byte val, pot_chan channel){

    // setup command
    byte command = get_cmd(channel);

    // CRITICAL SECTION I2C: start
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);

    Wire.beginTransmission(I2C_ADDRESS_DS1803);
    Wire.write(command);
    Wire.write(val); // first pot value
    if(channel == POT_CHAN_ALL){
        Wire.write(val); // second pot value
    }
    Wire.endTransmission();

    xSemaphoreGive(i2c_mutex);
    // CRITICAL SECTION I2C: end
}

uint16_t get_pot(pot_chan channel){

    // CRITICAL SECTION I2C: start
    xSemaphoreTake(i2c_mutex, portMAX_DELAY);

    Wire.requestFrom(I2C_ADDRESS_DS1803, 2); // request 2 bytes
    byte pot0 = Wire.read(); // get pot0
    byte pot1 = Wire.read(); // get pot1

    xSemaphoreGive(i2c_mutex);
    // CRITICAL SECTION I2C: end

    if(channel == POT_CHAN_ALL){
        return pot0 | (pot1 << 8);
    } else if(channel == POT_CHAN0) {
        return pot0;
    } else { // POT_CHAN1
        return pot1;
    }
}


