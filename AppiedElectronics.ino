//期末專題

//函式庫
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,16,2);

/*Pin註解

A4  LCD SCL
A5  LCD SDA
D4  超音波echoPin
D6  RCServo
D8  超音波trigPin
D9  馬達PwmL
D10 馬達PwmR
D11 馬達DirL
D12 馬達DirR

*/

//定義接腳
#define trigPin   8
#define echoPin   4   
#define DirL 11
#define DirR 12
#define PwmL 9
#define PwmR 10

//變數宣告
float dist;
float speed = 0.0347; //declare speed of sound in air @ room temp;
float pingTime;       //declare variable for containing echo time;
Servo myS;

//定義函式--發射超音波&計算距離
void distance_calculate(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pingTime = pulseIn(echoPin, HIGH);  //讀取反彈聲波(echoPin)
  dist = pingTime*speed/2;            //計算距離
}

//定義函式--LCD顯示
void LCDview(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Distance:");
  lcd.print(dist);
}

//定義函式--車車眼睛(左轉)
void Eye_Left(){
  for(int i = 90; i <= 180; i=i+1){
    myS.write(i);
    delay(5);}
}
void Eye_Left_O(){
  for(int i = 180; i >=90; i=i-1){
    myS.write(i);
    delay(5);}
}

//定義函式--車車眼睛(右轉)
void Eye_Right(){
  for(int i = 90; i >=0; i=i-1){
    myS.write(i);
    delay(5);}
}
void Eye_Right_O(){
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
  //Serial.begin(9600);
  myS.attach(6); //RCServo接腳
  myS.write(90); //0-180
}

//程式迴圈
void loop(){

  //計算距離
  distance_calculate();
  LCDview();
  
  if(dist < 80){//前方有障礙物
    cM(HIGH,HIGH,0,0);//停車
    delay(200);
    Eye_Left();
    distance_calculate();
    LCDview();
    delay(1000);
    if(dist >= 80){//左側無障礙物
      cM(HIGH,HIGH,255,0);//左轉
      delay(500);
      cM(HIGH,HIGH,0,0);//停車
      delay(200);
      Eye_Left_O();
      distance_calculate();
      LCDview();
    }else{//左側有障礙物，掃右側
      cM(HIGH,HIGH,0,0);//停車
      delay(200);
      Eye_Left_O();
      Eye_Right();
      distance_calculate();
      LCDview();
      delay(1000);
      if(dist >= 80){//右側無障礙物
        cM(HIGH,HIGH,0,255);//右轉
        delay(1000);
        cM(HIGH,HIGH,0,0);
        delay(200);
        Eye_Right_O();
        distance_calculate();
        LCDview();
      }else{//前方和左右皆有障礙物
        cM(HIGH,HIGH,0,0);
        delay(200);
        Eye_Right_O();
        distance_calculate();
        LCDview();
        cM(LOW,LOW,255,255);
        delay(1000);
      }
    }
  }else{
    delay(1500);
    LCDview();
  }

  //車車直走
  cM(HIGH,HIGH,255,255);
  
}//程式碼結束
