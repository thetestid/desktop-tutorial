/*
 * 로터리를 통한 시간 설정 후 반드시 버튼을 한번 눌러야 시간 설정 및 다음 기능으로 이동 가능
 */

#include <Encoder.h>
#include <TM1637Display.h>      //시계
#include <Adafruit_NeoPixel.h>  //네오픽셀
#include "Button2.h"            //button

//////////////////////////////////////////////////
////////핀 번호 선택
//로터리
#define ROTARY_ENCODER_A_PIN 32
#define ROTARY_ENCODER_B_PIN 21
#define ROTARY_ENCODER_BUTTON_PIN 22

//FND
#define CLK 2// FND CLK 입력
#define DIO 3// FND DIO 입력

//NEOPIXEL
#define PIN       21 // 네오픽셀 핀 21 설정
#define NUMPIXELS 24 // 네오픽셀 갯수
#define BRIGHT 10 // 네오픽셀 밝기
#define BRIGHTNESS 10



//////////////////////////////////////////////////////
/////////////FND 출력용 시간값 연산
#define numberOfHours(_time_)   ( ( (_time_ / 1000) / 60) / 60  ) // 시간
#define numberOfMinutes(_time_) (   (_time_ / 1000) / 60        ) // 분
#define numberOfSeconds(_time_) (   (_time_ / 1000) % 60        ) // 초

/////////////////////////////////////////////////////


const uint8_t DONE[] = //7세그먼트 시간 끝나면 done이라고 뜸
{
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_C | SEG_E | SEG_G,                           // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
};


/*
 * //////////////////수정 필요 LOOP로 수정 필요////////////////////
 */
const uint8_t LOOP[] = //7세그먼트 시간 끝나면 done이라고 뜸
{
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_C | SEG_E | SEG_G,                           // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
};

//////////////////////////////////////////////////////////////////////////NEOPIXEL
Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800); // 네오픽셀 초기화
uint32_t color; //네오픽셀 색상값 저장

//////////////////////////////////////////////////////////////////////////FND
TM1637Display display(CLK, DIO); // TM1637 FND

//////////////////////////////////////////////////////////////////////////BUTTON
Button2 button = Button2(ROTARY_ENCODER_BUTTON_PIN); //BUTTON
//Button2(byte attachTo, byte buttonMode = INPUT_PULLUP, boolean isCapacitive = false, boolean activeLow = true)

////////////////////////////////////////////////////////////////////////////
Encoder myEnc(32,26);          //ROTARY ENCODER
int position1 = 0;
int position2 = 0;
int oldPosition = 0;

int check_value = 0;        // 로터리 회전 방향을 양수, 음수로 알려줌
long timeLimit_global = 0;  // 타이머 설정 시간
int btn_flag  = 0;          // 버튼이 눌린 횟수 체크하는 플래그

long break_time = 0; //휴식시간
int loop_number = 0; //LOOP 기능 공부 횟수 지정

int color_position = 0;//색상 선택 기능을 위한 색상값 저장 변수
int temp_rotari_val = 0;//색상 선택 관련 로터리 값 임시 저장 변수

//========================== DEFAULT SETUP ==================================
void setup()
{
  Serial.begin(115200);
  
///////////////////////////////////////////////

  //BUTTON handler, you can ignore these
  button.setClickHandler(handler);
  //////////////////////////////////////
  ///////두번, 세번, 길게 누르기 용
  
  //button.setLongClickHandler(handler);
  button.setDoubleClickHandler(handler);
  //button.setTripleClickHandler(handler);

  /*
   * 두번 입력 핸들러와 별도 플래그 활용하여
   * 색상 변경 모드 진입하는 것으로 활용 예정
   * 
   * 싱글 버튼 입력 만 받게 될 경우 색상 변경 모드 진입 시에는
   * 텀을 두고 두번 눌러야 함
   * 이는 빠르게 두번 따닥 누른 것에 비해 직관적이지 않다고 판단됨
   */

/////////////////////////////////////////////////

  
  
}
//============================DEFAULT SETUP END====================================
//=========================== DEFAULT LOOP START=====================================
void loop()
{
  //for button, you must have this code if you want to use button
  //Button2 라이브러리에서 버튼 입력 감지를 위해 반드시 필요함
  button.loop();
  
  
  //================================모든 타이머 기능 설정 영역 START==============================//

/////////////////////////////////////

  switch (btn_flag){
    
///////////////////////////////////////////////////////////////////////////////////
//////////NORMAL TIMER DEFINITION START
    case 0: //버튼이 단 한번도 눌리지 않았을 때
      while(1)
      {
        Serial.println("기본 타이머 시작");
        timeLimit_global = rotateTime_rotari();
        button.loop(); //버튼 입력 체크를 위함
        
        if(btn_flag >= 1)//버튼이 눌리면
          break;
      }

      if(timeLimit_global == 0)
          break; // LOOP 기능으로 가기 위한 분기문 
           /*
           * 시간 설정 안하고 버튼 1번 눌렀을 때 loop기능으로 빠져나가기 위함
           * 현재 상태는 timeLimit_global이 0이고 (타이머 시간을 설정 안했고) 
           * 버튼을 1번 누른 상태이므로 LOOP 기능을 선택했다고 인지하여
           */
           

       //회전, 타이머 함수 부분
       Serial.println("기본 타이머 기능 작동 시작 3, 2, 1, 안되잖아?");
       neo_1(timeLimit_global);

       ///////////////////////////////////////////
       //neo_1내부 사운드 수정 필요
       
  
       /////////////////////////////////
       ////// 기본 타이머 기능 끝냈으므로 0로 초기화
       btn_flag = 0 ; 
       timeLimit_global = 0; 
       ////////////////////////////////
       break;
        
      


/////////////////////////////////////////////////////////////////////////////////////
//////////  LOOP DEFINITION START
    case 1: //버튼이 한번 눌렸을 때, LOOP 기능
      Serial.println("루프 기능 설정 시작");
      display.setSegments(LOOP); //loop 표출
            delay(1000);
            
      ///////////////////////////////////////////      
      while(1)//공부 시간 설정
      {   
        Serial.println("공부 시간 설정 시작 3, 2, 1");
        btn_flag = 1;
        timeLimit_global = rotateTime_rotari();
        Serial.println((String)"공부시간 :" + timeLimit_global);
        button.loop(); //버튼 입력 체크를 위함
        /*
         * btn_flag >= n 가 조건인 이유는
         * == n 시 오류에 의해 조건 충족 못하면 무한 반복에 갇히게 됨을
         * 방지하기 위함
         * 
         * 또한 n이 오류에 인하여 과도하게 숫자가 증가하여 다음기능을 시작하지 못할 경우를 대비하여 
         * 각 기능 시작 전에 n을 특정 숫자로 지정함
         * 
         * 본 설명은  LOOP기능 설정 과정에 적용됨.
         */
        if(btn_flag >= 2)
          break; 
      }
      ////////////////////////////////////////////
      while(1)//휴식시간 설정
      {   
        Serial.println("휴식 시간 설정 시작 3, 2, 1");
        btn_flag = 2;
        break_time = rotateTime_rotari();
        Serial.println((String)"휴식시간 :" + break_time);
        button.loop(); //버튼 입력 체크를 위함
        if(btn_flag >= 3)
          break; 
      }
      ////////////////////////////////////////////
      while(1)//반복횟수 설정
      {   
        Serial.println("반복 횟수 설정 시작 3, 2, 1");
        btn_flag = 3;
        loop_number = rotateTime_rotari();
        loop_number = loop_number / 60000;
        Serial.println((String)"반복횟수 :" + loop_number);
        button.loop(); //버튼 입력 체크를 위함
        
        if(btn_flag >= 4)
          break; 
      }

      //LOOP 기능 구현 함수 예정 부분

      for(int a = 1 ; loop_number ; a++){
        if( a != loop_number){
          neo_1(timeLimit_global);
          //theaterChase(strip.Color(127,   0,   0), 50); // Red
          delay(1000);
          neo_1(break_time);
          //theaterChase(strip.Color(127,   0,   0), 50); // Red
          delay(1000);
          strip.clear();
        }
        else
          break;
      }

      Serial.println("루프 타이머 기능 작동 시작 3, 2, 1, 안되잖아?");

      /////////////////////////////////////////
      //neo_1 내부 sound 기능 수정 필요


      /////////////////////////////////
      ////// LOOP 기능 끝냈으므로 0로 초기화
      btn_flag = 0 ; 
      timeLimit_global = 0; 
      ////////////////////////////////

      break;

  //////////////////////////////////////////////////////////////////////////////
  ///////////  COLOUR SETTING DEFINITION START
    case 2: //버튼이 2번 눌렸을 때 색깔 설정 기능

      Serial.println("LED색상 설정 시작");
      
      while(1)
      {
        //현재 버튼 누른 횟수(= 현재 선택된 모드) 고정, 이후 비교하여 while을 탈출하기 위함
        btn_flag = 2; 

        //기존 함수 최대한 활용 및 조작 방법 통일을 위해 check_rotari()와 별도 변수 이용
        temp_rotari_val = check_rotery();//로터리 회전 방향 및 증감값 받아오기

        /*
         * -1을 곱하는 이유는 시계방향 회전이 증가, 반시계 방향이 감소로 동작하게 만들기 위함
         * 시간 설정과 색상 설정 방식을 통일시켜 사용자에게 혼선을 주지 않게 하기 위함
         * 통상 시계방향 회전이 값 증가 인 것이 자연스러움
         */
        color_position = (-1 * temp_rotari_val) + color_position;



        /////////////////////////////////////////////////////////////////
        //색상변경 시의 오버플로, 언더플로 방지 부분
        if(color_position > 1337)  //  (65525/49)=1337.221...
          color_position = 0;
        else if(color_position < 0)
          color_position = 1337;
        else
          ;
        /////////////////////////////////////////////////////////////
        //색상 변경 시의 오버플로 언더플로 방지 부분 종료

        //범위는 0~65525, 값에 따라 빨간색 ~ 보라색 변환
        color = strip.ColorHSV( (color_position * 49), 255, 5);
        strip.fill(color, 0, 24);
        strip.show();

        button.loop(); //버튼 입력 체크를 위함

        if(btn_flag >= 3)
          break;
        
      }

      
      // 색상 변경이 끝났으므로 플래그 원래대로, 색깔 값은 초기화 시키지 않음
      // 연속사용 시 색상 유지를 위해서임
      btn_flag = 0;
        
      
    break;

  }
  
  //================================모든 기능 설정 영역 END==============================//

}
//==================================기본 LOOP 종료 ======================================//



//////////////////////////////////////////////////////////////////////////////////////////////////////
//========================전,현 로터리 값 비교 함수, 로터리 회전 방향 파악에 사용==========================//
/*
 * 이용한 로터리는 반시계방향으로 회전 시 값이 증가, 시계 방향은 감소
 * 기존 값, 현재 변한 값의 차를 이용하여 회전 방향을 알 수 있음
 */
int check_rotery()
{
  long newPosition = myEnc.read();
  if(newPosition != oldPosition)
  {
    int value = newPosition - oldPosition;
    oldPosition = newPosition;
    return value;
  }
  else
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////
//=================================로터리 회전 과 시간 연동 함수====================================//

int rotateTime_rotari()
{
  //본 함수 내부 타이머 설정 시간 연산용 변수
  long timeLimit = 0;
  //현재 버튼 누른횟수 저장, 이후 버튼 누른횟수 비교를 통해 밑의 while을 빠져 나가기 위함
  int btn_flag_rtrFunc = btn_flag;

  while(1)
  {
    //유이한 check_rotary() 동작하는 곳
    check_value = check_rotery();
    
  
    //로터리 엔코더는 반시계방향 회전 시 값이 증가함
    if(check_value < 0) //시계방향 회전 시 시간 증가 연산
    {
      if(timeLimit > 35994000){//시간 설정 최대값인 99시간 59분을 넘으면
        //Prevent overflow 오버플로 시
        timeLimit = 0;//시간 설정을 0으로
        Serial.println((String)"overflow detected");       
      }
      else{
        timeLimit = timeLimit + 60000; //1000 * 60 = 1 minute, 실질 시간 연산 부분
        //시간 출력 부분
        Serial.println((String) "설정시간 : " + timeLimit );
        display.showNumberDecEx(numberOfMinutes(timeLimit)-(numberOfHours(timeLimit)*60),0,true,2,2);
        display.showNumberDecEx(numberOfHours(timeLimit),(0x80 >> 1),true,2,0);
      }
    }

    //////////////////////////////////////////////////////////////////////////
    
    else if(check_value > 0) //반시계 방향 회전 시 시간 감소 연산
    {
      if(timeLimit < 0){
        //Prevent underflow 언더플로 시
        timeLimit = 35994000;//시간 설정 최댓값인 99시간 59분으로
        Serial.println((String)"underflow detected");        
      }
      else{
        timeLimit = timeLimit - 60000; //1000 * 60 = 1 minute, 실질 시간 연산 부분
        //시간 출력 부분
        Serial.println((String) "설정시간 : " + timeLimit );
        display.showNumberDecEx(numberOfMinutes(timeLimit)-(numberOfHours(timeLimit)*60),0,true,2,2);
        display.showNumberDecEx(numberOfHours(timeLimit),(0x80 >> 1),true,2,0);
      }
    }
    else //로터리가 회전 안하면 아무것도 안함
    ;

    button.loop(); //버튼 입력 감지를 위함
    
    if(btn_flag_rtrFunc != btn_flag)//타이머 시간 설정 후 한번이라도 버튼 눌렀으면
    {
      //연산한 시간 리턴
      return timeLimit;
      break;//무한 반복 while 문 탈출
    }
  
  }
  //여기까지 while문, 버튼 누르면 탈출해서 최종 시간 
  
}

///////////////////////////////////////////////////////////////////////////////////
//=================================네오픽셀 LED 점멸 기타 효과 동작=====================================

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


/////////////////////////////////////////////////////////////////////////////
//============================타이머 동작 중 FND, LED 동작 정의===============================

void neo_1(long neo1_time)
{
   for (long i = neo1_time; i > 0; i=i-1000)   // data type을 long 으로
   {    
      int hours   = numberOfHours(i);
      int minutes = numberOfMinutes(i);
      if (minutes >=60) // 80분으로 나오는거 방지
        minutes =60 - minutes;
  
      int seconds = numberOfSeconds(i);
     
  
      
      Serial.println((String) "남은시간 : " + i);                             // 시리얼 모니터에서 시간을 볼수 있슴
      
      display.showNumberDecEx(minutes, 0, true, 2, 2);                 // 분
      display.showNumberDecEx(hours, (0x80 >> 1), true, 2, 0) ;        // 시간
                                                                       // 콜론 켜기    원본은  0x80 >> 3 인데 콜론이 on 되지 않아 0x80 >> 1 로 변경
  
      int leftTime_LED =(i / (neo1_time / 24)  );                 // 남은 시간동안 led 켜기
  
      //uint32_t c = strip.Color(255, 0 , 0);           // 네오픽셀 빨간색, 색깔짙게, 밝기3
      strip.fill(color, 0, leftTime_LED);               // color를 0번 led ~ leftTime_LED번 led까지 채우세용
      //ColorHSV(0, 255 , 10);                                                                 // 남은 시간 동안 led 켜기
      
      Serial.println( (String) "남은 led : " + leftTime_LED);        // PC화면 출력                                          
  
      
      strip.show();                                                 //네오픽셀 보여주기
      strip.clear();                                                //네오픽셀 새로고침 
  
      
      delay(500);
    }


    
  
      
   display.setSegments(DONE); //끝났다고 뜨기
   //SSound();  ESP32 버전으로 수정 필수
   delay(2000);
   
}


////////////////////////////////////////////////////////////////////////
//======================버튼 핸들러 함수, 각 버튼 입력에 대한 동작 정의==============================//
void handler(Button2& btn) {
    switch (btn.getClickType()) {
        case SINGLE_CLICK:
          btn_flag = btn_flag + 1;
          Serial.println((String)"현재 플래그는 " + btn_flag);
          delay(1000);//delay는 단순 테스트용, 나중에 꼭 지워주기
            break;
        case DOUBLE_CLICK:
            Serial.print("double ");
            
            if(btn_flag == 0) //아무 기능도 선택하지 않았거나 아무 기능도 하고 있지 않거나
              btn_flag = 2;   //초기 기능 선택에서 색상 변경 기능으로 진입하기 위함
              
            Serial.println((String)"현재 플래그는 " + btn_flag);
            delay(1000);//delay는 단순 테스트용, 나중에 꼭 지워주기
            break;
        case TRIPLE_CLICK:
            Serial.print("triple ");
            delay(1000);//delay는 단순 테스트용, 나중에 꼭 지워주기
            break;
        case LONG_CLICK:
            Serial.print("long");
            delay(1000);//delay는 단순 테스트용, 나중에 꼭 지워주기
            break;
    }
}

/*
 * 버전 로그
 * V1.0 check rotari, rotatetime_rotari, handler, 기능 구현 case 0 , case 1 분기문 완성, 테스트 완료
 * V1.1 네오픽셀 객체 선언 및 핀번호 정의, 색상변경 코드 추가 및 해당 기능으로 진입 방식 개선, 
 *      일부 주석 수정 및 추가, 테스트 완료
 * V1.2 neo_1 내부 SSound();  ESP32 버전으로 수정 필수, 타이머 기능 및 동작 함수 추가, 타이머 기능 완성
 * 테스트 미완료 <08.31 시점>
 * V1.3
 * V1.4
 */