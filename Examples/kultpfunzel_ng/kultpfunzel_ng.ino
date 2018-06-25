/*
  Read analog values from a potentiometer
*/


#define switchPin 8 // The switch "SW" is on digital pin 8
#define ledPin 10 // The LED is on digital pin 10

unsigned int current_level, target_level;

void setup ()
{
  initPfunzel();
  pinMode(13, OUTPUT);	// that's the arduino standard LED 
}

void loop () {
  delay(200);
  digitalWrite(13, digitalRead(13) ^ 1);   // toggle LED pin
}

ISR(TIMER3_COMPA_vect)          // timer compare interrupt service routine
{
  if (isSwitch()) {
    if (current_level < 255) {
      current_level += 1;
    }
  }
  analogWrite (ledPin, current_level);
}

// Helper function from original Pfunzel sketch
// as readDigital(switchPin) doesn't work...
boolean isSwitch() {
  return PINE & _BV(2); // reads from pin no. 8,
}


void initPfunzel() {
  pinMode(switchPin, INPUT);
  pinMode(ledPin, OUTPUT);
  current_level = 0;

  // initialize timer3
  noInterrupts();           // disable all interrupts
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3  = 0;

  OCR3A = 32;	            // compare match register
  TCCR3B |= (1 << WGM12);   // CTC mode
  TCCR3B |= (1 << CS12);    // 256 prescaler 
  TIMSK3 |= (1 << OCIE3A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts
}

