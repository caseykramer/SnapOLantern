/* The Evil Mad Scientist Labs Snap-O-Lantern (or Peek-A-Book)
 * running on an Adafruit Trinket
 */
 
 #include <Adafruit_SoftServo.h>  // Adafruit SoftwareServo (works on non PWM pins)
 
 #define SERVOPIN 0    // Servo control line (orange) on Trinket Pin #0
 #define LEFT_EYE 1    // One eye is pin 1
 #define RIGHT_EYE 2   // The other is pin 2 (wow, shocking I know)
                       // Doesn't actually matter which is left or right, just makes it easier to refer to in code
 
 Adafruit_SoftServo myservo;
 
 void setup() {
   //OCROA = 0xAF;      // Apparently the number doesn't matter? So WTF is this?
   TIMSK |= _BV(OCIE0A); // Some interrupt thing
   
   myservo.attach(SERVOPIN);
   myservo.write(0);      // Start at 0 degrees
   
   pinMode(LEFT_EYE, OUTPUT);
   pinMode(RIGHT_EYE, OUTPUT);
   
   digitalWrite(LEFT_EYE, LOW);
   digitalWrite(RIGHT_EYE, LOW);
   
   randomSeed(analogRead(0)); // Initialize the RNG with noise from analog pin 0
 }

void loop() {
  // turn on eyes
  digitalWrite(LEFT_EYE,HIGH);
  digitalWrite(RIGHT_EYE,HIGH);
  // Open mouth slowly and snap shut
  sweepAndReset();
  // turn eyes off
  digitalWrite(LEFT_EYE,LOW);
  digitalWrite(RIGHT_EYE,LOW);
  
  // wait
  int delayTime = random(10,20) * 1000;
  delay(delayTime); // Delay some random interval between 10 and 20 seconds
  
}

void sweepAndReset() {
  int max_pos = 180;
  int pos = 0;
  
  // Open slowly
  while(pos < max_pos) {
    pos = pos + 5;
    myservo.write(pos);
    delay(100);
  }
  
  // then snap shut
  myservo.write(0);
}

// We'll take advantage of the built in millis() timer that goes off
// to keep track of time, and refresh the servo every 20 milliseconds
volatile uint8_t counter = 0;
SIGNAL(TIMER0_COMPA_vect) {
  // this gets called every 2 milliseconds
  counter += 2;
  // every 20 milliseconds, refresh the servos!
  if (counter >= 20) {
    counter = 0;
    myservo.refresh();
  }
 }
