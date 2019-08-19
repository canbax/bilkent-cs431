#ifndef HTTP_CONTROLLER_H
#define HTTP_CONTROLLER_H

#include "HTTPRequestHandler.h"

class HttpController : public HTTPRequestHandler
{

private:
  static int _temperature;
  static int _humidity;
  static int _fanRPS;
  static bool _isRoofOpen;

public:
  HttpController(const char *rootPath, const char *path, TCPSocket *pTCPSocket);
  virtual ~HttpController();

  //protected:
  static inline HTTPRequestHandler *inst(const char *rootPath, const char *path, TCPSocket *pTCPSocket) { return new HttpController(rootPath, path, pTCPSocket); } //if we ever could do static virtual functions, this would be one

  virtual void doGet();
  virtual void doPost();
  virtual void doHead();

  virtual void onReadable();  //Data has been read
  virtual void onWriteable(); //Data has been written & buf is free
  virtual void onClose();     //Connection is closing
  
  static void setTemperatureAndHumidity(int t, int h);
  static void setFanRps(int s);
  static void setRoofState(bool r);
};

#endif
