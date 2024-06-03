String command;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.print(".\n");
  //Serial.print("da");
  if(Serial.available()){
    command = Serial.readStringUntil('\n');
    command.trim();
    if(command.equals("w")){
      //Serial.println();
      Serial.print("Alb\n");
    }
    else if(command.equals("a")){
      //Serial.println();
      Serial.print("albastru\n");
    }
  }
  delay(1000);
}
