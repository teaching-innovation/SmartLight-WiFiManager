#define FASTLED_ALLOW_INTERRUPTS 0 //  https://github.com/FastLED/FastLED/issues/306#issuecomment-219868579

#include "FastLED.h"          // by Daniel Garcia V 3.3.3
#include <WiFiManager.h>          //by tzapu,tablatronix V 2.0.3  https://github.com/tzapu/WiFiManager

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <ESP8266mDNS.h>          
#include <WiFiClient.h>          
#include <DNSServer.h>         
#include <ESP8266WebServer.h>          

#define CHIPSET    WS2812 // WS2811
#define NUM_LEDS    9 //number of LEDS in strip
#define DATA_PIN    D3
struct CRGB leds[NUM_LEDS];
#define COLOR_ORDER GRB //BRG

//WiFiServer server(80);
ESP8266WebServer server(80);

const String HOMEPAGE = "<form action=\"/cmd1\" method=\"get\"><button class=\"button1\">White</button></form><br/><form action=\"/cmd2\" method=\"get\"><button class=\"button2\">Red</button></form><br/> <form action=\"/cmd3\" method=\"get\"><button class=\"button3\">Blue Chase</button></form><br/><form action=\"/cmd4\" method=\"get\"><button class=\"button4\">Off</button></form><br/>    ";
const char HTTP_HEADER1[] PROGMEM          = "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/><title>{v}</title>";
const char HTTP_SCRIPT1[] PROGMEM          = "<script>function c(l){document.getElementById('s').value=l.innerText||l.textContent;document.getElementById('p').focus();}</script>";
const char HTTP_STYLE1[] PROGMEM           = "<style>.c{text-align: center;} div,input{padding:5px;font-size:1em;} input{width:95%;} body{text-align: center;font-family:verdana;} button{border:0;border-radius:0.3rem;background-color:#1fa3ec;color:#fff;line-height:2.4rem;font-size:1.2rem;width:100%;} .q{float: right;width: 64px;text-align: right;} .l{background: url(\"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAMAAABEpIrGAAAALVBMVEX///8EBwfBwsLw8PAzNjaCg4NTVVUjJiZDRUUUFxdiZGSho6OSk5Pg4eFydHTCjaf3AAAAZElEQVQ4je2NSw7AIAhEBamKn97/uMXEGBvozkWb9C2Zx4xzWykBhFAeYp9gkLyZE0zIMno9n4g19hmdY39scwqVkOXaxph0ZCXQcqxSpgQpONa59wkRDOL93eAXvimwlbPbwwVAegLS1HGfZAAAAABJRU5ErkJggg==\") no-repeat left center;background-size: 1em;}</style>";
const char HTTP_HEADER_END1[] PROGMEM        = "</head><body><div style='text-align:left;display:inline-block;min-width:260px;'>";

//for LED status
#include <Ticker.h>
Ticker ticker;

int lightProgram = 0;
int buttonPin = D4;
int buttonState = 0;

void tick()
{
  //toggle state
  int state = digitalRead(BUILTIN_LED);  // get the current state of GPIO1 pin
  digitalWrite(BUILTIN_LED, !state);     // set pin to the opposite state
  if (state == true) {
    leds[0] = CRGB::Green;
  } else {
    leds[0] = CRGB::Black;
  }
  FastLED.show();
}

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
  ticker.attach(0.2, tick);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  //set led pin as output
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(buttonPin, INPUT);

  FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);

  // start ticker with 0.5 because we start in AP mode and try to connect
  ticker.attach(0.6, tick);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  
  WiFiManager wm;
  //reset settings - wipe credentials for testing
  //wm.resetSettings();

  WiFiManagerParameter custom_text("<p> Go to http://smartlight.local when light turns blue</p>");
  wm.addParameter(&custom_text);

  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wm.setAPCallback(configModeCallback);

//  if (!wm.autoConnect("SmartLightABHS2020")) {
  if (!wm.autoConnect()) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  leds[0] = CRGB::Blue;

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //   the fully-qualified domain name is "smartlight.local"
  if (!MDNS.begin("smartlight")) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 80);

  server.on("/", updateWeb);
  server.on("/cmd1", cmd1);
  server.on("/cmd2", cmd2);
  server.on("/cmd3", cmd3);
  server.on("/cmd4", cmd4);

  server.begin();
  Serial.println("HTTP server started");

  ticker.detach();

  //turn off builtin LED
  digitalWrite(BUILTIN_LED, HIGH);
}

void loop() {
  server.handleClient();
  MDNS.update();

  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW) {
    lightProgram += 1;
    Serial.print("light Program:");
    Serial.println(lightProgram);
    delay(500);
  }

  switch (lightProgram) {
    case 1:
      lightProgram1();      //do something when var equals 1
      break;
    case 2:
      lightProgram2();      //do something when var equals 2
      break;
    case 3:
      lightProgram3();      //do something when var equals 2
      break;
    case 4:
      lightProgram4();      //do something when var equals 2
      break;
    default:
      lightProgram = 0;    // if nothing else matches, do the default
      break;
  }

  FastLED.show();
}



//void handleRoot() {
//  updateWeb();
//}

void  updateWeb() {
  String ipAddressURL = "<a href=\"http://";
  ipAddressURL += WiFi.localIP().toString();
  ipAddressURL += "\">Goto IP Address</a>";

  String page = FPSTR(HTTP_HEADER1);
  page += FPSTR(HTTP_STYLE1);
  page += FPSTR(HTTP_SCRIPT1);
  page += FPSTR(HTTP_HEADER_END1);
  page += "<H3>My Smart Light </H3> ";
  page += HOMEPAGE;
  page += "<p>IP Addresss: ";
  page += WiFi.localIP().toString();
  page += "<br>";
  page += ipAddressURL;
  page += "</p>";
  server.send(200, "text/html", page);
}

//handle html button presses to change lightProgram variable
void cmd1() {
  lightProgram = 1;
  updateWeb();
}
void cmd2() {
  lightProgram = 2;
  updateWeb();
}
void cmd3() {
  lightProgram = 3;
  updateWeb();
}
void cmd4() {
  lightProgram = 4;
  updateWeb();
}


void lightProgram1() {
  FastLED.setBrightness(255);
  for (int dot = 0; dot < NUM_LEDS; dot++) {
    leds[dot] = CRGB::White;
  }
}

void lightProgram2() {
  FastLED.setBrightness(25);
  for (int dot = 0; dot < NUM_LEDS; dot++) {
    leds[dot] = CRGB::Red;
  }
}

void lightProgram3() {
  FastLED.setBrightness(255);
  for (int dot = 0; dot < NUM_LEDS; dot++) {
    leds[dot] = CRGB::Blue;
    FastLED.show();
    // clear this led for the next time around the loop
    leds[dot] = CRGB::Black;
    delay(100);
    FastLED.show();
  }
}

void lightProgram4() {
  for (int dot = 0; dot < NUM_LEDS; dot++) {
    leds[dot] = CRGB::Black;
  }
}
