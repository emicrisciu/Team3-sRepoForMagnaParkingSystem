
#define servoPin 	9
#define servoPin2 	3
#define trigPinIn1 	11
#define echoPinIn1 	10
#define trigPinOut1  8
#define echoPinOut1 	7
#define trigPinIn2 	6
#define echoPinIn2 	5
#define trigPinOut2  4
#define echoPinOut2 	2
#define ledVerde 	13
#define ledRosu		12
#define PragDist 	10

#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

bool deschis=false,deschis2=false,recep=false;
long duration, distance, rightSensor, leftSensor;
int motorDegree = 0;
int ms=0,ms2=0,ct=5,okct,okct2;
String command;

LiquidCrystal_I2C lcd(0x27,20,4);

Servo servo, servo2; 

long InSensor1, OutSensor1,InSensor2, OutSensor2;

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

    servo2.attach(servoPin2);
   	servo2.write(0);
  
	lcd.init();
    lcd.backlight();
  	pinMode(ledVerde,OUTPUT);
  	pinMode(ledRosu,OUTPUT);
  

   	pinMode(trigPinIn1, OUTPUT);
   	pinMode(echoPinIn1, INPUT);
   	pinMode(trigPinOut1, OUTPUT);
   	pinMode(echoPinOut1, INPUT);
    pinMode(trigPinIn2, OUTPUT);
   	pinMode(echoPinIn2, INPUT);
   	pinMode(trigPinOut2, OUTPUT);
   	pinMode(echoPinOut2, INPUT);

    Serial.begin (9600);	
  
  	TCCR0A=0;
  	TCCR0B = (1<<CS02) | (1<<CS00);
    TCCR2A=0;
  	TCCR2B = (1<<CS22) | (1<<CS20);
  
  	digitalWrite(ledVerde,LOW);
    digitalWrite(ledRosu,LOW);
  
    lcd.clear();
  	lcd.setCursor(1,0);
  	lcd.print("Bariera 1 Coborata");
    lcd.setCursor(1,1);
    lcd.print("Bariera 2 Coborata");
  	lcd.setCursor(2,3);
    lcd.print("Nr. locuri:");
    lcd.setCursor(14,3);
    lcd.print(ct);
          
    deschis=false;
    deschis2=false;

    ct=5;
    okct=0;
    okct2=0;
} 

void loop() 
{
   	InSensor1 = getSensorDistance(trigPinIn1, echoPinIn1);
   	OutSensor1 = getSensorDistance(trigPinOut1, echoPinOut1);
    InSensor2 = getSensorDistance(trigPinIn2, echoPinIn2);
   	OutSensor2 = getSensorDistance(trigPinOut2, echoPinOut2);
/*
   	Serial.print("In: ");
   	Serial.print(InSensor);
   	Serial.print("\nOut: ");
   	Serial.println(OutSensor);*/
    //Serial.println(ms);
  	//Serial.println(ms2);

  	if(TCNT0>15)
    {
      if(deschis==true)
      	ms++;
      TCNT0=0;
    }

    if(TCNT2>15)
    {
      if(deschis2==true)
      	ms2++;
      TCNT2=0;
    }

    if(deschis==false && recep==true && InSensor1>PragDist)
      recep=false;
  
	if (OutSensor1<=PragDist && deschis==true)
  	{
      if(okct==0)
      {
        --ct;
        okct=1;
      }
    	 	ms=0;
      	TCNT0=0;
  	}  
	else
    if (InSensor1<=PragDist && ct>0)
    {
      if(recep==false)
      {
        Serial.print("CAMERA\n");
        recep=true;
      }

      command=Serial.readStringUntil('\n');
      command.trim();

      if(command.equals("FALSE"))
        recep=false;
      else  
      if(deschis==false && command.equals("TRUE"))
      {
        for(int angle=1;angle<90;++angle)
        {
            servo.write(angle);
            //delay(5);
        }
		    digitalWrite(ledVerde,HIGH);
		    lcd.setCursor(1,0);
  			lcd.print("                  ");
		    lcd.setCursor(1,0);
        lcd.print("Bariera 1 Ridicata");
        
        deschis=true;
      }
    	ms=0;
      TCNT0=0;
    }
  		else
          if(OutSensor1>PragDist && ms>50 && deschis==true)
        {
            for(int angle=89;angle>0;--angle)
            {
                servo.write(angle);
            //delay(5);
            }

         	  digitalWrite(ledVerde,LOW);
          
          	lcd.setCursor(1,0);
  			    lcd.print("                  ");
  			    lcd.setCursor(1,0);
  			    lcd.print("Bariera 1 Coborata");
          	deschis=false;
            lcd.setCursor(14,3);
            lcd.print("    ");
            lcd.setCursor(14,3);
            lcd.print(ct);
          	ms=0;
            okct=0;
            recep=false;
        }





    if (InSensor2<=PragDist)
    {
      if(deschis2==false)
      {
        for(int angle=1;angle<90;++angle)
        {
            servo2.write(angle);
            //delay(5);
        }
		    digitalWrite(ledRosu,HIGH);
		    lcd.setCursor(1,1);
  			lcd.print("                  ");
		    lcd.setCursor(1,1);
  			lcd.print("Bariera 2 Ridicata");
        
        deschis2=true;
      }
    	ms2=0;
      TCNT2=0;
    }
	else
		if (OutSensor2<=PragDist && deschis2==true)
  		{
        if(okct2==0)
        {
          ++ct;
          okct2=1;
        }
    	 	ms2=0;
      	TCNT2=0;
  		}
  		else
          if(OutSensor2>PragDist && ms2>50 && deschis2==true)
        {
            for(int angle=89;angle>0;--angle)
            {
            servo2.write(angle);
            //delay(5);
            }

         	  digitalWrite(ledRosu,LOW);
          
          	lcd.setCursor(1,1);
  			    lcd.print("                  ");
  			    lcd.setCursor(1,1);
  			    lcd.print("Bariera 2 Coborata");

            lcd.setCursor(14,3);
            lcd.print("    ");
            lcd.setCursor(14,3);
            lcd.print(ct);
          	deschis2=false;
          	ms2=0;
            okct2=0;
        }


}