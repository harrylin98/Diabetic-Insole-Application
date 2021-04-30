#include <SoftwareSerial.h>

SoftwareSerial hc06(2,3);

String cmd="";
String data = "";
double IdealWH,IdealWF,IdealWM,IdealSH,IdealSF,IdealSM;
int mode =0;

int midfoot_pressure = A0;
int heelfoot_pressure = A1;
int pump = A2;
int heelvalve = A3;
int midfootvalve = A4;
int vent = A5;

void setup(){
  //Initialize Serial Monitor
  Serial.begin(9600);
  //Initialize Bluetooth Serial Port
  hc06.begin(9600);
  pinMode(pump,OUTPUT);
  pinMode(heelvalve,OUTPUT);
  pinMode(midfootvalve,OUTPUT);
  pinMode(vent,OUTPUT);
}

void loop(){

  int mid_p = analogRead(midfoot_pressure);
  float midfoot = map(mid_p,0,1023,0,1000);

  int mid_h = analogRead(heelfoot_pressure);
  float heel = map(mid_h,0,1023,0,1000);
  
    //Read data from HC06
  while(hc06.available()){
    cmd+=(char)hc06.read();
  }
  if(cmd!=""){
    data += cmd;
    cmd=""; //reset cmd
    if (data.charAt(data.length()-1) == 'g')
    {
          if (data.equals("Standing"))
          {
            Serial.println ("Standing");
            mode = 1;
          }
          else if (data.equals("Walking"))
          {
            Serial.println ("Walking");
            mode = 0;
          }
          else
          {
            int start = data.indexOf("IdealW");
            data.remove(0,start);

            IdealWH = data.substring(7,13).toDouble();
            IdealWF = data.substring(14,20).toDouble();
            IdealWM = data.substring(20,27).toDouble();
            Serial.println("IdealWH: " + (String)IdealWH + " IdealWF: " + (String)IdealWF + " IdealWM: " + (String)IdealWM);
     
            start = data.indexOf("IdealS");
            data.remove(0,start);

            IdealSH = data.substring(7,13).toDouble();
            IdealSF = data.substring(14,20).toDouble();
            IdealSM = data.substring(20,26).toDouble();
            Serial.println("IdealSH: " + (String)IdealSH + " IdealSF: " + (String)IdealSF + " IdealSM: " + (String)IdealSM);
          }
          
          data = "";
    }

  }

  double M,H;
  
  if (mode == 1) //standing
  {
    M = IdealSM;
    H = IdealSH;
  }

   if (mode == 0) //walking
   {
      M = IdealWM;
      H = IdealWH;
    }
    
    if(midfoot < M)
  {
    digitalWrite(midfootvalve,HIGH);
    digitalWrite(pump,HIGH);
    Serial.println("trun on midfoot valve and pump");
  }
  else
  {
    if (heel > H)
    {
      digitalWrite(midfootvalve,LOW);
      digitalWrite(pump,LOW);
      Serial.println("trun off midfoot valve and pump");
    }
    
  }
  
    if(heel < H)
  {
    digitalWrite(heelvalve,HIGH);
    digitalWrite(pump,HIGH);
  }
  else
  {
    if (midfoot > M)
    {
    digitalWrite(heelvalve,LOW);
    digitalWrite(pump,LOW);
    }
  }

  
  delay(100);
  


  
}
