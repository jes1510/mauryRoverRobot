/* Based on the PING example by Tom Igoe from
   http://www.arduino.cc/en/Tutorial/Ping
     
   Original source code for Maury Rover Robot is located here:
   https://github.com/jes1510/mauryRoverRobot
 
   This example code is in the public domain.

 */
 
#include <Servo.h>

// Defines for pins
#define pingOutPin 7
#define pingInPin 8
#define RMOTORAPIN 5
#define RMOTORBPIN 6
#define LMOTORAPIN 9
#define LMOTORBPIN 10

Servo scanServo;

// global variables
long distance;
long echo;
int lastTime;
int currentTime;
int servoPosition = 0;


int angles[5] = {0, 45, 90, 135, 180};
int ranges[5] = {0, 0,  0,  0,   0};


void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  
  // Configure Sensor pin
  pinMode(pingOutPin, OUTPUT);
  pinMode(pingInPin, INPUT);
  
    // Configure motor Pins
  pinMode(RMOTORAPIN, OUTPUT);
  pinMode(RMOTORBPIN, OUTPUT);
  pinMode(LMOTORAPIN, OUTPUT);
  pinMode(LMOTORBPIN, OUTPUT);
  
  // configure the scan servo
  scanServo.attach(11);
  
  scan();
  forward(128);
  
  
  
  
}

void loop()
{  
 
  
  scan();
  
 // forward(128);
  //delay(3000);
 // forward(0);
 // delay(1000); 
 

 
  // delay(500);
 
}

void forward(int s)
{
  analogWrite(RMOTORBPIN, s);
  digitalWrite(RMOTORAPIN, LOW);  
  analogWrite(LMOTORAPIN, s);
  digitalWrite(LMOTORBPIN, LOW);  
}


int range(int angle)
{
  int i;
  long sum;
  int avg;
  
  scanServo.write(angle);
  delay(250);
  
  for(i=0; i<11; i++)
  {
    echo = doPing();    
    sum += microsecondsToInches(echo);  
  }
  
  avg = sum/10;  
  return avg;
}
  
  
    
  
  
void scan(void)
{
  int i;
  for (i=0; i< (sizeof(angles) + 1); i++)
  {
    ranges[i] = range(angles[i]);
    Serial.println(i);
    
  }
}
  



long doPing(void)
{
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
 
  digitalWrite(pingOutPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingOutPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingOutPin, LOW);

  // Read the pulse in pin
  duration = pulseIn(pingInPin, HIGH);
  
  // Return the pulse width
  
  return duration;
  
}



long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

void steppedScan(void)
{
   for (servoPosition = 0; servoPosition < 180; servoPosition+= 15)
 {
   scanServo.write(servoPosition);
   delay(15);
   
   echo = doPing();
   distance = microsecondsToInches(echo);
   Serial.println(distance);
   delay(75);
 }
 
  for (servoPosition = 180; servoPosition >= 1; servoPosition-= 15)
 {
   scanServo.write(servoPosition);
   delay(15);
   
   echo = doPing();
   distance = microsecondsToInches(echo);
   Serial.println(distance);
   delay(75);
 }
}
