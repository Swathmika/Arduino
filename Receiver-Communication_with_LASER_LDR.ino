//Receiver

char y[7]={'1','1','0','0','0','0','1'}; //test binary code

void setup() {
  // put your setup code here, to run once:
 
Serial.begin(9600);
pinMode(A0,INPUT); //A0 pin in the arduino is set as input

}

void loop() {
  // put your main code here, to run repeatedly:

delay(400); //to indicate of the start of a message
 for (int i=0;i<7;i++)
 {
  int rawvalue = analogRead(A0); //to read the sensor value from the pin A0

  if (rawvalue>700)
  {
    y[i]='1'; //if the sensor value is greater than 700,the bit is coded as 1
    delay(50);
  }
  else
  {
   y[i]='0';
   delay(50);
  }
  }

 String mystring(y);

long value = strtol(y, NULL, 2);
Serial.print((char)value); //print corrresponding ascii character

}
