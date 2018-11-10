
/*MAIN PROGRAM 10/19/18 WE ARE TRYING TO RUN THE FAN AND THE ULTRASONIC
 * SENSOR AT THE SAME. WE WANT TO TAKE DATA FROM THE SENSOR VARIABLE "DISTANCE" AND
 * SEND IT TO OUR SENSOR TO TURN ON OUR FAN ("outputPin") AND CONTROL THE VELOCITY
 * FROM SENSOR DATA. DELAY IS TOO LONG. FAN WON'T WORK.
 */
/* 
#define echoPin 8
#define trigPin 7
*/
#define outputPin 3
long duration, distance;
int PWMVal;

void setup() {
  Serial.begin(9600);
  //pinMode(echoPin,INPUT);
  //pinMode(trigPin,OUTPUT);
  pinMode(outputPin,OUTPUT);
}

void loop() {
  
  /*digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = duration/58.2;

  Serial.println(distance);
  */
  //digitalWrite(outputPin,HIGH);
  PWMVal = 255;
  //PWMVal = map(distance, 2, 100, 0, 255);
  analogWrite(outputPin, PWMVal);
}

  
