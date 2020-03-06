#include "sensor.h"

//===============================================================
//                  Get Sensor Temperature
//===============================================================
String getSensorTemperature() {
  
  long temperature = random(5, 100);
  return String(temperature);
}
//===============================================================
//                  Get Sensor Humidity
//===============================================================
String getSensorHumidity() {

  long humidity = random(5, 100);
  return String(humidity);
  
}
//===============================================================
//                  Get Sensor Particle Count
//===============================================================
String getSensorParitcleCount() {
  long phcount=random(1, 10);
  return String(phcount);
}