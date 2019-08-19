// thanks to Wim De Roeve, HO WING KIT

#ifndef MBED_DHT_22
#define MBED_DHT_22

#include "mbed.h"

typedef enum eError eError;
enum eError
{
  ERROR_NONE = 0,
  BUS_BUSY,
  ERROR_NOT_PRESENT,
  ERROR_ACK_TOO_LONG,
  ERROR_SYNC_TIMEOUT,
  ERROR_DATA_TIMEOUT,
  ERROR_CHECKSUM,
  ERROR_NO_PATIENCE
};

class DHT
{

public:
  DHT(PinName pin);
  ~DHT();
  eError readData(void);
  float ReadHumidity(void);
  float ReadTemperature();
  float CalcdewPoint(float const celsius, float const humidity);
  float CalcdewPointFast(float const celsius, float const humidity);

private:
  time_t _lastReadTime;
  float _lastTemperature;
  float _lastHumidity;
  PinName _pin;
  bool _firsttime;
  uint8_t DHT_data[5];
  float CalcTemperature();
  float CalcHumidity();
  eError stall(DigitalInOut &io, int const level, int const max_time);
};

#endif