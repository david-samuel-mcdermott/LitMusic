#include <FastLED.h>

#pragma GCC push_options
#pragma GCC optimize ("unroll-loops")

CRGB leds[50];
CRGBPalette16 palette;
TBlendType blending;

const int numsamps = 20;
short i = 0;
short samples[numsamps] = {0};
float sum = 0;
volatile bool multicolor = false;
long theT = millis();

void setup() {
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
  pinMode(1, OUTPUT);
  digitalWrite(1, LOW);
  pinMode(2, INPUT);
  multicolor = digitalRead(2);
  attachInterrupt(2, jmpMode, CHANGE);
  FastLED.addLeds<WS2811, 8, GRB>(leds, 50).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(40);
  palette = RainbowColors_p;
  blending = LINEARBLEND;
}

void jmpMode(){
  multicolor = digitalRead(2);
}

__attribute__((optimize("unroll-loops")))
void loop() {
  static int idx;
  sum -= samples[i];
  samples[i] = digitalRead(9);
  sum += samples[i];
  float total = sum/numsamps;
  
  if(!multicolor){
    if (millis()-theT < 15){
      goto show;
    }
    idx++;
    for(int j = 0; j < 50; j++){
      leds[j] = ColorFromPalette(palette, idx, 255, blending);
    }
    FastLED.setBrightness(total*255);
  } else {
    if (millis()-theT < 10){
      goto show;
    }
    idx+=10;
    for(int j = 49; j >=0; j--){
      leds[j] = leds[j-1];
    }
    leds[0] = ColorFromPalette(palette, idx, 255, blending);
    FastLED.setBrightness(total*255);
  }
  theT = millis();
  show:;
    
  FastLED.show();
  i = ++i%numsamps;
}

#pragma GCC pop_options

