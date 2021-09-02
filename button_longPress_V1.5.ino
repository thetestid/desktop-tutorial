#include <TM1637Display.h>
#include <ezButton.h>
#include <Adafruit_NeoPixel.h>

#define CLK 2// FND CLK 입력
#define DIO 3// FND DIO 입력

#define speakerpin 11

TM1637Display display(CLK, DIO); // 

unsigned long timeLimit = 0;
unsigned long study_time=0;
unsigned long break_time=0;

int loop_count=-1;
int number = -1;

#define UP_btn_pin 8
#define DOWN_btn_pin 9
#define LOOP_btn_pin 10

#define numberOfHours(_time_) ( ( (_time_ / 1000) / 60) / 60  ) // 시간

#define numberOfMinutes(_time_)(   (_time_ / 1000) / 60        ) // 분

#define numberOfSeconds(_time_) (   (_time_ / 1000) % 60        ) // 초



#define PIN        6 // 네오픽셀 핀 6 설정
#define NUMPIXELS 24 // 네오픽셀 갯수
#define BRIGHT 10 // 네오픽셀 밝기
#define BRIGHTNESS 10  

Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


const int SHORT_PRESS_TIME = 1000; // 1000 milliseconds
const int LONG_PRESS_TIME  = 1000; // 1000 milliseconds

ezButton button_timeUP(UP_btn_pin);  // create ezButton object that attach to pin 7;
ezButton button_timeDOWN(DOWN_btn_pin);  // create ezButton object that attach to pin 7;
ezButton button_LOOP(LOOP_btn_pin);

unsigned long pressedTime  = 0;
unsigned long releasedTime = 0;
bool isPressing_UPbtn = false;      //detection of UP button press
bool isPressing_DOWNbtn = false;    //detection of DOWN button press
bool isLongDetected_UPbtn = false;  //detection of UP button long press
bool isLongDetected_DOWNbtn = false;//detection of DOWN button long press
bool isPressing_LOOPbtn = false;      //detection of LOOP button press

const uint8_t DONE[] = //7세그먼트 시간 끝나면 done이라고 뜸
{
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_C | SEG_E | SEG_G,                           // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
  };

void setup() {
  Serial.begin(9600);
  strip.begin(); //네오픽셀 시작
  strip.setBrightness(BRIGHT);
  
  button_timeUP.setDebounceTime(50); // set debounce time to 50 milliseconds
  button_timeDOWN.setDebounceTime(50); // set debounce time to 50 milliseconds
  button_LOOP.setDebounceTime(50); // set debounce time to 50 milliseconds
  
  display.setBrightness(1);// FND 밝기조정   0~7
  display.showNumberDecEx(0, 0x40, true);

     rainbowCycle(10);
   strip.show();

   timeLimit=0;
}

void neo_1()
{
   for (long i = study_time; i > 0; i=i-1000)   // data type을 long 으로
  {    
    int hours   = numberOfHours(i);
    int minutes = numberOfMinutes(i)-(numberOfHours(i)*60);
    if (minutes >=60) // 80분으로 나오는거 방지
      minutes =60 - minutes;

    int seconds = numberOfSeconds(i);
   

    
    Serial.println((String) "남은시간 : " + i);                             // 시리얼 모니터에서 시간을 볼수 있슴
    
    display.showNumberDecEx(minutes, 0, true, 2, 2);                 // 분
    display.showNumberDecEx(hours, (0x80 >> 1), true, 2, 0) ;        // 시간
                                                                     // 콜론 켜기    원본은  0x80 >> 3 인데 콜론이 on 되지 않아 0x80 >> 1 로 변경

    int x =(i / (study_time / 24)  );                                   // 남은 시간동안 led 켜기

    uint32_t c = strip.Color(255, 0 , 0);                         // 네오픽셀 빨간색, 색깔짙게, 밝기3
    strip.fill(c,0,x);                                               // c를 0번 led ~ x번 led까지 채우세용
    //ColorHSV(0, 255 , 10);                                                                 // 남은 시간 동안 led 켜기
    
    Serial.println( (String) "남은 led : " + x);                           // 시리얼 키자                                          

    
    strip.show();                                                    //네오픽셀 보여주기
    strip.clear();                                                   //네오픽셀 새로고침 

    
    delay(1000);
  }


    
  
      
   display.setSegments(DONE); //끝났다고 뜨기
    SSound();
   delay(2000);
   timeLimit = 0;
  }

  void neo_2()
{
   for (long i = break_time; i > 0; i=i-1000)   // data type을 long 으로
  {    
    int hours   = numberOfHours(i);
    int minutes = numberOfMinutes(i)-(numberOfHours(i)*60);
    if (minutes >=60) // 80분으로 나오는거 방지
      minutes =60 - minutes;

    int seconds = numberOfSeconds(i);
   

    
    Serial.println((String) "남은시간 : " + i);                             // 시리얼 모니터에서 시간을 볼수 있슴
    
    display.showNumberDecEx(minutes, 0, true, 2, 2);                 // 분
    display.showNumberDecEx(hours, (0x80 >> 1), true, 2, 0) ;        // 시간
                                                                     // 콜론 켜기    원본은  0x80 >> 3 인데 콜론이 on 되지 않아 0x80 >> 1 로 변경

    int x =(i / (break_time / 24)  );                                   // 남은 시간동안 led 켜기

   
    uint32_t c = strip.Color(255, 0 , 0);                         // 네오픽셀 빨간색, 색깔짙게, 밝기3
    strip.fill(c,0,x);                                               // c를 0번 led ~ x번 led까지 채우세용
    //ColorHSV(0, 255 , 3)                                                                 // 남은 시간 동안 led 켜기
    
    Serial.println( (String) "남은 led : " + x);                           // 시리얼 키자                                          

    
    strip.show();                                                    //네오픽셀 보여주기
    strip.clear();                                                   //네오픽셀 새로고침 

    
    delay(1000);
  }


    
  
      
   display.setSegments(DONE);//끝났다고 뜨기
   SSound();
   delay(2000);
   timeLimit = 0;
  }

void loop() {
  // MUST call the loop() function first
  button_timeUP.loop();
  button_timeDOWN.loop();
  button_LOOP.loop();
  //===================essential loop function area END===================//

//=================timer UP button Area Start===========================//
  if(button_timeUP.isPressed()) //when the button is pressed
  {
    pressedTime = millis();     //현재 시간 체크
    isPressing_UPbtn = true;    //시간 증가 버튼 눌렸음을 부울 대수 변수에 반영
    isLongDetected_UPbtn = false;//아직 장기 입력인지 판별되지 않았으므로 거짓
  }

  if(button_timeUP.isReleased()) { //when the button is released
    isPressing_UPbtn = false;     //버튼이 입력이 중지되었으므로 false 반영
    releasedTime = millis();      //현재 시간 체크

    long pressDuration = releasedTime - pressedTime;   
    //누른 시간에서 뗀 시간 차이 계산, 총 눌린 시간 반영


//=========총 눌린 시간이 설정해놓은 최대 단기 입력 인지 시간보다 작을 때==========//
    if( pressDuration < SHORT_PRESS_TIME )
    {
      Serial.println("A short press is detected(UP)");
      //단기 입력 감지 되었다는 문자열 출력

      //===========시간 증가 연산 부분 시작==================//

      if(timeLimit >=0 && timeLimit <= 36234000){ 
          //36,234,000 = 99hours 99minutes , To Prevent overflow and underflow
          //36,234,000 = 99시간 99분, 오버플로 언더플로 방지용 범위 설정
          if(timeLimit > 36234000){
            //Prevent overflow 오버플로 시
            timeLimit = 0;//시간 설정을 0으로
            Serial.println((String)"overflow detected");
            
          }

          //실질 시간 증가계산 부분, 증가 버튼영역에서 언더플로 방지 코드는 불필요
          else{
            timeLimit = timeLimit + 60000; //1000 * 60 = 1 minute
            Serial.println((String) "설정시간 : " + timeLimit );
            display.showNumberDecEx(numberOfMinutes(timeLimit)-(numberOfHours(timeLimit)*60),0,true,2,2);
            display.showNumberDecEx(numberOfHours(timeLimit),(0x80 >> 1),true,2,0);
          }
        }
        else//오버, 언더플로시 동작 미 시행
          ;
      //control timeLimit(plus) end
    }
    //===============총 눌린 시간이 설정해놓은 최대 단기 입력 인지 시간보다 작을 때 영역 종료==============//
  }


//==========버튼이 눌렸으나 장기 입력이 감지되지 않았을 때 영역 시작(UP button)===================//
  if(isPressing_UPbtn == true && isLongDetected_UPbtn == false) {
    long pressDuration = millis() - pressedTime;   //현재시각 - 누른 시각 측정

    if( pressDuration > LONG_PRESS_TIME ) { 
      //장기 입력 인지 완료, 연속 시간 증가 시작
      Serial.println("A long press is detected(UP)");
      isLongDetected_UPbtn = true;
      while(1)
      {
        timeLimit = timeLimit + 60000; //1000 * 60 = 1 minute
        Serial.println((String) "설정시간 : " + timeLimit );
        display.showNumberDecEx(numberOfMinutes(timeLimit)-(numberOfHours(timeLimit)*60),0,true,2,2);
        display.showNumberDecEx(numberOfHours(timeLimit),(0x80 >> 1),true,2,0);
        delay(100);
        if(digitalRead(UP_btn_pin)==1)
        {
          isPressing_UPbtn = false;
          releasedTime = millis();
          break;
        }
      }
    }
  }
  //=========버튼이 눌렸으나 장기 입력이 감지되지 않았을 때 영역(UP button) 시작===================//

  //======================timer DOWN button Area===========================//

  if(button_timeDOWN.isPressed()) //when the button is pressed
  {
    pressedTime = millis();
    isPressing_DOWNbtn = true;
    isLongDetected_DOWNbtn = false;
  }

  if(button_timeDOWN.isReleased()) { //when the button is released
    isPressing_DOWNbtn = false;
    releasedTime = millis();

    long pressDuration = releasedTime - pressedTime;

//============버튼 단기 입력 시 시간 감소 부분===========//
    if( pressDuration < SHORT_PRESS_TIME )
    {
      Serial.println("A short press is detected(DOWN)");

      //control timeLimit(minus) start

      if(timeLimit >=0 && timeLimit <= 36234000){
          //36,234,000 = 99hours 99minutes

          if(timeLimit < 60000){
            //Prevent underflow
            timeLimit = 0;
            Serial.println((String)"underflow detected");
          }
          else{
            timeLimit = timeLimit - 60000; //1000 * 60 = 1 minute
            Serial.println((String) "설정시간 : " + timeLimit );
            display.showNumberDecEx(numberOfMinutes(timeLimit)-(numberOfHours(timeLimit)*60),0,true,2,2);
            display.showNumberDecEx(numberOfHours(timeLimit),(0x80 >> 1),true,2,0);
          }
        }
        else
          ;
      //control timeLimit (minus) end
    }
  }

//==========버튼 장기 입력 시 시간 감소 부분=========// 
  if(isPressing_DOWNbtn == true && isLongDetected_DOWNbtn == false) {
    long pressDuration = millis() - pressedTime;

    if( pressDuration > LONG_PRESS_TIME ) {
      //감소 버튼 장기 입력 인지 완료, 연속 시간 감소 시작
      Serial.println("A long press is detected(DOWN)");
      isLongDetected_DOWNbtn = true;

      while(1)
      {
        timeLimit = timeLimit - 60000; //1000 * 60 = 1 minute
        Serial.println((String) "설정시간 : " + timeLimit );
        display.showNumberDecEx(numberOfMinutes(timeLimit)-(numberOfHours(timeLimit)*60),0,true,2,2);
        display.showNumberDecEx(numberOfHours(timeLimit),(0x80 >> 1),true,2,0);
        delay(100);
        if(digitalRead(DOWN_btn_pin)==1)
        {
          isPressing_DOWNbtn = false;
          releasedTime = millis();
          break;
        }
      }
    }
  }
  --------------------------------//loop//---------------------
    if(button_LOOP.isPressed()) //when the button is pressed
  {
    pressedTime = millis();     //현재 시간 체크
    isPressing_LOOPbtn = true;    //루프 버튼 눌렸음을 부울 대수 변수에 반영
    
  }

  if(button_LOOP.isReleased()) { //when the button is released
    isPressing_LOOPbtn = false;     //버튼이 입력이 중지되었으므로 false 반영
    releasedTime = millis();      //현재 시간 체크

    long pressDuration = releasedTime - pressedTime;   
    //누른 시간에서 뗀 시간 차이 계산, 총 눌린 시간 반영


//=========총 눌린 시간이 설정해놓은 최대 단기 입력 인지 시간보다 작을 때==========//
    if( pressDuration < SHORT_PRESS_TIME )
    {
      Serial.println("A short press is detected(LOOP)");
      //단기 입력 감지 되었다는 문자열 출력
      loop_count++;
      save(loop_count);
    }
  }
}

void save(int i){
  //int read_loop = digitalRead(button_loop_Pin);
  switch(i){
    case 1 : study_time = timeLimit; timeLimit=0;Serial.println((String)"공부시간 :" + study_time);
              break;
    case 2 : break_time = timeLimit; timeLimit=0;Serial.println((String)"휴식시간 : " + break_time);
              break;
    case 3 : number = (timeLimit / 60000);Serial.println((String)"number : " + number);
              break;
    case 4 : working(); loop_count=0;
              break;
    }
  }

void working(){
  for(int a=0;1;a++){
    if( a != number){
      neo_1();
      theaterChase(strip.Color(127,   0,   0), 50); // Red
      delay(1000);
      neo_2();
      theaterChase(strip.Color(127,   0,   0), 50); // Red
      delay(1000);
      strip.clear();
    }
    else{
      break;
    }
  }
  }

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  for(j=0; j<256*2; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0); //turn every third pixel off
      }
    }
  }
}

void SSound(){
  for(int i=0;i<3;i++){
    tone(speakerpin,523,300);
    delay(500);
  }
}
