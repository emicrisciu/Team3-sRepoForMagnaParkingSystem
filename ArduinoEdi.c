#define servoPin     12
#define trigPinIn     11
#define echoPinIn     10
#define trigPinOut  9
#define echoPinOut     8
#define ledVerde     13
#define ledRosu        7
#define PragDist     60

#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

bool deschis=false;
long duration, distance, rightSensor, leftSensor;
int motorDegree = 0;
int ms=0;

LiquidCrystal_I2C lcd(0x27,20,4);

Servo servo;

long InSensor, OutSensor;

long getSensorDistance(int trigPin, int echoPin)
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2); //waits to avoid collision in serial monitor
       digitalWrite(trigPin, HIGH);
    delayMicroseconds(10); //keep the trigger on to generate pulse
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH); //If pulse reached the receiver echoPin become high, then pulseIn() returns the time taken by the pulse to reach the receiver

    distance = (duration/2) / 29.1; //we divide by 2 because the sound travel to the object and back, so the distance to the object is half the distance traveled by the sound

    return distance;
}

void setup()
{
      servo.attach(servoPin);
       servo.write(0);
  
    lcd.init();
    lcd.backlight();
      pinMode(ledVerde,OUTPUT);
      pinMode(ledRosu,OUTPUT);
  
       Serial.begin (115200);
       pinMode(trigPinIn, OUTPUT);
       pinMode(echoPinIn, INPUT);
       pinMode(trigPinOut, OUTPUT);
       pinMode(echoPinOut, INPUT);

       
  
      TCCR0A=0;
      TCCR0B = (1<<CS02) | (1<<CS00);
  
      digitalWrite(ledVerde,LOW);
    digitalWrite(ledRosu,LOW);
  
    lcd.clear();
      lcd.setCursor(5,0);
      lcd.print("Bariera");
      lcd.setCursor(7,1);
    lcd.print("Coborata");
          
    deschis=false;
}

void loop()
{
       InSensor = getSensorDistance(trigPinIn, echoPinIn);
       OutSensor = getSensorDistance(trigPinOut, echoPinOut);
/*
       Serial.print("In: ");
       Serial.print(InSensor);
       Serial.print("\nOut: ");
       Serial.println(OutSensor);*/
      Serial.println(ms);

      if(TCNT0>15)
    {
      if(deschis==true)
          ms++;
      TCNT0=0;
    }
  

    if (InSensor<=PragDist)
    {
      if(deschis==false)
      {
          motorDegree = 90;
        servo.write(90);
        digitalWrite(ledVerde,HIGH);
          digitalWrite(ledRosu,LOW);
        lcd.clear();
        lcd.setCursor(5,0);
        lcd.print("Bariera");
        lcd.setCursor(7,1);
        lcd.print("Ridicata");
        
        deschis=true;
      }
        ms=0;
          TCNT0=0;
    }
    else
        if (OutSensor<=PragDist && deschis==true)
          {
                servo.write(90);
             ms=0;
              TCNT0=0;
          }
          else
          if(OutSensor>PragDist && ms>100 && deschis==true)
        {
             digitalWrite(ledVerde,LOW);
              digitalWrite(ledRosu,HIGH);
          
              lcd.clear();
              lcd.setCursor(5,0);
              lcd.print("Bariera");
              lcd.setCursor(7,1);
            lcd.print("Coborata");
              deschis=false;
            servo.write(0);
              ms=0;
        }

}

