#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "wifi_manager.h"
#include "sensor.h"
#include "defines.h"


extern ESP8266WebServer server;
extern bool wifiStaConnectedtoAP;
bool isMQTTSetServer = false;

String Timestamp_buffer;
// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

WiFiClient espClient;
PubSubClient client(espClient);
String msgToPublish;
//===============================================================
//              Create json and serialize the message 
//===============================================================
void CreateJson(String temperature,String humidity, String pth,String &seriazlied_message) 
{

  StaticJsonDocument<CHAR_MAX> doc;
  JsonObject object = doc.to<JsonObject>();
  
  
 if (seriazlied_message)
 {
    object["deviceid"] = BD_PTH_SENSOR_DEVICE_ID;
    object["timestamp"] = Timestamp_buffer;
    object["temp"] = temperature;
    object["humidity"] = humidity;
    object["pm"] = pth;
    serializeJson(doc, seriazlied_message);  
 }
     
}

//===============================================================
//                  Reconnect
//===============================================================
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
  {
      Serial.print("Attempting MQTT connection...");
      // Attempt to connect
      if (client.connect(BD_MQTT_CLIENT_ID)) 
      {
        Serial.println("connected");
        // Once connected, publish an announcement...
        client.publish(BD_MQTT_PUBLISH_TOPIC , BD_MQTT_CLIENT_ID);
        // ... and resubscribe
        // Initialize a NTPClient to get time
        timeClient.begin();
      } else 
      {
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(BD_MQTT_RECONNECT_DELAY);
      }
  }
}

void logDebug(char *str,...)
{
    
}
//===============================================================
//                 Date Time Update from NTP 
//===============================================================

void DateTimeUpdate()
{
   while(!timeClient.update())
  {
      timeClient.forceUpdate();
  }
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  
  formattedDate = timeClient.getFormattedDate();
  
  // Extract date
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
  
  // Extract time
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  
  Timestamp_buffer = dayStamp +"  "+timeStamp;
}

void setup() 
{
      Serial.begin(BD_SERIAL_BAUD_RATE);
      Serial.println("Configuring...");
      Serial.println("In AP mode");
      //WiFi.mode(WIFI_AP);           //Only Access point
      Serial.println(WiFi.softAP(BD_WIFI_AP_SSID, BD_WIFI_AP_PASSWORD)?"Ready":"failed");  //Start HOTspot removing password will disable security

      IPAddress myIP = WiFi.softAPIP(); //Get IP address
      Serial.print("HotSpt IP:");
      Serial.println(myIP);
    

      server.on("/", handleRoot);      //Which routine to handle at root location
      server.on("/login", HTTP_POST, handleLogin);

      server.begin();                  //Start server
      Serial.println("HTTP server started");
      
      // Initialize a NTPClient to get time
      timeClient.begin();

      // Set offset time in seconds to adjust for your timezone, for example:
      // GMT +1 = 3600
      // GMT +8 = 28800
      // GMT -1 = -3600
      // GMT 0 = 0
      timeClient.setTimeOffset(TIMEZONE_OFFSET);
    
}
void loop() 
{
    
      delay(BD_AURDINO_LOOP_DELAY);
      server.handleClient(); 
      Serial.print("Num of stations connected to AP ");
      Serial.println(WiFi.softAPgetStationNum());
      
   
    if (!wifiStaConnectedtoAP)
    {
      //Serial.print("Trying to connect...AP IP is ");
      //Serial.println(myIP);
      return;
    }
    
    if (!isMQTTSetServer)
    {
      client.setServer(BD_MQTT_SERVER_HOSTNAME, BD_MQTT_SERVER_PORT);
      isMQTTSetServer = TRUE;
    }
      
    if (!client.connected()) 
    {
      reconnect();
    }
    client.loop();
    msgToPublish.clear();

    DateTimeUpdate();

    CreateJson(getSensorTemperature(),getSensorHumidity(),getSensorParitcleCount(),msgToPublish);
     
    client.publish(BD_MQTT_PUBLISH_TOPIC , msgToPublish.c_str());
    
    delay(BD_AURDINO_LOOP_DELAY);
}