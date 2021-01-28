//char c[]="a"; //test letter
char x[7]={0,0,0,0,0,0,0}; //array to store 7 bit binary equivalent of ascii character
char c[44]; //array to store the string
String mystring = "the quick brown fox jumps over the lazy dog"; //message to be send

void setup()
{
Serial.begin(9600);
pinMode(9,OUTPUT); //setting pin 9 as output for laser
}

void loop()
{
delay(300); //indicating the start of a message
//loop to transfer the message
for (int i=0;i<44;i++)
{
mystring.toCharArray(c,44); //storing the string as an array
Serial.print(c[i]);
String thisString = String(c[i],BIN); //converting to binary equivalent of ascii character and storing it as a string
//code to transmit space
if (thisString=="100000")
{
 x[0]=0;
 for (int i=0; i<7;i++)
 {
  //Serial.println(x[i]);
  x[i+1]=thisString.charAt(i); //storing each bit of binary equivalent
  if ( x[i] == '1')
 {
 digitalWrite(9,HIGH);
 delay(50);
 }
 else  
 {
 digitalWrite(9,LOW);
 delay(50);
 }
 }
 delay(400);
}
 //code for all letters other than space
 else
 {
  for (int i = 0; i <7; i++)
  {
   x[i] = thisString.charAt(i); //storing each bit of binary equivalent
   if ( x[i] == '1')
  {
   digitalWrite(9,HIGH); //laser ON for binary 1
   delay(50);
  }
  else  
  {
  digitalWrite(9,LOW); //laser OFF for binary 0
  delay(50);
  }
 }
 delay(400);
}
}
}
