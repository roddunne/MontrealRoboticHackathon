void setup()
{
Serial.begin(115200); //Set serial baud rate to 115200

pinMode(A0,LOW);
pinMode(A1,HIGH);

}

void loop()
{
      int val;
      val=analogRead(2);   //connect mic sensor to Analog 0
      Serial.println(val,DEC);//print the sound value to serial        
      delay(100);
}

