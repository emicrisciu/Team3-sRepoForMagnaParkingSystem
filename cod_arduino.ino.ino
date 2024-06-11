#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

const int greenLedPin = 13;
const int redLedPin = 12;
const int trigPin1 = 11;
const int echoPin1 = 10;
const int servoPin1 = 9;
const int trigPin2 = 8;
const int echoPin2 = 7;
const int trigPin3 = 6;
const int echoPin3 = 5;
const int trigPin4 = 4;
const int echoPin4 = 2;
const int servoPin2 = 3;
const int debounce_time = 7000;

unsigned long lastSentTime = 0;

LiquidCrystal_I2C lcd(0x27, 20, 4);
Servo servo1, servo2;

int parkingSpots = 5; // numărul de locuri de parcare prestabilit
int simultan = 0; // flag folosit pentru a indica prezența unei situații când două mașini doresc utilizarea barierei simultan

int entryDistance = 10; // când o mașină se află la o distanță mai mică decât acest prag față de senzor, atunci este considerată suficient de aproape de barieră pentru a o deschide
int farThreshold = 20; // când o mașină se află la o distanță mai mare decât acest prag față de senzor, atunci este considerată îndepărtată suficient

long duration, distance, sensor1, sensor2, sensor3, sensor4; // senzorii cu index impar sunt cei aflați pe părțile exterioare ale barierelor și trebuie verificați constant
                                                             // senzorii cu index par sunt cei care se verifică doar pt a ne asigura că într-adevăr mașina a trecut de barieră și putem lăsa bariera jos
String command;
int time = 0;
bool valid = true;
bool presence = false;

//bool camera = true;

long getSensorDistance(int trigPin, int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); //waits to avoid collision in serial monitor
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); //keep the trigger on to generate pulse
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); //If pulse reached the receiver echoPin become high, then pulseIn() returns the time taken by the pulse to reach the receiver
  distance = (duration * .0343)/2; //we divide by 2 because the sound travel to the object and back, so the distance to the object is half the distance traveled by the sound
  return distance;
}

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;

bool validarePrezenta(int trigPin, int echoPin) // timp de 2 secunde se verifica daca intr-adevar avem o cerere de ridicare a barierei
{
  // time = 0;
  valid = true;
  previousMillis = millis();
  currentMillis = millis();
  while(currentMillis - previousMillis < 2000)
  {
    if(getSensorDistance(trigPin, echoPin) > entryDistance)
    {
      valid = false;
      break;
    }
    // time += 10;
    // delay(100);
    currentMillis = millis();
  }
  return valid;
}

bool validareTrecereBariera(int trigPin, int echoPin)
{
  // time = 0;
  presence = false;
  previousMillis = millis();
  currentMillis = millis();
  while(currentMillis - previousMillis < 5000)
  {
    if(getSensorDistance(trigPin, echoPin) <= entryDistance)
    {
      presence = true;
      break;
    }
    // time += 10;
    // delay(100);
    currentMillis = millis();
  }
  return presence;
}

// funcția setup, în care se configurează pinii digitali
void setup() 
{
  pinMode(greenLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  lcd.init();
  lcd.backlight();
  servo1.attach(servoPin1);
  servo1.write(0);
  servo2.attach(servoPin2);
  servo2.write(0);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(trigPin4, OUTPUT);
  pinMode(echoPin4, INPUT);
  Serial.begin(9600);
}

void loop() 
{
  // controlăm LED-urile
  if(parkingSpots > 0) // atunci când parcarea mai are locuri libere, LED-ul verde va fi aprins, iar cel roșu stins
  {
    digitalWrite(greenLedPin, HIGH);
    digitalWrite(redLedPin, LOW);
  }
  else // atunci când parcarea nu mai are locuri libere, LED-ul roșu va fi aprins, iar cel verde stins
  {
    digitalWrite(greenLedPin, LOW);
    digitalWrite(redLedPin, HIGH);
  }

  // afișăm pe ecranul LCD numărul de locuri libere
  lcd.clear();
  lcd.print("Locuri neocupate");
  lcd.setCursor(7,1);
  lcd.print(parkingSpots);

  // ne interesează dacă se dorește o intrare sau o ieșire din parcare deci se vor verifica senzorii ce stau înaintea fiecărei dintre cele 2 bariere
  sensor1 = getSensorDistance(trigPin1, echoPin1);
  sensor3 = getSensorDistance(trigPin3, echoPin3);

  

  
      if(sensor3 <= entryDistance) // în caz contrar, verificăm dacă se solicită bariera de la ieșire
      {
        bool v3 = validarePrezenta(trigPin3, echoPin3);
        valid = v3;

        if(valid)
        {
          // se ridică bariera de la ieșire
          for (int angle = 0; angle <= 90; angle += 1)
          {
            servo2.write(angle);
            delay(10);
          }

           // ramanem in aceasta bucla pana cand masina se indeparteaza de senzor (fie catre parcare, fie catre exteriorul ei)
          sensor3 = getSensorDistance(trigPin3, echoPin3);
          while(sensor3 <= entryDistance)
          {
            sensor3 = getSensorDistance(trigPin3, echoPin3);
            delay(500);
          }

          // acum trebuie verificat dacă mașina trece de barieră
          bool p4 = validareTrecereBariera(trigPin4, echoPin4);
          presence = p4;

          if(presence) // se ajunge aici doar dacă mașina a trecut deja pe sub bariera de la ieșire
          {
            parkingSpots++; // asta înseamnă că trebuie să creștem numărul de locuri de parcare disponibile deoarece a avut loc o ieșire validă din parcare
            // se așteaptă ca mașina să se îndepărteze de barieră pentru a putea fi lăsată jos
            time = 0;
            sensor4 = getSensorDistance(trigPin4, echoPin4);
            while(sensor4 < farThreshold || time < 100) // se iasă din while abia atunci când mașina e îndepărtată. Dacă cumva se mișcă mașina foarte rapid atunci se va sta 2 secunde datorită lui time
            {
              sensor4 = getSensorDistance(trigPin4, echoPin4);
              time += 10;
              delay(200);
            }
            delay(300);

            for (int angle = 90; angle >= 0; angle -= 1)
            {
              servo2.write(angle);
              delay(10);
            }
          }
          else
          {
            // se coboară bariera
            for (int angle = 90; angle >= 0; angle -= 1)
            {
              servo2.write(angle);
              delay(10);
            }
          }

          // actualizare stare LED-uri
          if(parkingSpots > 0)
          {
            digitalWrite(greenLedPin, HIGH);
            digitalWrite(redLedPin, LOW);
          }
          else
          {
            digitalWrite(greenLedPin, LOW);
            digitalWrite(redLedPin, HIGH);
          }

          // afișăm pe ecranul LCD numărul actualizat de locuri libere
          lcd.clear();
          lcd.print("Locuri neocupate");
          lcd.setCursor(7,1);
          lcd.print(parkingSpots);
        }
        else // dacă validarea eșuează stopăm întreg sistemul pt 2 secunde
        {
          lcd.clear();
          lcd.setCursor(2,0);
          lcd.print("Iesire interzisa");
          delay(2000);
        }
      }
      else
      {
        //Serial.print("DELETE\n");
        if(sensor1 <= entryDistance) // dacă nu avem de a face cu puțin probabila situație în care fix deodată se solicită ambele bariere, atunci mergem mai departe
        // și gestionăm situația când e solicitată doar intrarea în parcare la un moment de timp
        {
          if(parkingSpots > 0) // are sens să vorbim de intrarea în parcare doar atunci când mai există locuri libere
          {
            bool v1 = validarePrezenta(trigPin1, echoPin1);
            valid = v1;

            if(valid)
            {
       


              if(millis() - lastSentTime > debounce_time)
              {
                Serial.print("CAMERA\n");
                lastSentTime = millis();
              }
              
                  
         
             
             command = Serial.readStringUntil('\n');
             command.trim();
             if(command.equals("TRUE"))
             {
              
              // se ridică bariera de la intrare
              for (int angle = 0; angle <= 90; angle += 1)
              {
                servo1.write(angle);
                delay(10);
              }

              // ramanem in aceasta bucla pana cand masina se indeparteaza de senzor (fie catre parcare, fie catre exteriorul ei)
              sensor1 = getSensorDistance(trigPin1, echoPin1);
              while(sensor1 <= entryDistance)
              {
                sensor1 = getSensorDistance(trigPin1, echoPin1);
                delay(500);
              }

              // acum trebuie verificat dacă mașina trece de barieră, validare ce ne indica in ce directie s-a deplasat masina
              bool p2 = validareTrecereBariera(trigPin2, echoPin2);
              presence = p2;

              if(presence) // se ajunge aici doar dacă mașina a trecut deja pe sub bariera de la intrare
              {
                parkingSpots--; // asta înseamnă că trebuie să scădem numărul de locuri de parcare disponibile deoarece a avut loc o intrare validă în parcare
                // se așteaptă ca mașina să se îndepărteze de barieră pentru a putea fi lăsată jos
                time = 0;
                sensor2 = getSensorDistance(trigPin2, echoPin2);
                while(sensor2 < farThreshold || time < 100) // se iasă din while abia atunci când mașina e îndepărtată. Dacă cumva se mișcă mașina foarte rapid atunci se va sta 2 secunde datorită lui time
                {
                  sensor2 = getSensorDistance(trigPin2, echoPin2);
                  time += 10;
                  delay(200);
                }
                delay(300);
                // se coboară bariera
                for (int angle = 90; angle >= 0; angle -= 1)
                {
                  servo1.write(angle);
                  delay(10);
                }
                //camera=true;
              }
              else
              {
                // se coboară bariera
                for (int angle = 90; angle >= 0; angle -= 1)
                {
                  servo1.write(angle);
                  delay(10);
                }
                //Serial.print("DELETE\n");
                //camera=true;
                
              }

              // actualizare stare LED-uri
              if(parkingSpots > 0)
              {
                digitalWrite(greenLedPin, HIGH);
                digitalWrite(redLedPin, LOW);
              }
              else
              {
                digitalWrite(greenLedPin, LOW);
                digitalWrite(redLedPin, HIGH);
              }

              // afișăm pe ecranul LCD numărul actualizat de locuri libere
              lcd.clear();
              lcd.print("Locuri neocupate");
              lcd.setCursor(7,1);
              lcd.print(parkingSpots);
              }
              //else
              //{
                //camera=true;
                //Serial.print("DELETE\n");
              //}
            }
            else // dacă validarea eșuează stopăm întreg sistemul pt 2 secunde
            {
              lcd.clear();
              lcd.print("Acces interzis");
              delay(2000);
            }
          }
          else // dacă parcarea este deja plină vom afișa un mesaj corespunzător
          {
            lcd.clear();
            lcd.setCursor(1,0);
            lcd.print("Parcarea este");
            lcd.setCursor(5,1);
            lcd.print("plina!");
            delay(2000);
          }
        }
      }
      delay(100); // senzorii calculează distanțe la fiecare 100ms pentru a capta într-un timp cât mai apropiat de cel real o solicitare de ridicare a barierei
}
  
