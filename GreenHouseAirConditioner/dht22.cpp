// thanks to Wim De Roeve, HO WING KIT

#include "dht22.h"

const int DHT_DATA_BIT_COUNT = 40;

DHT::DHT(PinName pin) {
    _pin = pin;
    _firsttime = true;
}

DHT::~DHT() {
}

eError DHT::stall(DigitalInOut &io, int const level, int const max_time) {
    int cnt = 0;
    while (level == io) {
        if (cnt > max_time) {
            return ERROR_NO_PATIENCE;
        }
        cnt++;
        wait_us(1);
    }
    return ERROR_NONE;
}

eError DHT::readData() {
    uint8_t i = 0, j = 0, b = 0, data_valid = 0;
    uint32_t bit_value[DHT_DATA_BIT_COUNT] = {0};

    eError err = ERROR_NONE;
    time_t currentTime = time(NULL);

    DigitalInOut DHT_io(_pin);

    // IO must be in high state to start
    // !!! high state means output
    if (ERROR_NONE != stall(DHT_io, 0, 250)) {
        return BUS_BUSY;
    }

    // start the transfer
    DHT_io.output();
    DHT_io = 0;
    // 500uS for DHT22, 18ms for DHT11
    wait_us(500);
    DHT_io = 1;
    wait_us(30);
    DHT_io.input();
    // wait till the sensor grabs the bus
    // !!! wait till sensor will output sth (IC is in input state)
    if (ERROR_NONE != stall(DHT_io, 1, 40)) {
        return ERROR_NOT_PRESENT;
    }
    // sensor should signal low 80us and then hi 80us
    if (ERROR_NONE != stall(DHT_io, 0, 100)) {
        return ERROR_SYNC_TIMEOUT;
    }
    if (ERROR_NONE != stall(DHT_io, 1, 100)) {
        return ERROR_NO_PATIENCE;
    }
    // capture the data
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 8; j++) {
            if (ERROR_NONE != stall(DHT_io, 0, 75)) {
                return ERROR_DATA_TIMEOUT;
            }
            // logic 0 is 28us max, 1 is 70us
            wait_us(40);
            bit_value[i * 8 + j] = DHT_io;
            if (ERROR_NONE != stall(DHT_io, 1, 50)) {
                return ERROR_DATA_TIMEOUT;
            }
        }
    }
    // store the data
    for (i = 0; i < 5; i++) {
        b = 0;
        for (j = 0; j < 8; j++) {
            if (bit_value[i * 8 + j] == 1) {
                b |= (1 << (7 - j));
            }
        }
        DHT_data[i] = b;
    }

    // uncomment to see the checksum error if it exists
    //printf(" 0x%02x + 0x%02x + 0x%02x + 0x%02x = 0x%02x \n", DHT_data[0], DHT_data[1], DHT_data[2], DHT_data[3], DHT_data[4]);
    data_valid = DHT_data[0] + DHT_data[1] + DHT_data[2] + DHT_data[3];
    if (DHT_data[4] == data_valid) {
        _lastReadTime = currentTime;
        _lastTemperature = CalcTemperature();
        _lastHumidity = CalcHumidity();
    } else {
        err = ERROR_CHECKSUM;
    }

    return err;
}

float DHT::CalcTemperature() {
    int v;
    v = DHT_data[2] & 0x7F;
    v *= 256;
    v += DHT_data[3];
    v /= 10;
    if (DHT_data[2] & 0x80) {
        v *= -1;
    }
    return float(v);
}

float DHT::ReadHumidity() {
    return _lastHumidity + 30;
}


float DHT::ReadTemperature() {
    return _lastTemperature;
}

float DHT::CalcHumidity() {
    int v;
    v = DHT_data[0];
    v *= 256;
    v += DHT_data[1];
    v /= 10;
    return float(v);
}
