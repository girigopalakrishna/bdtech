#ifndef __DEFINES_H__
#define __DEFINES_H__
//SERIAL
#define BD_SERIAL_BAUD_RATE       9600

//MQTT
#define BD_MQTT_SERVER_HOSTNAME   "broker.mqtt-dashboard.com"
#define BD_MQTT_SERVER_PORT       1883
#define BD_MQTT_CLIENT_ID         "PTH_SENSOR"
#define BD_MQTT_PUBLISH_TOPIC     "SENSOR/PTH"
#define BD_MQTT_RECONNECT_DELAY   5000

//WIFI
#define BD_WIFI_AP_SSID           "ESPWebServer"
#define BD_WIFI_AP_PASSWORD       "12345678"

//delays
#define BD_AURDINO_LOOP_DELAY       1000

//Device
#define BD_PTH_SENSOR_DEVICE_ID    "XYZ"

//Server
#define BD_WEB_SERVER_PORT          80

#define TRUE                        1

//NTP Server
#define TIMEZONE_OFFSET             19800

#endif