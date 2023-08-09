
#include <SoftwareSerial.h>
int bluetoothTx = 2; // định nghĩa chân 2 là chân truyền tín hiệu 
int bluetoothRx = 3;// định nghĩa chân 3 là chân nhận tín hiệu
SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
   
//định nghĩa chân motor trái
#define enA 11
#define in1 6
#define in2 7

//định nghĩa chân  motor phai
#define enB 10
#define in3 8
#define in4 9

/*------------------------------------------------------------------*/
#define LENGTH 3                                                    
const int Trig = 12;     // chân trig của HC-05                
const int Echo = 13;     // chân echo của HC-05                
int f = 0;               //biến phục vụ việc lưu các biến khoảng cách để đem vào bộ lọc                                         
int input[LENGTH] = {};  //hàm lưu các biến khoảng cách để đem vào bộ lọc                                     
int Distance;           // biến lưu khoảng cách trước khi lọc              
int output=1000;       //biến lưu khoảng cách sau khi lọc                                           
/*------------------------------------------------------------------*/

char blue='S';// là vùng nhớ để so sánh kí tự trả về từ app.
char sos='y'; //kí tự báo có vật cản
const int spd=130; // giá trị băm xung( tay ga).
bool isForward = true; // biến trạng thái cho phép đi thẳng

/*----------------------------------------------------------------*/
unsigned long previousMillis_Buzz = 0;  // lưu trữ thời gian trước đó của hàm firstBuzz
unsigned long previousMillis_robot_android = 0; // lưu trữ thời gian trước đó của hàm robot_android

void setup() {
  bluetooth.begin(115200);    
  bluetooth.print("$$$");    
  delay(100);    
  bluetooth.println("U,9600,N");    
  bluetooth.begin(9600);
  Serial.begin(9600); 

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(Trig, OUTPUT);  // chân trig sẽ phát tín hiệu
  pinMode(Echo, INPUT);   // chân echo sẽ nhận tín hiệu
}

/********************************************************************************/
/*********************************************************************************/
int Buzz() {
  unsigned long Duration; // biến đo thời gian
  //digitalWrite(firstTrig, 0);  // tắt chân trig
  //delayMicroseconds(2);
  digitalWrite(Trig, 1);  // phát xung từ chân trig
  delayMicroseconds(3);   // xung có độ dài 4 microSeconds
  digitalWrite(Trig, 0);  // tắt chân trig
  Duration = pulseIn(Echo, HIGH);
  Distance = int(Duration / 2 / 29.412);
  return Distance;
}

int MA_filter(int input[]) {  //lọc tín hiệu nhiễu
  int sum = 0;
  for (int i = 0; i <= LENGTH; i++) {
    sum = sum + input[i];
  }
  output = sum / LENGTH;
  return output;
}

void readSensor(){
  for (int i = 0; i <= LENGTH; i++) {
    Buzz();
    input[f] = Distance;
    f++;
    delay(4);
  }
  MA_filter(input);
  f = 0;
}


/*liên quan đế điều khiển xe*/
// dieu khien motor trai
void motor_left(int spd, byte dir) 
{ 
   if (dir == 0) // motor trái quay tới
   { 
     digitalWrite(in2, LOW);
     digitalWrite(in1, HIGH);
   }
   else if (dir == 1) // motor trái quay lùi
   {
     digitalWrite(in2, HIGH);
     digitalWrite(in1, LOW);
   }
   else if (dir == 2)// motor trái đứng yên
   {
    digitalWrite(in1, LOW);
    digitalWrite(in2,LOW);
    }
   analogWrite(enA, spd );// băm xung cho motor trái
}

// dieu khien motor phai
void motor_right(int spd, byte dir) 
{ 
   if (dir == 0) // motor phải quay tới 
   { 
     digitalWrite(in4, LOW);
     digitalWrite(in3, HIGH);
   }
   else if (dir == 1)//motor phải quay lùi
   { 
     digitalWrite(in4, HIGH);
     digitalWrite(in3, LOW);
   }
   else if (dir ==2)//motor phải dứng lùi
   {
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    }
   analogWrite(enB, spd );
}

void forw () // chay thang
{
  //Serial.println("motor di thang");
    motor_right(spd-1.25 , 0);
    motor_left( spd , 0);
}

void back () // chay lui
{
  //Serial.println("motor di lui");
    motor_right(spd-1.25 , 1);
    motor_left( spd , 1);
}

void right () // quay phai
{
  //Serial.println("motor cua phai");
    motor_right(spd , 0);
    motor_left( spd , 2);
    
}
void left () // quay trai
{
  //Serial.println("motor cua trai");
    motor_left( spd , 0);
    motor_right( spd , 2);
}
void leftforw () // quay trai cham 
{
  //Serial.println("motor cua trai cham");
    motor_left( spd-20 , 0);
    motor_right( spd , 0);
}
void rightforw () // quai phai cham
{
  //Serial.println("motor cua phai cham ");
    motor_left( spd , 0);
    motor_right( spd-20 , 0);
}
void rightback () // lui phai cham
{
  //Serial.println("motor lui phai cham ");
    motor_left( spd , 1);
    motor_right( spd-20 , 1);
}
void leftback () // lui trai cham
{
  //Serial.println("lui trai cham ");
    motor_left( spd-20 , 1);
    motor_right( spd , 1);
}
void tack()
{
  motor_left( spd , 2);
  motor_right( spd , 2);
}

void robot_android(){
  if ( bluetooth.available() )
  {
      blue =bluetooth.read();
      Serial.println("blue la   ");
      Serial.println(blue);
  }
  if(isForward==true){
    switch (blue){  

      case ('S'):
      tack();
      break;
      case ('F'): 
      forw();
      break;
      case ('B'): 
      back();
      break;
      case ('L'): 
      left();
      break;
      case ('R'): 
      right();
      break;
      case ('H'): 
      leftback();
      break;
      case ('J'): 
      rightback();
      break;
      case ('I'): 
      rightforw();
      break;
      case ('G'): 
      leftforw();
      break;
    }
  }
  if (isForward==false){
    switch (blue){  

      case('F'):
      break;
      case ('S'):
      tack();
      break;
      case ('B'): 
      back();
      break;
      case ('H'): 
      leftback();
      break;
      case ('J'): 
      rightback();
      break;
    }
  }
}

void warning() {
  Serial.println(output);
  if ( output <= 26) {
    if(isForward==true){
      while(sos!='x'){
        sos =bluetooth.read();
        tack();
      }
      sos='y';
    }
    isForward = false;
  }
  if(output >= 27){
    isForward = true;
  }
}

void loop() {  
  /**Xử lý tín hiệu trên cảm biến HC 05*/

  unsigned long currentMillis = millis();  // lấy thời gian hiện tại
  // thực hiện hàm robot_android với chu kì 200ms
  if (currentMillis - previousMillis_robot_android >= 10) {
    robot_android();  // gọi hàm điều khiển xe
    previousMillis_robot_android = currentMillis;  // cập nhật lại thời gian trước đó
  }
  // thực hiện hàm Buzz với chu kì 100ms
  if (currentMillis - previousMillis_Buzz >= 330) {
    //Buzz();  // gọi hàm đọc cảm biến
    readSensor();
    warning();
    previousMillis_Buzz = currentMillis;  // cập nhật lại thời gian trước đó
  }

}
