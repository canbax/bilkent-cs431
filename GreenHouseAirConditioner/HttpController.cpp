#include "HttpController.h"
#include "globalVariable.h"

HttpController::HttpController(const char *rootPath, const char *path, TCPSocket *pTCPSocket)
    : HTTPRequestHandler(rootPath, path, pTCPSocket)
{
  // printf("httpCtr construtor");
  HttpController::_isRoofOpen = true;
}

// static variables
int HttpController::_humidity;
int HttpController::_temperature;
int HttpController::_fanRPS;
bool HttpController::_isRoofOpen;

HttpController::~HttpController()
{
  // printf("httpCtr destrutor");
}

void HttpController::doGet()
{
  int numberOfPaths = 5;
  string paths[] = {"/dht", "/fan", "/roof", "/reqTemp", "/reqHum"};
  string type = reqHeaders()["Content-Type"];
  string p = path();

  bool isResponded = false;
  for (int i = 0; i <  numberOfPaths; i++)
  {
    if (p.find(paths[i]) != string::npos)
    {
      printf(" get p: %d", i);
      char s[32];

      if (i == 0)
      {
        int size = sprintf(s, "{\"t\": %d,\"h\": %d}", HttpController::_temperature, HttpController::_humidity);
        printf("response: %s", s);
        respHeaders()["Access-Control-Allow-Origin"] = "*";
        respHeaders()["Connection"] = "close";
        writeData(s, size);
        isResponded = true;
      }
      else if (i == 1)
      {
        int size = sprintf(s, "%d", HttpController::_fanRPS);
        printf("response: %s", s);
        respHeaders()["Access-Control-Allow-Origin"] = "*";
        respHeaders()["Connection"] = "close";
        writeData(s, size);
        isResponded = true;
      }
      else if (i == 2)
      {
        int size = sprintf(s, "%d", HttpController::_isRoofOpen);
        printf("response: %s", s);
        respHeaders()["Access-Control-Allow-Origin"] = "*";
        respHeaders()["Connection"] = "close";
        writeData(s, size);
        isResponded = true;
      }
      else if (i == 3)
      {
        int size = sprintf(s, "%d", requiredTemperature);
        printf("response: %s", s);
        respHeaders()["Access-Control-Allow-Origin"] = "*";
        respHeaders()["Connection"] = "close";
        writeData(s, size);
        isResponded = true;
      }
      else if (i == 4)
      {
        int size = sprintf(s, "%d", requiredHumidity);
        printf("response: %s", s);
        respHeaders()["Access-Control-Allow-Origin"] = "*";
        respHeaders()["Connection"] = "close";
        writeData(s, size);
        isResponded = true;
      }
    }
  }

  if (!isResponded)
  {
    char buf[3] = {'4', '0', '4'};
    respHeaders()["Access-Control-Allow-Origin"] = "*";
    respHeaders()["Connection"] = "close";
    writeData(buf, 3);
  }
}

void HttpController::doPost()
{
  string type = reqHeaders()["Content-Type"];
  string p = path();
  int len = dataLen();
  printf("\nhttpCtr doPost path: %s, len: %d", p, len);

  char buf[32];
  if (p == "/t")
  {
    readData(buf, 32);
    printf("\n doPost buf = %s", buf);
    int i = atoi(buf);
    printf("\n doPost i = %d", i);
    requiredTemperature = i;
  }
  else if (p == "/h")
  {
    readData(buf, 32);
    printf("\n doPost buf = %s", buf);
    int i = atoi(buf);
    printf("\n doPost i = %d", i);
    requiredHumidity = i;
  }

  char buf2[3] = {'2', '0', '0'};
  respHeaders()["Access-Control-Allow-Origin"] = "*";
  respHeaders()["Connection"] = "close";
  writeData(buf2, 3);
}

void HttpController::doHead()
{
  // printf("\nhttpCtr doHead");
}

void HttpController::onReadable()
{
  char buf[32];
  readData(buf, 32);
  printf("\n onReadable buf = %s", buf);
  // printf("\nhttpCtr onReadable");
}

void HttpController::onWriteable()
{
  // printf("\nhttpCtr onWriteable");
}

void HttpController::onClose()
{
  // printf("\nhttpCtr onClose");
}

// static methods which syncs state of device with server variables
void HttpController::setTemperatureAndHumidity(int t, int h)
{
  HttpController::_humidity = h;
  HttpController::_temperature = t;
}

void HttpController::setFanRps(int s)
{
  HttpController::_fanRPS = s;
}

void HttpController::setRoofState(bool r)
{
  HttpController::_isRoofOpen = r;
}
