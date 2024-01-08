//Pin definition
#define PIN_PWM 3
#define PIN_BTN 2

//Min/max pwm
#define LED_OFF 0
#define LED_MAXPWM 200

int state = 0;
bool interrupt = false;

void setup() {
  analogWrite(PIN_PWM, LED_OFF); // Avoid unexpected LED flashes during firmware loading and during reset

  pinMode(PIN_PWM, OUTPUT);     // Set pin 3 as the output
  analogWrite(PIN_PWM, LED_OFF);// Set pwm to 0

  //Set pwm frequency to 2 kHz
  TCCR2B = 0b00000011;
  TCCR2A = 0b00000011;

  attachInterrupt(digitalPinToInterrupt(PIN_BTN), changeState, FALLING);
}

void loop() {
  if(interrupt){
    state++;
    if(state > 5)
      state = 0;

    interrupt = false;
  }

  if(state == 0)
    turnOffLed();
  else if(state == 1)
    turnOnLed(50); //50% power
  else if(state == 2)
    turnOnLed(80); //80% power
  else if(state == 3)
    turnOnLed(100); //100% power
  else if(state == 4)
    classicBlink(600, 60);
  else if(state == 5)
    fadeBlink();
}

void turnOffLed(){
  analogWrite(PIN_PWM, LED_OFF);
  delay(50);
}

void turnOnLed(int percent){
  analogWrite(PIN_PWM, getPwmValue(percent));
  delay(50);
}

void classicBlink(int dl, int percent){
  analogWrite(PIN_PWM, LED_OFF);
  delay(dl);
  analogWrite(PIN_PWM, getPwmValue(percent));
  delay(dl);
}

void fadeBlink(){
  for(int i = 0; i<100;i++){
    analogWrite(PIN_PWM, getPwmValue(i));
    delay(4);
  }

  for(int i = 100; i>0;i--){
    analogWrite(PIN_PWM, getPwmValue(i));     
    delay(4);
  }

  for(int i = 0; i<100;i++){
    analogWrite(PIN_PWM, getPwmValue(i));
    delay(4);
  }

  for(int i = 100; i>0;i--){
    analogWrite(PIN_PWM, getPwmValue(i));     
    delay(8);
  }
}

byte getPwmValue(int pc){
  byte res = map(pc, 0, 100, 0, LED_MAXPWM);
  return res;
}

void changeState(){
  interrupt = true;
}