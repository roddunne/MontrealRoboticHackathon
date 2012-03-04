#define FRONT_IR_SENSOR_PIN  3 // Must be an analog pin
#define REAR_IR_SENSOR_PIN 2  // Must be an analog pin

#define FLAME_SENSOR_PIN 4 
#define SOUND_SENSOR_PIN 5
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
#define TILT_SENSOR_PIN 4 // Digital
#define VIBRATION_SENSOR_PIN 2 // Digital

#define BUZZER_PIN 3  // Must be an digital pin


int DEBUG_DUMP = 0;

int nRearSafetyThreshold = 100; // When the rover approaches the edge the signal falls from 400 to less than 100.
int nfrontSafetyThreshold = 100; // When the rover approaches the edge the signal falls from 400 to less than 100.

int E1 = 6; //M1 Speed Control
int E2 = 5; //M2 Speed Control
int M1 = 8; //M1 Direction Control
int M2 = 7; //M2 Direction Control

/// GLOBAL STATES
int MOVING_FORWARD = 1;
int TURNING_LEFT = 2;
int TURNING_RIGHT = 3;
int REVERSING = 4;
int STOPPED = 5;
int nMovementState_ = STOPPED;

/// PERMISSION STATES
boolean bSafeToReverse_ = false;
boolean bSafeToGoForward_ = false;

// SENSOR PUBLISHING
int nIterationsSinceLastPost_ = 0;
int nLightSensor_ = 0;
int nTemperatureSensor_ = 0;
int nFlameSensor_ = 0;
int nSoundSensor_ = 0;
int nTiltSensor_ = 0;
int nVibrationSensor_ = 0;

int nFrontIRSensor_ = 0;
int nRearIRSensor_ = 0;



void setup()
{
        pinMode(BUZZER_PIN, OUTPUT); 
        
        pinMode(TILT_SENSOR_PIN, INPUT); 
        
        pinMode(VIBRATION_SENSOR_PIN, INPUT); 
        
	int i;
	for(i=5;i<=8;i++)
	{
		pinMode(i, OUTPUT);
	}
	Serial.begin(115200); //Set serial baud rate to 115200
}


boolean isDangerousRearSensorValue(int n)
{
	return n < nRearSafetyThreshold;
}


boolean isDangerousFrontSensorValue(int n)
{
	return n < nfrontSafetyThreshold;
}


void readRearIRSensor()
{
	nRearIRSensor_ = analogRead(REAR_IR_SENSOR_PIN);
	if (DEBUG_DUMP) Serial.print(" RearIR:");
	if (DEBUG_DUMP) Serial.print(nRearIRSensor_);
	bSafeToReverse_ = ! isDangerousRearSensorValue(nRearIRSensor_);
	if (nMovementState_ != STOPPED && nMovementState_ != MOVING_FORWARD)
	{
		if (! bSafeToReverse_)
		{
			if (DEBUG_DUMP) Serial.print(" EMERGENCY STOP ");
			stop();
                        alert();
		}
	}
}


void readFrontIRSensor()
{
	nFrontIRSensor_ = analogRead(FRONT_IR_SENSOR_PIN);
	if (DEBUG_DUMP) Serial.print(" FrontIR:");
	if (DEBUG_DUMP) Serial.print(nFrontIRSensor_);
	bSafeToGoForward_ = ! isDangerousFrontSensorValue(nFrontIRSensor_);

	if (nMovementState_ != STOPPED && nMovementState_ != REVERSING)
	{
		if (! bSafeToGoForward_)
		{
			if (DEBUG_DUMP) Serial.print(" EMERGENCY STOP ");
			stop();
                        alert();
		}
	}
}


void stop(void) //Stop
{
	if (DEBUG_DUMP) Serial.print(" StopCommand ");
	digitalWrite(E1,LOW);
	digitalWrite(E2,LOW);
	nMovementState_ = STOPPED;
}


void forward(char a,char b)
{
	if (DEBUG_DUMP) Serial.print(" ForwardCommand ");
	if (bSafeToGoForward_)
	{
		analogWrite (E1,a);
		digitalWrite(M1,LOW);
		analogWrite (E2,b);
		digitalWrite(M2,LOW);
		nMovementState_ = MOVING_FORWARD;
	}
}


void reverse (char a,char b)
{
	if (DEBUG_DUMP) Serial.print(" ReverseCommand ");
	if (bSafeToReverse_)
	{
		analogWrite (E1,a);
		digitalWrite(M1,HIGH);
		analogWrite (E2,b);
		digitalWrite(M2,HIGH);
		nMovementState_ = REVERSING;		
	}
}


void left (char a,char b)
{
	if (DEBUG_DUMP) Serial.print(" LeftCommand ");
	analogWrite (E1,a);
	digitalWrite(M1,HIGH);
	analogWrite (E2,b);
	digitalWrite(M2,LOW);
	nMovementState_ = TURNING_LEFT;		
}


void right (char a,char b)
{
	if (DEBUG_DUMP) Serial.print(" RightCommand ");
	analogWrite (E1,a);
	digitalWrite(M1,LOW);
	analogWrite (E2,b);
	digitalWrite(M2,HIGH);
	nMovementState_ = TURNING_RIGHT;		
}


void servoleft()
{
    digitalWrite(8,1);
}

void servoright()
{
    digitalWrite(8,254);
}

void obtainCommandInput()
{
	if (DEBUG_DUMP) Serial.print(" ReadingCommands ");

	if( Serial.available() )           // if data is available to read
	{
		if (DEBUG_DUMP) Serial.print(" CharactersRead ");

		char val = Serial.read();

		int leftspeed = 200; //255 is maximum speed
		int rightspeed =  204;
		if (DEBUG_DUMP) Serial.print(val);
		if (DEBUG_DUMP) Serial.print(" ");

                click(); 

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
			case 'p'://Turn Right
				servoright ();
				break;

			case 'o'://Turn Right
				servoleft ();
				break;

			default:
				stop();
			break;
		}
	}

}




void click()                     
{
    digitalWrite(BUZZER_PIN, HIGH);
    delay(1);
    digitalWrite(BUZZER_PIN, LOW); 
}


void alert()                     
{
    digitalWrite(BUZZER_PIN, HIGH);
    delay(10);
    digitalWrite(BUZZER_PIN, LOW); 
    delay(10);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(10);
    digitalWrite(BUZZER_PIN, LOW); 
    delay(10);    
    digitalWrite(BUZZER_PIN, HIGH);
    delay(10);
    digitalWrite(BUZZER_PIN, LOW); 
}


void reportSensors()
{
    if (nIterationsSinceLastPost_ > 5)
    {
        nIterationsSinceLastPost_ = 0;
        
        nLightSensor_ = analogRead(A0);
        nTemperatureSensor_ = analogRead(A1);
        nFlameSensor_ = analogRead(FLAME_SENSOR_PIN);      
        nSoundSensor_ = analogRead(SOUND_SENSOR_PIN);      
      
        nTiltSensor_ = digitalRead(TILT_SENSOR_PIN);
        nVibrationSensor_ = digitalRead(VIBRATION_SENSOR_PIN);
        
        Serial.println();
        Serial.print("sensor light ");
        Serial.println(nLightSensor_);
        
        Serial.print("sensor temperature ");
        Serial.println(nTemperatureSensor_);

        Serial.print("sensor sound ");
        Serial.println(nSoundSensor_);


        Serial.print("sensor flame ");
        Serial.println(nFlameSensor_);    

        Serial.print("sensor tilt ");
        Serial.println(nTiltSensor_); 
    
        Serial.print("sensor vibration ");
        Serial.println(nVibrationSensor_);     
    
        Serial.print("sensor front_ir ");
        Serial.println(nFrontIRSensor_);     
    
        Serial.print("sensor rear_ir ");
        Serial.println(nRearIRSensor_);     
    }
    else
    {
        ++nIterationsSinceLastPost_;
    }
}


void loop()
{
	readFrontIRSensor();
	readRearIRSensor();
	obtainCommandInput();
        reportSensors();
	delay(100);
}

