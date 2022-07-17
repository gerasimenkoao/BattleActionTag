// radar-style Point to shoot sound with two microphones
#include <Arduino.h>
#include <Math.h>
#include <FastLED.h>

#define NUM_LEDS 20  // How many leds in your strip?
#define DATA_PIN 9  // LED strip pin
CRGB leds[NUM_LEDS]; // Define the array of leds

int range = 180; //width of detection sector
int degree;    // degree to shoot point
int delta = 0; // delay of signal between microphones
int delays[NUM_LEDS]; //delays of each degree

boolean quiet = true ;  // mark of no shoots
boolean centre = false ; // mark of centre position
word radar[NUM_LEDS];    //number of shoots at each directions
byte pos =0; //led position
byte lednum;
byte  br = 200; //brightness of led-s
int ar; // read analog signal from potentiometer

long base = 240; //distance between microphones in mm
long r=base/2;
long x = 0; // delay between signal in mmm
long v = 340 ; // sound speed
long rd=1000*r/v;  //time in micros that sound pass the radius
byte i;
// variables to write interrupts time
volatile unsigned long time1 = 0;
volatile unsigned long time2 = 0;

void setup() {
Serial.begin(9600);
degree=180/NUM_LEDS;
for (i=0; i<NUM_LEDS ; i++) //some math for begin becose arduino not so good in it
{
  delays[i]=rd*cos((degree*i+degree/2)*3.141592/180); // signal delay in micros for each direction
 // Serial.print(delays[i]); Serial.println(); // just check 4 debug 
}
  
 
 pinMode(LED_BUILTIN, OUTPUT);
    
  for (i=0; i<NUM_LEDS ; i++){leds[i]=0;}

      FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);  // GRB ordering is typical, RGB if wrong colors
      attachInterrupt(0, sensor_1, RISING); //left sensor
      attachInterrupt(1, sensor_2, RISING); //right sensor
}

void loop() 
{
ar=analogRead(0);
br=map(ar, 0, 1023, 0, 255);

  if (quiet) { // display radar-style lights
      leds[pos] = CRGB::Black;
    if (pos < NUM_LEDS-1) {pos++;} else {pos=0;}
      leds[pos] = CRGB::White;
  FastLED.setBrightness(br/3);
  FastLED.show();
  }
  else { //display shoot intense - <3 - blue sector 4-9 - green sector, over 9 - red sector
    leds[pos] = CRGB::Black;
    for (i=0; i<NUM_LEDS ; i++){  
          
       if ( radar[i] > 9 ) { leds[i] =  leds[i] = CRGB::Red; }
       else {if ( radar[i] > 4  ) {leds[i] = leds[i] = CRGB::Green; } 
       else {if ( radar[i] > 0  ) {leds[i] = leds[i] = CRGB::Blue; } }}
      
      if (centre) {digitalWrite(LED_BUILTIN, HIGH);} else {digitalWrite(LED_BUILTIN, LOW);}
      }
      FastLED.setBrightness(br);
      FastLED.show(); 
      }
      delay(100);  
 
    if ( time1 != 0 && time2 != 0 )
    {
      quiet=false;
            delta=time2-time1;
            if (abs(delta)>delays[0] ) {goto wrongarg;}
      for (i=0; i<NUM_LEDS ; i++){  if (delta<delays[i]){ lednum=i; } }     
      
      Serial.print(delta); Serial.println(); // click on middle betwen microphones will return closer to 0 digit (you need adjuct it by mic. sensitivity)
      
      radar[lednum]++;
      
      if (abs(delta)<16) {centre=true;} else {centre=false;} 
      
      wrongarg:
      
      delay(30);
          time1 = 0;
          time2 = 0;
    }
     
}

void sensor_1()
{ if ( time1 == 0 ) { time1=micros(); } }

void sensor_2()
{ if ( time2 == 0 ) { time2=micros();}  }
