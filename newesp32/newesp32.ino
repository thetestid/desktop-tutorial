#include <ESP32Time.h>
#include <WiFi.h>

#include <FS.h>
#include <SD.h>
#include <SPI.h>

#include <BluetoothSerial.h>


ESP32Time rtc;
BluetoothSerial SerialBT;


//wifi 세팅
const char* ssid       = "abc";
const char* password   = "";

const char* ntpServer = "kr.pool.ntp.org";
const long  gmtOffset_sec = 3600*8;
const int   daylightOffset_sec = 3600;

char TTT = 'T';


void sdcardblesend(char k)
{
   if(k == 'T')        // T를 보내면
    readFile(SD, "/hello.txt"); //앱에다 보냄
}

void firebasesend(long neo1_time)
{
    SerialBT.write((int)neo1_time);
   
}



//sd카드
void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file)
    {
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available())
    {
        //Serial.write(file.read());
        SerialBT.write(file.read());   
    }
    file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file)
    {
        Serial.print("Failed to open file for writing");
        return;
    }
    if(file.println(message))
    {
        Serial.println("File written");
    } 
    else 
    {
        Serial.println("Write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message))
    {
        Serial.println("Message appended");
    } 
    else 
    {
        Serial.println("Append failed");
    }
    file.close();
}


void setup() 
{
Serial.begin(115200);

  
SerialBT.begin("ESP32test");


while(!SerialBT.begin())
        Serial.println("블루투스 확인해주세요");   
Serial.println("esp32 bluetooth start!")

while(!SD.begin())
    Serial.println("sd 카드 확인해주세요");
Serial.println("sd카드가 연결 되었어요!");   
   


  
  //wifi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  
   
   
   
  
  //---------set with NTP---------------
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  struct tm timeinfo;
  if (getLocalTime(&timeinfo))
  {
    rtc.setTimeStruct(timeinfo); 
  }
  
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  
  
    writeFile(SD, "/hello.txt", "test1"); //처음 파일 만들어줌
    appendFile(SD, "/hello.txt", "공부한 시간 : timelimit1test - ");
    appendFile(SD, "/hello.txt", asctime(&timeinfo));
    

  //SerialBT.end();  //블루투스 끌수 있음

}

void loop() 
{ 
 
 /*
 if (Serial.available()) 
 {
    SerialBT.write(Serial.read());
 }
 
  if (SerialBT.available()) 
  {
    Serial.write(SerialBT.read());
  }
  */
  
if(TTT == SerialBT.read())
    blesend(SerialBT.read());
    
    
    /*
    kkk = SerialBT.read();
    if(kkk == 'T')
      readFile(SD, "/hello.txt");
    */
    
  
  
}  
