#include "wifi_manager.h"
#include "defines.h"

//String client_ssid = "";
//String client_password = "";

ESP8266WebServer server(BD_WEB_SERVER_PORT); //Server on port 80
bool wifiStaConnectedtoAP = false;

//===============================================================
//              Print WiFi Status
//===============================================================

void printWifiStatus() {
  long rssi;
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

//===============================================================
//              Setting up WiFi Client
//===============================================================

void setupWiFiClient(String ssid,String password)
{
   int status;
   int trialCount = 0;
   status = WL_IDLE_STATUS;
   if (WiFi.status() == WL_NO_SHIELD) 
   {
        Serial.println("WiFi shield not present");
        // don't continue:
        while (true);
   }
  Serial.println(ssid);
  Serial.println(password);

  WiFi.begin(ssid, password);
  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) 
  {
        Serial.print("Attempting to connect to SSID: "); 
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        Serial.println(status);
        // wait 10 seconds for connection:
        delay(10000);
        trialCount++;
        if (trialCount > 4)
        {
          Serial.println("Could not connect to WiFi...");
          break;
        }
        
  }
  server.stop();
  server.on("/", handleRoot);      //Which routine to handle at root location
  server.on("/login", HTTP_POST, handleLogin);                 
  server.begin();
  server.handleClient();
  // you're connected now, so print out the status:
  printWifiStatus();
  if (WiFi.localIP())
  {
    wifiStaConnectedtoAP=TRUE;
  }
}

void handleRoot() {                          // When URI / is requested, send a web page with a button to toggle the LED
  server.send(200, "text/html", "<form action=\"/login\" method=\"POST\"></br></br> \
  <p color=\"blue\"><h1> WiFi Configuration</h1></p> \
  <input type=\"text\" name=\"SSID\" placeholder=\"SSID\"></br> \
  <p><input type=\"password\" name=\"password\" placeholder=\"Password\"></p></br> \
  <input type=\"submit\" value=\"Configure\"></form>");
}

//===============================================================
//              Handle Web Page to configure Station IP and Password
//===============================================================
void handleLogin() {                         // If a POST request is made to URI /login
  String client_ssid,client_password;
  if( ! server.hasArg("SSID") || ! server.hasArg("password") 
      || server.arg("SSID") == NULL || server.arg("password") == NULL) { // If the POST request doesn't have username and password data
    server.send(400, "text/plain", "400: Invalid Request");         // The request is invalid, so send HTTP status 400
    return;
  }
  //if(server.arg("SSID") == "varun" && server.arg("password") == "naidu143") { // If both the username and the password are correct
  if (server.arg("SSID").length() != 0 || server.arg("password").length() != 0) {
    server.send(200, "text/html", "<h1>Hurrah!!, " + server.arg("username") + "!</h1><p>WiFi Configured successfully!!</p>");
    client_ssid = server.arg("SSID");
    client_password = server.arg("password");
    //server.stop();
    setupWiFiClient(client_ssid,client_password);
  } else {                                                                              // Username and password don't match
    server.send(401, "text/plain", "401: Empty SSID/Password found. Please reconfigure");
  }
}
