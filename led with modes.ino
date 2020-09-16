#include <FastLED.h>
#define LED_PIN     2
#define NUM_LEDS    100
#define BRIGHTNESS  255
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

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
}


void loop()
{
  //mode++;
  if(mode>10)
    {mode=0;}
    ChangePalettePeriodically();
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; /* motion speed */
    FillLEDsFromPaletteColors( startIndex);
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
if (digitalRead(4)==LOW)
      {

      mode=mode+1;
      Serial.print("__HIGH__");      
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
