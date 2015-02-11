#include <SoftwareSerial.h>

#define TERM '!'
#define BRAKE_CMD     "BRK"
#define NO_BRAKE_CMD  "NBR"
#define WAIT          "WAIT"
#define WUT            "UWUTM8"
#define NCMD            "NOTACMD"
#define ICMD            "INCMD"
#define NICMD          "NOTINCMD"
#define OOCMD          "OUTOFCMD"

int in_command = 0;
char buffer[60]={0};
int buffer_index;

const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);    
}

void loop(){

  parse_commands();
  
    // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {    
    // turn LED on:    
    digitalWrite(ledPin, HIGH);  
  }
  else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
}

void parse_commands(){
  int l;
  if((l = Serial.available())){
    
     if(l > 60)
       send(WAIT);
       
     for(int j=0; j<l; j++){
       if(!(buffer_index < 60)){
         send(WAIT);
         clear_buffer();
         buffer_index=0;
       }
         
        char c = Serial.read();
       switch(c){
          case '<':
            if(in_command){
              send(ICMD);
            }
            in_command = 1;
            break;
          case '>':
            if(!in_command){
               send(NICMD); 
            }else if(handle_serial()){ 
              send(NCMD);
              clear_buffer();
              buffer_index=0;
            }else{
              clear_buffer();
              buffer_index=0;
            }
            in_command = 0;
            break;
          default:
            if(in_command){
              buffer[buffer_index] = c;
              buffer_index++;
            }else{
              send(OOCMD);
            }
            
       } 
     }
  }
}

void send(const char *cmd){
   Serial.print("<");
   Serial.print(cmd);
   Serial.print(">"); 
}

void clear_buffer(){
   for(int i=0; i<60; i++){
      buffer[i] = 0;
   } 
}

int handle_serial(){
   if(strcmp(buffer,BRAKE_CMD)==0){
     if(!state)
       blink();
     return 0;
   }else if(strcmp(buffer,NO_BRAKE_CMD)==0){
     if(state)
       blink();
     return 0;
   }else if(strcmp(buffer,WAIT)==0){
      //do something
     return 0; 
   }
   return -1;
}


