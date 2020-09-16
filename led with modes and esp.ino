//esp
#include <Arduino.h>
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif
#include <ESPAsyncWebServer.h>
#include "fauxmoESP.h"

// Rename the credentials.sample.h file to credentials.h and
#define WIFI_SSID "WI-FI I"
#define WIFI_PASS "846239+846239"
// edit it according to your router configuration
//#include "credentials.h"

fauxmoESP fauxmo;
AsyncWebServer server(80);

void wifiSetup() {

// Set WIFI module to STA mode
WiFi.mode(WIFI_STA);

// Connect
Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
WiFi.begin(WIFI_SSID, WIFI_PASS);

// Wait
while (WiFi.status() != WL_CONNECTED) {
Serial.print(".");
delay(100);
}
Serial.println();

// Connected!
Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());

}

void serverSetup() {

// Custom entry point (not required by the library, here just as an example)
server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request){
request->send(200, "text/plain", "Hello, world");
});

// These two callbacks are required for gen1 and gen3 compatibility
server.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
if (fauxmo.process(request->client(), request->method() == HTTP_GET, request->url(), String((char *)data))) return;
// Handle any other body request here...
});
server.onNotFound([](AsyncWebServerRequest *request) {
String body = (request->hasParam("body", true)) ? request->getParam("body", true)->value() : String();
if (fauxmo.process(request->client(), request->method() == HTTP_GET, request->url(), body)) return;
// Handle not found request here...
});

// Start the server
server.begin();

}
//esp
#include <FastLED.h>
#define LED_PIN     2
#define NUM_LEDS    100
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

String c="";
int mode=0;
CRGB leds[NUM_LEDS];
#define UPDATES_PER_SECOND 100
CRGBPalette16 currentPalette;
TBlendType    currentBlending;
extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

void setup() {
    delay( 3000 ); // power-up safety delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;

pinMode(4,  INPUT);
pinMode(12,  INPUT);
Serial.begin(9600);
digitalWrite(4, HIGH);
pinMode(12,  INPUT);
//esp
wifiSetup();

// Web server
serverSetup();

// Set fauxmoESP to not create an internal TCP server and redirect requests to the server on the defined port
// The TCP port must be 80 for gen3 devices (default is 1901)
// This has to be done before the call to enable()
fauxmo.createServer(false);
fauxmo.setPort(80); // This is required for gen3 devices

// You have to call enable(true) once you have a WiFi connection
// You can enable or disable the library at any moment
// Disabling it will prevent the devices from being discovered and switched
fauxmo.enable(true);

// You can use different ways to invoke alexa to modify the devices state:
// "Alexa, turn kitchen on" ("kitchen" is the name of the first device below)
// "Alexa, turn on kitchen"
// "Alexa, set kitchen to fifty" (50 means 50% of brightness)

// Add virtual devices
fauxmo.addDevice("next");
fauxmo.addDevice("previous");

// You can add more devices
fauxmo.addDevice("curtains");
fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {

// Callback when a command from Alexa is received.
// You can use device_id or device_name to choose the element to perform an action onto (relay, LED,...)
// State is a boolean (ON/OFF) and value a number from 0 to  (if you say "set kitchen switch to 50%" you will receive a 128 here).
// Just remember not to delay too much here, this is a callback, exit as soon as possible.
// If you have to do something more involved here set a flag and process it in your main loop.

//if (0 == device_id) digitalWrite(AA, state);
// if (1 == device_id) digitalWrite(BB, state);
// if (2 == device_id) analogWrite(CC, value);

Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);

// For the example we are turning the same LED on and off regardless fo the device triggered or the value

//digitalWrite(LED, !state); // we are nor-ing the state because our LED has inverse logic.
if (strcmp(device_name, "next")==0) {
if(mode<11){mode=mode+1;}
else{mode=1;}
} else if (strcmp(device_name, "previous")==0) {
if(mode!=0){mode=mode-1;}
else{
  mode=11;
  }
} else if (strcmp(device_name, "curtains")==0) {
mode=0;
} });
//esp
}


void loop()
{
  //esp
fauxmo.handle();
  //esp
  //mode++;
  if(mode>11)
    {mode=1;}
    ChangePalettePeriodically();
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    FillLEDsFromPaletteColors( startIndex);
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
if (digitalRead(4)==LOW)
      {
if(c!="LOW"){
      mode=mode+1;
      Serial.print("__HIGH__");
      c="LOW"; 
}     
      }
      else
      {
        c="HIGH";
        }
           Serial.print("__MODE=");
      Serial.print(mode);
      Serial.println("__");
delay(5);
}
  

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 255;
   for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}
void ChangePalettePeriodically()
{
    uint8_t secondHand = (millis() / 1000) % 60;
    static uint8_t lastSecond = 99;
    
    
 //   if( lastSecond != secondHand) {
  //      lastSecond = secondHand;
        if( mode ==  0)  {SetupTotallyoff();}
        
        if( mode ==  1)  { currentPalette = RainbowColors_p;         currentBlending = LINEARBLEND; }
        if( mode ==  2)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
        if( mode ==  3)  { currentPalette = RainbowStripeColors_p;   currentBlending = LINEARBLEND; }
        if( mode ==  4)  { SetupPurpleAndGreenPalette();             currentBlending = LINEARBLEND; }
        if( mode ==  5)  { SetupTotallyRandomPalette();              currentBlending = LINEARBLEND; }
        if( mode ==  6)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
        if( mode ==  7)  { SetupBlackAndWhiteStripedPalette();       currentBlending = LINEARBLEND; }
        if( mode ==  8)  { currentPalette = CloudColors_p;           currentBlending = LINEARBLEND; }
        if( mode ==  9)  { currentPalette = PartyColors_p;           currentBlending = LINEARBLEND; }
        if( mode ==  10)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
        if( mode ==  11)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = LINEARBLEND; }
  //  }
}
void SetupTotallyRandomPalette()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( random8(), 255, random8());
    }
}
void SetupTotallyoff()
{
    for( int i = 0; i < 16; i++) {
        currentPalette[i] = CHSV( 0, 0, 0);
    }
}
void SetupBlackAndWhiteStripedPalette()
{
    fill_solid( currentPalette, 16, CRGB::Black);
    currentPalette[0] = CRGB::White;
    currentPalette[4] = CRGB::White;
    currentPalette[8] = CRGB::White;
    currentPalette[12] = CRGB::White;
}
void SetupPurpleAndGreenPalette()
{
    CRGB purple = CHSV( HUE_PURPLE, 255, 255);
    CRGB green  = CHSV( HUE_GREEN, 255, 255);
    CRGB black  = CRGB::Black;
    currentPalette = CRGBPalette16(
                                   green,  green,  black,  black,
                                   purple, purple, black,  black,
                                   green,  green,  black,  black,
                                   purple, purple, black,  black );
}
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray, 
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Red,
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black,
    CRGB::Black
};
