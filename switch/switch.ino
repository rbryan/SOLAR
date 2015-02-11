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
#define TSLEFTON          "Left Turn Signal: On"
#define TSLEFTOFF          "Left Turn Signal: Off"
#define TSRIGHTON          "Right Turn Signal: On"
#define TSRIGHTOFF          "Right Turn Signal: Off"
#define HAZARDON            "Hazards: On"
#define HAZARDOFF            "Hazards: Off"
#define TSSLT                "Turn Signal Switch: Left Turn"
#define TSSRT                "Turn Signal Switch: Right Turn"
#define TSSOFF                "Turn Signal Switch: Off"
#define TSSOS                "Turn Signal Switch: Oh shit!"

int in_command = 0;
char buffer[60]={0};
int buffer_index;

//We should be able to do something clever so that this
//only uses two pins...
const int left_turn_switch_pin = 2;
const int right_turn_switch_pin = 3;
const int off_turn_switch_pin = 4;

int left_signal_state;
int right_signal_state;
int off_signal_state;

void setup() {
  pinMode(left_turn_switch_pin, INPUT);   
  pinMode(right_turn_switch_pin, INPUT);  
  pinMode(off_turn_switch_pin, INPUT);   
}

void loop(){

  parse_commands();
  
  //None of this is very clever but I'm trying to write
  //robust and readable code here.
  int left_signal_changed = left_signal_state;
  int right_signal_changed = right_signal_state;
  int off_signal_changed = off_signal_state;
  
  left_signal_state = digitalRead(left_turn_switch_pin);
  right_signal_state = digitalRead(right_turn_switch_pin);
  off_signal_state = digitalRead(off_turn_switch_pin);
  
  if(left_signal_changed == left_signal_state) left_signal_changed = 0;
  if(right_signal_changed == right_signal_state) right_signal_changed = 0;
  if(off_signal_changed == off_signal_state) off_signal_changed = 0;
  
  if (left_signal_changed && left_signal_state) {    
     send(TSSLT);
  }
  if (right_signal_changed && right_signal_state) {    
     send(TSSRT);
  }
  if (off_signal_changed && off_signal_state) {    
     send(TSSOFF);
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
      goto DONE;
   }else if(strcmp(buffer,NO_BRAKE_CMD)==0){
      goto DONE;
   }else if(strcmp(buffer,WAIT)==0){
      goto DONE;
   }

DONE:
   return -1;
}


