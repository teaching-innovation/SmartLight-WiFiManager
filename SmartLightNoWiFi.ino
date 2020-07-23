#include <Adafruit_NeoPixel.h>
#define PIN D3        // input pin Neopixel is attached to
#define NUMPIXELS      12 // number of neopixels in Ring

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// constants won't change. They're used here to set pin numbers:
const int buttonPin = D4;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

int redColor = 0;
int greenColor = 0;
int blueColor = 0;

int delayval = 50; // timing delay 0.5 second

int LightMode = 3;  // used to incriment light modes

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);
  pixels.begin(); // Initializes the NeoPixel library.
  setColor();
  
}

void loop() {
  setColor();
  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW) {
    LightMode += 1;   //increment light mode by 1 each time button is pressed.
   delay(500);

  }

  Serial.println(LightMode);
  
  switch (LightMode) {
  case 1:
  mode1(); // red
    break;

  case 2:
  mode2(); // green
    break;
  
  case 3:
  mode3(); // chase
    break;
 
  case 4:
  modeOFF(); // all off
    break;
  
}
      if (LightMode > 3){
      LightMode = 0;
    }
  delay(100); //just so the computer can keep up with simulation.
}

void mode1(){
      for (int i = 0; i < NUMPIXELS; i++) {
          pixels.setPixelColor(i, pixels.Color(200, 0, 0)); // red 200.
      }
    pixels.show(); // This sends the updated pixel color to the hardware.
  delay(500);
    
}

void mode2(){
      for (int i = 0; i < NUMPIXELS; i++) {
          pixels.setPixelColor(i, pixels.Color(0, 200, 0)); // green 200.
      }
    pixels.show(); // This sends the updated pixel color to the hardware.
  delay(500);
    
}

void mode3(){
      for (int i = 0; i < NUMPIXELS; i++) {
    //  setColor();
    pixels.setPixelColor(i, pixels.Color(redColor, greenColor, blueColor)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    delay(delayval); // Delay for a period of time (in milliseconds).

    if (i > NUMPIXELS) {
      pixels.clear();  //turn all light off
      i = 0; // start all over again!
    }
  }
}

void modeOFF(){
     pixels.clear();  //turn all light off
      pixels.show(); // This sends the updated pixel color to the hardware.

  delay(500);

}

// setColor()
// picks random values to set for RGB
void setColor(){
  redColor = random(0, 255);
  greenColor = random(0,255);
  blueColor = random(0, 255);
}
