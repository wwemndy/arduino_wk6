/*
  WiFi Web Server LED Blink
  A simple web server that lets you blink an LED via the web.
  This sketch will print the IP address of your WiFi Shield (once connected)
  to the Serial monitor. From there, you can open that address in a web browser
  to turn on and off the LED on pin 9.
  If the IP address of your shield is yourAddress:
  http://yourAddress/H turns the LED on
  http://yourAddress/L turns it off
  This example is written for a network using WPA encryption. For
  WEP or WPA, change the Wifi.begin() call accordingly.
  Circuit:
  * LinkIt 7697 HDK
  * LED attached to pin 9
  created 25 Nov 2012
  by Tom Igoe
  modified 23 May 2017
  by MediaTek Labs
*/
#include <LWiFi.h>

char ssid[] = "103-1";      //  your network SSID (name)
char pass[] = "ntuedorm";   // your network password
int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(80);

String website;

void setup() {
  Serial.begin(9600);      // initialize serial communication
  pinMode(LED_BUILTIN, OUTPUT);      // set the LED pin mode

  buildWeb();

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
  }
  server.begin();                           // start the web server on port 80
  printWifiStatus();                        // you're connected now, so print out the status
}


void loop() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print(website);

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.startsWith("GET /H")) {
          digitalWrite(LED_BUILTIN, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.startsWith("GET /L")) {
          digitalWrite(LED_BUILTIN, LOW);                // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}


void buildWeb(){
  website +="<!DOCTYPE html>" ;
  website += "<html>";
   website += "<head>";
   website += "<meta charset=\"utf-8\">";
   website += "<title>IOT Controler</title>";
   website += "<link href=\"https://fonts.googleapis.com/css?family=Caveat\" rel=\"stylesheet\">";
   website += "<style media=\"screen\">";
   website += "*{font-family: Caveat;font-size: 45px;box-sizing: border-box;}";
   website += ":link, :visited{color: #fe7701;text-decoration: none;}";
   website += "body{background-image: url(img/bg.jpg);background-repeat: no-repeat;background-size: cover;}";
   website += "main{width: 700px;height: 250px;padding: 10px;margin: 40px auto;}";
   website += ".btn_div{width: 300px;margin: 20px auto;}";
   website += ".botton{display: inline;width: 110px;padding: 0px 30px;margin: 10px auto;background-color: rgba(255, 255, 255, 0.8);";
   website += "  border-radius: 3px;box-shadow: 5px 5px rgba(80, 80, 80, 0.3);}";
   website += "#text{width: 600px;height: 100px;margin: 10px auto;padding: 0px 40px;color: rgb(75, 75, 75);}";
   website += "#on{float: left;color: rgb(22, 174, 153);}";
   website += "#off{float: right;color: rgb(236, 80, 80);}";
   website += "</style>";
   website += "</head>";
   website += "<body>";
   website += "<main>";
   website += "<div id=\"text\">Click Here to control your LED :)</div>";
   website += "<div class=\"btn_div\">";
   website += "<a href=\"/H\" class=\"botton\" id=\"on\">On</a>";
   website += "<a href=\"/L\" class=\"botton\" id=\"off\">Off</a>";
   website += "</div>";
   website += "</main>";
   website += "</body>";
   website += "</html>";
 }
