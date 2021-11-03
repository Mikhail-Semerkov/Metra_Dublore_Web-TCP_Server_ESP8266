#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>
#include "GyverTimer.h"   
#include <ESP8266mDNS.h>
#include "index.h"

const int packageSize = 15;             
char buffer[packageSize];               


const char* ssid = "indconf2002";
const char* password = "07031968200703196820123456";

ESP8266WebServer WebServer(80);
SoftwareSerial RS485(D7, D6);

MDNSResponder MDNS;        //назначаем экземпляр класса ESP8266mDNS


#define MAX_SRV_CLIENTS 10
int Port_Server = 8266;


WiFiServer Server(Port_Server);
WiFiClient ServerClients[MAX_SRV_CLIENTS];

int i;

String Weight_Buffer;
String Full_Buffer;

GTimer Timer_DataOK(MS);                


void Handle_Root() 
{
 String s = webpage;
 WebServer.send(200, "text/html", s);
 
}

void Weight_Stable() 
{
  
 String stable_value = "";
  
 if (buffer[9] == 0x20)
      {
       stable_value = "STABLE";
      }

    if (buffer[9] == 0x3F)
      {
      stable_value = "----------";
    }
   WebServer.send(200, "text/plane", stable_value);
}

void Weight_Data()
{
 WebServer.send(200, "text/plane", Weight_Buffer);
}

void Button_Control() 
{
 String act_state = WebServer.arg("state");
 if(act_state == "1")
 { 
  RS485.write("A");
  delay(250);
  act_state = "";
 }
}



void setup(void)
{
  

  

  Timer_DataOK.setTimeout(5000);         

  i = 0;

  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 
  WebServer.on("/", Handle_Root);
  WebServer.on("/led_set", Button_Control);
  WebServer.on("/adcread", Weight_Data);
  WebServer.on("/stable", Weight_Stable);
  WebServer.begin();

  Server.begin();
  RS485.begin(9600);

  if (MDNS.begin("esp8266", WiFi.localIP())) 
  {
    Serial.println("MDNS responder started");
  }

  MDNS.addService("http", "tcp", 80);
}

void loop(void)
{
  if(WiFi.status() == WL_CONNECTED)
  {
    WebServer.handleClient();


    if(RS485.available())
    { 
      RS485.readBytes(buffer, packageSize);

      for(i = 0; i < MAX_SRV_CLIENTS; i++)
      {
        if (ServerClients[i] && ServerClients[i].connected())
        {
          ServerClients[i].print(Full_Buffer);     
        }
      }

      if (buffer[13] == 0x0D && buffer[14] == 0x0A)
      {
        Full_Buffer = String(buffer[0]) + String(buffer[1]) + String(buffer[2]) + String(buffer[3]) + String(buffer[4]) + String(buffer[5]) + String(buffer[6]) + String(buffer[7]) + String(buffer[8]) + String(buffer[9]) + String(buffer[10]) + String(buffer[11]) + String(buffer[12]) + String(buffer[13]) + String(buffer[14]);
        Weight_Buffer = String(buffer[2]) + String(buffer[3]) + String(buffer[4]) + String(buffer[5]) + String(buffer[6]) + String(buffer[7]) + String(buffer[8]);
        Weight_Buffer.trim();

        Timer_DataOK.reset();
        Timer_DataOK.start();

      }
      if (buffer[13] != 0x0D && buffer[14] != 0x0A)
      {
        Weight_Buffer = "";
        RS485.end();
        delay(500);
        RS485.begin(9600);
        
      }
    }

    if (Server.hasClient())
    {
      for(i = 0; i < MAX_SRV_CLIENTS; i++)
      {
        if (!ServerClients[i] || !ServerClients[i].connected())
        {
          if(ServerClients[i]) ServerClients[i].stop();
          {
            ServerClients[i] = Server.available();
            continue;
          }
        }
      }
      WiFiClient ServerClient = Server.available();
      ServerClient.stop();
    }

    for(i = 0; i < MAX_SRV_CLIENTS; i++)
    {
     if (ServerClients[i] && ServerClients[i].connected())
      {
        if(ServerClients[i].available())
        {
         while(ServerClients[i].available())
         {
         RS485.print(ServerClients[i].read());
         break;
         }
         
        }

      }
    }

      if (Timer_DataOK.isReady())
      {
        Timer_DataOK.start();
        Full_Buffer = "ERROR";
        Weight_Buffer = "ERROR";
      }
  }
}