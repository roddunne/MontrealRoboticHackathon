void setup()
{
Serial.begin(115200); //Set serial baud rate to 115200
}
void loop()
{
Serial.print("Hello!"); //print out hello string
delay(1000); //1 second delay
}
