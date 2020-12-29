//期末專題

//函式庫
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,16,2);

/*Pin註解

A4 LCD SCL
A5 LCD SDA



*/

//定義接腳
#define trigPin   8   //declare pin for trigger pin of UltraSonic sensor;
#define echoPin   4   //declare pin for echo pin of UltraSonic sensor;

//車車馬達
#define DirL 11
#define DirR 12
#define PwmL 9
#define PwmR 10

//變數宣告
long unsigned duration;
float dist;
float speed = 0.0347; //declare speed of sound in air @ room temp;
float pingTime;       //declare variable for containing echo time;
Servo myS;

//定義函式--計算距離
void distance_calculate(){
  duration = pulseIn(echoPin, HIGH);  //讀取反彈聲波(echoPin)
  dist = duration*0.034/2;            //計算距離
}

//定義函式--車車眼睛(左轉)
void Eye_Left(){
  for(int i = 90; i <= 180; i=i+1){
    myS.write(i);
    delay(5);}
  for(int i = 180; i >=90; i=i-1){
    myS.write(i);
    delay(5);}
}

//定義函式--車車眼睛(右轉)
void Eye_Right(){
  for(int i = 90; i >=0; i=i-1){
    myS.write(i);
    delay(5);}
  for(int i = 0; i <=90; i=i+1){
    myS.write(i);
    delay(5);}
}

//定義函式--直走(H)&後退(L)&轉彎
void cM(int dL,int dR,int sL,int sR){
  digitalWrite(PwmL,0);
  digitalWrite(PwmR,0);
  delay(200); 
  digitalWrite(DirL,dL);
  digitalWrite(DirR,dR);
  analogWrite(PwmL,sL);
  analogWrite(PwmR,sR);
}

//程式開始--初始化
void setup() {
  pinMode(DirL,OUTPUT);
  pinMode(DirR,OUTPUT);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  myS.attach(6); //RCServo接腳
  myS.write(90); //0-180
}

//程式迴圈
void loop(){

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  //車車直走
  
  //計算距離
  distance_calculate();
  
  if(dist < 50){
    Eye_Left();
    //delay(500);
    Eye_Right();
    cM(LOW,LOW,255,255);;//後退0.2秒
  }else{
    cM(HIGH,HIGH,255,255);//前進0.2秒
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Distance:");
  lcd.print(dist);
  delay(100);
  
/*   不同距離指令
  if(dist<=10.0){

  } 
  else if(dist<=30.0 && dist>10.0)
  {

  }
  else if((dist>30.0) && (dist<60.0))
  {

  }
  else if(dist>=60.0 && dist<120.0)
  {

  }
*/ 

}
