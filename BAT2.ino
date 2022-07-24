// radar-style Point to shoot sound with two microphones
#include <Arduino.h>
#include <Math.h>
#include <FastLED.h> //https://github.com/FastLED/FastLED

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
byte  br1 = 0; //brightness of led-s
int ar; // read analog signal from potentiometer

long base = 240; //distance between microphones in mm
long r=base/2;
long x = 0; // delay between signal in mmm
long v = 340 ; // sound speed
long rd=1000*r/v;  //time in micros that sound pass the radius
byte i;
int correction; // value from 4 adjuct center position
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
      
      leds[0] = CRGB::White; // next 3 lines set white color of border screen sectors 
      leds[NUM_LEDS-1] = CRGB::White;
      FastLED.setBrightness(br);
      FastLED.show();
     
      attachInterrupt(0, sensor_1, RISING); //left sensor
      attachInterrupt(1, sensor_2, RISING); //right sensor
}

void loop() 
{
ar=analogRead(0);
br1=map(ar, 0, 1023, 0, 255);
if (abs(br1-br)>7) //set brightness if its really changed
{  
br=br1;
FastLED.setBrightness(br);
FastLED.show();
}
delay(100);
 
    if ( time1 != 0 && time2 != 0 )
    {
      quiet=false;
            delta=time2-time1;
        //    if (abs(delta)>delays[0] ) {goto wrongarg;}
        Serial.print(delta); Serial.println(); // click on middle betwen microphones will return closer to 0 digit (you need adjuct it by mic. sensitivity)
        correction=-2; // to precision adjuct set small(-50 to 50) value from serial monitor multiplied on -1 to correction variable
        delta=delta+correction;
      for (i=0; i<NUM_LEDS ; i++){  if (delta<delays[i]){ lednum=i; } }     
       
      radar[lednum]++;
      
      if (abs(delta)<16) {centre=true;} else {centre=false;} 

       for (i=0; i<NUM_LEDS ; i++){  //display shoot intense - <3 - blue sector 4-9 - green sector, over 9 - red sector
       leds[0] = CRGB::Black; 
       leds[NUM_LEDS-1] = CRGB::Black;
       if ( radar[i] > 9 ) { leds[i] =  leds[i] = CRGB::Red; }
       else {if ( radar[i] > 4  ) {leds[i] = leds[i] = CRGB::Green; } 
       else {if ( radar[i] > 0  ) {leds[i] = leds[i] = CRGB::Blue; } }}
      
      if (centre) {digitalWrite(LED_BUILTIN, HIGH);} else {digitalWrite(LED_BUILTIN, LOW);}
      }
      FastLED.setBrightness(br);
      FastLED.show(); 
      
      wrongarg:
      
      delay(50);
          time1 = 0;
          time2 = 0;
    }
     
}

void sensor_1()
{ if ( time1 == 0 ) { time1=micros(); } }

void sensor_2()
{ if ( time2 == 0 ) { time2=micros();}  }
