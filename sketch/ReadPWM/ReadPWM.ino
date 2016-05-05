#include <SPI.h>
#include <Wire.h>
#include <TM1637.h>

#define CLK 3
#define DIO 2
TM1637 tm1637(CLK,DIO);

//const int hardwareCounterPin = 5;

unsigned int count;
unsigned int preC;

void setup(void) {
  Wire.begin(4);
  Wire.onRequest(requestEvent);
  Serial.begin(9600);
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  count = 0;
  preC = 0;
  tm1637.init();
  tm1637.set(7);
}

void loop() {
  count = 0;
  bitSet(TCCR1B,CS12);
  bitSet(TCCR1B,CS11);
  bitSet(TCCR1B,CS10);
  delay(500);
  TCCR1B = 0;
  count = TCNT1;
  TCNT1 = 0;
  //if(abs(count-preC)>10){
    //count = preC;
  //}
  
  //Serial.println((float)count*3600/8000*1.609/1.17);
  int number = count*3600/8000*1.609/1.17;
  Serial.print("count: ");
  Serial.println(count);
  Serial.print("KMH: ");
  Serial.println(number);
  
  int speed = count*1.1667;
  number = speed;
  //number = 157;
  if((number /100) == 0)
  {
      tm1637.display(1,17);
  }
    else {
      tm1637.display(2,number/100); 
    }
    if( (number%100)/10 == 0 )
      tm1637.display(2,17);
      else {
    tm1637.display(1,(number%100)/10);
      }
    tm1637.display(0,(number%10));
  
}

void requestEvent() {
  Wire.write("STA");
}
  



