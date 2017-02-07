#define SHORT_PRESS 10
#define LONG_PRESS 60
#define LONG_LONG_PRESS 500
#define SHORT_INTER 40

const int RecLightPin=7;
const int RecPin=5;
const int PlayPin=4;
const int LightPin=3;
int light_val=0;

bool rec_mode=false;
bool play_mode=false;

int rec_pressed_time=0;
int play_pressed_time=0;

int suprise_count=0;
int inter_time=0;

int play_long_delay=0;

// send 1: rec, 2: play, 3: check, 4: reset, 7:suprise


void setup(){
  
  pinMode(RecPin,INPUT);
  pinMode(PlayPin,INPUT);
  pinMode(LightPin,OUTPUT);
  pinMode(RecLightPin,OUTPUT);

  Serial.begin(9600);
   
}

void loop(){

  
  if(digitalRead(RecPin)==HIGH){    
    rec_pressed_time++;
    if(checkReset()){
      delay(2000);         
      return;
    }
    
  }else{  
    if(rec_pressed_time>SHORT_PRESS){      
      
      if(inter_time<SHORT_INTER){
        suprise_count++;    
        
        if(suprise_count>=6){
          Serial.println("7|");
          suprise_count=0;
          delay(2000);
        }    
      }else{
        Serial.println("1|");         
        suprise_count=0;
      }

      inter_time=0;
      
    }
    
    rec_pressed_time=0;
  }

  
  
  if(digitalRead(PlayPin)==HIGH){
    play_pressed_time++;    

    if(play_pressed_time>LONG_PRESS && digitalRead(RecPin)==LOW){
        Serial.println("3|");      
//        play_long_delay=LONG_PRESS;
        play_pressed_time=0; 
        suprise_count=0;       
        delay(2000);         
    }
  }else{
    if(play_pressed_time>SHORT_PRESS){
        Serial.println("2|");             
       suprise_count=0;
    }
    play_pressed_time=0;    
    
  }
  
  if(Serial.available()){
    String str_=Serial.readStringUntil('|');
    if(str_=="a"){
      digitalWrite(RecLightPin,HIGH);
    }else if(str_=="b"){
      digitalWrite(RecLightPin,LOW);
    }else{    
      if(str_.length()>1){
        light_val=constrain(str_.toInt(),0,255);      
      }
    }
  }
  analogWrite(LightPin,light_val);

  inter_time++;
  if(play_long_delay>0) play_long_delay--;
  
  delay(10);
  
}

bool checkReset(){
  if(rec_pressed_time>LONG_LONG_PRESS && play_pressed_time>LONG_LONG_PRESS){ 
      Serial.println("4|");    
      play_pressed_time=rec_pressed_time=0;    
      suprise_count=0;      
    return true;
  }
  return false;
}



