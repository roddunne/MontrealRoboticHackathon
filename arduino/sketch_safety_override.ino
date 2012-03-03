#define FRONT_IR_SENSOR_PIN  2 // Must be an analog pin
#define REAR_IR_SENSOR_PIN 1  // Must be an analog pin
int fFrontSensorValue;    // Must be of type float for pow()
int fRearSensorValue;
int fFrontInches;
int fRearInches;
boolean bSafeToReverse = false;
boolean bSafeToGoForward = false;

int nRearSafetyThreshold = 100; // When the rover approaches the edge the signal falls from 400 to less than 100.
int nfrontSafetyThreshold = 100; // When the rover approaches the edge the signal falls from 400 to less than 100.

int E1 = 6; //M1 Speed Control
int E2 = 5; //M2 Speed Control
int M1 = 8; //M1 Direction Control
int M2 = 7; //M2 Direction Control

void setup()
{
    int i;
    for(i=5;i<=8;i++)
    {
        pinMode(i, OUTPUT);
    }
    Serial.begin(115200); //Set serial baud rate to 115200
}


boolean isDangerousRearSensorValue(int n)
{
  return n < 100;
}


boolean isDangerousFrontSensorValue(int n)
{
  return n < 100;
}


void readRearIRSensor()
{
    fRearSensorValue = analogRead(REAR_IR_SENSOR_PIN);
    Serial.print(" RearIR:");
    Serial.print(fRearSensorValue); 
    bSafeToReverse = ! isDangerousRearSensorValue(fRearSensorValue);
    if (! bSafeToReverse)
    {
        Serial.print("STOPPING"); 
        stop();
    }
}


void readFrontIRSensor()
{
    fFrontSensorValue = analogRead(FRONT_IR_SENSOR_PIN);
    Serial.print(" FrontIR:");
    Serial.print(fFrontSensorValue); 
    bSafeToGoForward = ! isDangerousFrontSensorValue(fFrontSensorValue);
    
    if (! bSafeToGoForward)
    {
        Serial.print("STOPPING"); 
       stop();
       
    }
}





void stop(void) //Stop
{
     Serial.print(" STOPPING ");
     digitalWrite(E1,LOW);
     digitalWrite(E2,LOW);
}


void forward(char a,char b)
{
     Serial.print(" Forward ");
    if (bSafeToGoForward)
  {  
     analogWrite (E1,a);
     digitalWrite(M1,LOW);
     analogWrite (E2,b);
     digitalWrite(M2,LOW);
  }
}


void reverse (char a,char b)
{
     Serial.print(" reverse ");
     if (bSafeToReverse)
     {
        analogWrite (E1,a);
        digitalWrite(M1,HIGH);
        analogWrite (E2,b);
        digitalWrite(M2,HIGH);
     }
}


void left (char a,char b)
{
     Serial.print(" left ");
    analogWrite (E1,a);
    digitalWrite(M1,HIGH);
    analogWrite (E2,b);
    digitalWrite(M2,LOW);
}


void right (char a,char b)
{
     Serial.print(" right ");
    analogWrite (E1,a);
    digitalWrite(M1,LOW);
    analogWrite (E2,b);
    digitalWrite(M2,HIGH);
}


void obtainCommandInput()
{
     Serial.print(" oci ");
    
  //while (Serial.available() < 1) {} 
  if( Serial.available() )           // if data is available to read
  {  
     Serial.print(" available ");

    char val = Serial.read();
    
    int leftspeed = 50; //255 is maximum speed
    int rightspeed = 51;
     Serial.print(val);
     Serial.print(" ");
    
    switch (val) // Perform an action depending on the command
    {
         case 'w'://Move Forward
             forward (leftspeed,rightspeed);
            break;
         case 's'://Move Backwards
            reverse (leftspeed,rightspeed);
            break;
          case 'a'://Turn Left
            left (leftspeed,rightspeed);
            break;
          case 'd'://Turn Right
            right (leftspeed,rightspeed);
            break;
          default:
              stop();
          break;
        }
    }

}


void moveRobot()
{
}


void loop()
{
    readFrontIRSensor();
    readRearIRSensor();
    obtainCommandInput();
    delay(100);
    Serial.println();
}

