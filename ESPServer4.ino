#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <HardwareSerial.h>
#define PIN        19 
#define NUMPIXELS 120  

int musicState;
 
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Replace with your network credentials
const char* ssid     = "foo";
const char* password = "bar";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

HardwareSerial espSerial(1);

// Auxiliar variables to store the current output state

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(115200);
  espSerial.begin(9600,SERIAL_8N1,16,17);
  // Initialize the output variables as outputs
  // Set outputs to LOW
  pixels.begin();
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  IPAddress local_IP(10, 0, 0, 10);//sets static IP address
  // Set your Gateway IP address
  IPAddress gateway(10, 0, 0, 1);
  IPAddress subnet(255, 255, 0, 0);
  IPAddress primaryDNS(8, 8, 8, 8);   
  IPAddress secondaryDNS(8, 8, 4, 4);
  
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /26/on") >= 0) {
              for(int j = 0; j<NUMPIXELS; j++){
                for(int i = 0; i<j+10; i++) {
                  pixels.setPixelColor(i, pixels.Color(0, 0, 220));
                }
                pixels.setPixelColor(j-1, pixels.Color(0,0,0));
                if(j == NUMPIXELS){ 
                  j = 0;
                }
                 pixels.show();
                 if(musicState == 1){
                  for(int i = 0; i <10; i++){
                  espSerial.write('c');
                }
                }
                 
             }
            } else if (header.indexOf("GET /27/on") >= 0) {
          
             for(int j = 0; j<NUMPIXELS; j++){
                for(int i = 0; i<j+10; i++) {
                  pixels.setPixelColor(i, pixels.Color(255,140,0));
                }
                pixels.setPixelColor(j-1, pixels.Color(0,0,0));
                if(j == NUMPIXELS){ 
                  j = 0;
                }
              pixels.show();
             }
             for(int i = 0; i <10; i++){
                  espSerial.write('d');
                }
             if(musicState == 1){
                  for(int i = 0; i <10; i++){
                  espSerial.write('e');
                }
             }
            }else if (header.indexOf("GET /28/on") >= 0) {
             
              for(int j = 0; j<NUMPIXELS; j++){
                for(int i = 0; i<j+10; i++) {
                  pixels.setPixelColor(i, pixels.Color(128, 0, 128));
                }
                pixels.setPixelColor(j-1, pixels.Color(0,0,0));
                if(j == NUMPIXELS){ 
                  j = 0;
                }
              pixels.show();
              }
            }else if (header.indexOf("GET /29/on") >= 0) {
    
              for(int i=0; i<NUMPIXELS*1; i++) {
                pixels.setPixelColor(i, pixels.Color(128, 0, 128));
              }for(int i=0; i<NUMPIXELS*0.8; i++) {
                pixels.setPixelColor(i, pixels.Color(0, 0, 128));
              }for(int i=0; i<NUMPIXELS*0.6; i++) {
                pixels.setPixelColor(i, pixels.Color(128, 0, 0));
              }for(int i=0; i<NUMPIXELS*0.4; i++) {
                pixels.setPixelColor(i, pixels.Color(128, 128, 0));
              }for(int i=0; i<NUMPIXELS*0.2; i++) {
                pixels.setPixelColor(i, pixels.Color(220, 150, 0));
              }
             pixels.show();
             if(musicState == 1){
                  for(int i = 0; i <10; i++){
                  espSerial.write('a');
                }
                }
             
            } else if (header.indexOf("GET /30/off") >= 0) {
              for(int i=0; i<NUMPIXELS; i++) {
                pixels.setPixelColor(i, pixels.Color(0, 0, 0));
              }
              for(int i = 0; i <10; i++){
                  espSerial.write('b');
                }
              pixels.show();
            }

            if (header.indexOf("GET /31/off") >= 0) {
                digitalWrite(13, !digitalRead(13));
                musicState = digitalRead(13);
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #FF8C00; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}");
            client.println(".button3 {background-color: #0000FF;}");
            client.println(".button4 {background-color: #800080;}");
            client.println(".button5 {background-image: linear-gradient(to right, yellow, green, red, blue, purple);}</style></head>");
            // Web Page Heading
            client.println("<body><h1>LED Control</h1>");
  
            client.println("<p><a href=\"/26/on\"><button class=\"button button3\">BLUE</button></a></p>");
            client.println();
            client.println("<p><a href=\"/27/on\"><button class=\"button\">FIRE</button></a></p>");            
            client.println();
            client.println("<p><a href=\"/28/on\"><button class=\"button button4\">PURPLE</button></a></p>");
            client.println();
            client.println("<p><a href=\"/29/on\"><button class=\"button button5\">RAINBOW</button></a></p>");     
            client.println();   
            client.println("<p><a href=\"/30/off\"><button class=\"button button2\">OFF</button></a></p>");
            client.println(); 
            client.println("<p><a href=\"/31/off\"><button class=\"button button2\">Music</button></a></p>");
            client.println("</body></html>");
            
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
