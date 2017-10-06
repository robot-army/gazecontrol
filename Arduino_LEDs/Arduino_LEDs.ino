#include <Adafruit_NeoPixel.h>
#include <string.h>

// CHANGE this number for each fiducial

#define PIN 6
#define MAXLEDS 56

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(MAXLEDS, PIN, NEO_RGB + NEO_KHZ800);

String inputString ="";
boolean stringComplete = false;

void setup() {
  Serial.begin(115200);
  pixels.begin();
  inputString.reserve((3*MAXLEDS)+3);
}

void loop() {  
  if (stringComplete) {
    
 //   Serial.println(inputString);

    int ledindex = 0;
    for(int i=0; i<(inputString.length()-1); i=i+3) {
      int R = (inputString.charAt(i)-'0')*28;
      int G = (inputString.charAt(i+1)-'0')*28;
      int B = (inputString.charAt(i+2)-'0')*28;
      pixels.setPixelColor(ledindex++,R,G,B);
    }

    pixels.show(); // This sends the updated pixel color to the hardware.
    inputString = "";
    stringComplete = false;
    
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;  
    if(inChar == '\n') {
      stringComplete = true;
    }
  }
}
