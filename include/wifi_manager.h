
#ifndef __WIFI_MANAGER_H__
#define __WIFI_MANAGER_H__
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>


void printWifiStatus(void);
void setupWiFiClient(void);
void handleRoot(void);
void handleLogin(void);
#endif
