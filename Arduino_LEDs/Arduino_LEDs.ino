#include <Adafruit_NeoPixel.h>
#include <string.h>


#define PIN 6
#define NUMPIXELS 28
#define ARRAYSIZE 38
#define MAXWIDTH 4
#define MINWIDTH 1
#define MAXDIST 1000

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_RGB + NEO_KHZ800);

  
  int b[ARRAYSIZE];
  int c[ARRAYSIZE];
  int d[ARRAYSIZE];


void setup() {
  Serial.begin(115200);
  pixels.begin();
  
    //lazy array initialization - fix hacked bounds
  for(int i=0; i<ARRAYSIZE; i++) {
    d[i] = c[i] = b[i] = 0;
  }
}

void loop() {
  int angle = 0;
  int distance = 1000;
  int centreled;
  float dist;
  String  inputstring;
  String dist_string;
  String angle_string;
  int refresh=0;

//these for loops are for testing only, they'll be replaced with a Serial.read() when connected to the other thing
//
//for(distance=1000; distance > 50; distance=distance-100){
//  for(angle=0;angle<360;angle=angle+10){
while (Serial.available()){
  delay(30);
  inputstring = Serial.readStringUntil('A');
  refresh = 1;
}
    dist_string = inputstring.substring(0,4);
    angle_string = inputstring.substring(4,8);
    angle = angle_string.toInt();
    distance = dist_string.toInt();

    
if(distance==0){
  distance = 1000;
}
    //lazy array initialization - fix hacked bounds
     for(int i=0; i<ARRAYSIZE; i++) {
    d[i] = c[i] = b[i] = 0;
  }

//  Serial.print("Distance: ");
//  Serial.print(distance);
//  Serial.print(" Angle: ");
//  Serial.print(angle);
//  Serial.print(" Refresh: ");
//  Serial.println(refresh);
    
  centreled = map(angle,0,360,0,NUMPIXELS);

  delay(40);
  
  dist = map(distance,0,MAXDIST,MAXWIDTH,MINWIDTH);



 
    //make the ramp one-sided
    for(int i=0; i<(dist+1); i++) {
      b[i] = 255*float(i/dist);
      if(distance==1000){
        b[i] = 0;
      }
    }

    

    //make the ramp two sided
    for(int i=0; i<(2*dist)+1; i++) {
      int disti = dist-(i-dist);
      if(i<dist) c[i] = b[i];
      if(i>=dist & i<(2*dist)+1) c[i] = b[disti];
      if(i>=(2*dist)+1) c[i] = 0; 
    }


    //centre the ramp around the right pixel and set the pixel colour
    for(int i=0; i<NUMPIXELS; i++) {
      int index = (centreled-i+int(dist)) % (NUMPIXELS);
      pixels.setPixelColor(i,0,c[index],c[index]);    
    }

  if(refresh == 1){
          pixels.show(); // This sends the updated pixel color to the hardware.
      refresh = 0;
  }
    
  }
//}
//}
