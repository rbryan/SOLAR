#include <SoftwareSerial.h>

#define PIN 2

#define CMD_LEN 3

#define TERM '!'
#define BRAKE_CMD     "BRK"
#define NO_BRAKE_CMD  "NBR"
#define WAIT          "WAIT"
#define WUT            "UWUTM8"
#define NCMD            "NOTACMD"
#define ICMD            "INCMD"
#define NICMD          "NOTINCMD"
#define OOCMD          "OUTOFCMD"

volatile int state = LOW;
int in_command = 0;
char buffer[60]={0};
int buffer_index;

void setup()
{
  pinMode(PIN, OUTPUT);
  attachInterrupt(0, blink, CHANGE);

  Serial.begin(57600);
}

void loop()
{
  int l;
  
  digitalWrite(PIN, state);

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

void blink()
{
  state = !state;
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

