/* Turn signal and Brake light controller
 * For Sooner Solar Racing Team
 * Target Device: Arduino Pro Micro 5V 16MHz
 * 
 * Written by: Connor Taylor
 * Last edited: Feb 3, 2015
 */
 
//Temporary until we get serial stuff working
#define LEFT_TURN_SIGNAL false
#define RIGHT_TURN_SIGNAL true

//Pin allocations
#define LEFT_TURN_LIGHT 9    //Output compare match 1A
#define RIGHT_TURN_LIGHT 10  //Output compare match 1B
#define BRAKE_LIGHT 16
#define BRAKE_SIGNAL 14      //External interrupt trigger input 0

void setup(){
  //Set pin modes
  pinMode(LEFT_TURN_LIGHT, OUTPUT);
  pinMode(RIGHT_TURN_LIGHT, OUTPUT);
  pinMode(BRAKE_LIGHT, OUTPUT);
  pinMode(BRAKE_SIGNAL, INPUT);
  
  //Set initial states
  digitalWrite(LEFT_TURN_LIGHT, LOW);
  digitalWrite(RIGHT_TURN_LIGHT, LOW);
  digitalWrite(BRAKE_LIGHT, LOW);
  
  //Attach the brake ISR to INT0. Use falling edge to trigger
  attachInterrupt(0, brake, FALLING);
  
  //Setup Timer 1
  setupTimer1();
}

void loop(){
  if(LEFT_TURN_SIGNAL)
    setDutyCycleA(0.5);
  else
    setDutyCycleA(0);
    
  if(RIGHT_TURN_SIGNAL)
    setDutyCycleB(0.5);
  else
    setDutyCycleB(0);
    
  //if(digitalRead(BRAKE_SIGNAL))
    //digitalWrite(BRAKE_LIGHT, LOW);
}

void setDutyCycleA(float duty){
  //Error checking
  if(duty > 1)
    return;
    
  //invert duty cycle
  duty = 1-duty;
  
  //Set duty cycle
  OCR1AH = highByte((word)(duty*62500));
  OCR1AL = lowByte((word)(duty*62500));
}

void setDutyCycleB(float duty){
  //Error checking
  if(duty > 1)
    return;
    
  //invert duty cycle
  duty = 1-duty;
  
  //Set duty cycle
  OCR1BH = highByte((word)(duty*62500));
  OCR1BL = lowByte((word)(duty*62500));
}

//Brake light ISR
void brake(){
  digitalWrite(BRAKE_LIGHT, HIGH);
}

void setupTimer1(){
  //Timer/Counter1 Control Register A
  TCCR1A = 0;
  TCCR1A |= (1 << COM1A1);
  TCCR1A |= (1 << COM1A0);
  TCCR1A |= (1 << COM1B1);
  TCCR1A |= (1 << COM1B0);
  TCCR1A |= (1 << WGM11);    //CTC ICR, fast PWM
  
  //Timer/Counter1 Control Register B
  TCCR1B = 0;
  TCCR1B |= (1 << WGM12);    //CTC ICR, fast PWM
  TCCR1B |= (1 << WGM13);    //CTC ICR, Fast PWM
  TCCR1B |= (1 << CS12);    //62500Hz
  
  //Timer/Counter1 Control Register C
  TCCR1C = 0;
  
  //Timer/Counter1 
  TCNT1H = 0;
  TCNT1L = 0;
  
  //Input Capture Register 1 (used for setting CTC point
  ICR1H = highByte(62500);
  ICR1L = lowByte(62500);
  
  //Set both outputs to 
  OCR1AH = highByte(62500);
  OCR1AL = lowByte(62500);
  OCR1BH = highByte(62500);
  OCR1BL = lowByte(62500);
}
