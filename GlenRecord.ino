#define SHORT_PRESS 10
#define LONG_PRESS 50
#define LONG_LONG_PRESS 100
const int RecLightPin=7;
const int RecPin=5;
const int PlayPin=4;
const int LightPin=3;
int light_val=0;

bool rec_mode=false;
bool play_mode=false;

int rec_pressed_time=0;
int play_pressed_time=0;

// send 1: rec, 2: play, 3: check, 4: reset


void setup(){
  
  pinMode(RecPin,INPUT);
  pinMode(PlayPin,INPUT);
  pinMode(LightPin,OUTPUT);
  pinMode(RecLightPin,OUTPUT);

  Serial.begin(9600);
   
}

void loop(){

 
  
  if(checkReset()){
    delay(2000);
    return;
  }
  
  if(digitalRead(RecPin)==HIGH){    
    rec_pressed_time++;
  }else{    
    if(rec_pressed_time>SHORT_PRESS) Serial.println("1|");     
    rec_pressed_time=0;
  }
  
  if(digitalRead(PlayPin)==HIGH){
    play_pressed_time++;
  }else{
    if(play_pressed_time>LONG_PRESS) Serial.println("3|");      
    else if(play_pressed_time>SHORT_PRESS) Serial.println("2|");         
    
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
    
  delay(10);
  
}

bool checkReset(){
  if(play_pressed_time>LONG_LONG_PRESS && rec_pressed_time>LONG_LONG_PRESS){
    Serial.println("4|");    
    play_pressed_time=rec_pressed_time=0;    
    return true;
  }
  return false;
}



