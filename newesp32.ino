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

  if(!SD.begin())
  {
        Serial.println("Card Mount Failed");
        return;
  }

  
  SerialBT.begin("ESP32test");
  Serial.println("The device started, now you can pair it with bluetooth!");

  
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
  
  
    //writeFile(SD, "/hello.txt", "test1"); //처음 파일 만들어줌
    appendFile(SD, "/hello.txt", "공부한 시간 : timelimit1test - ");
    appendFile(SD, "/hello.txt", asctime(&timeinfo));
    
    readFile(SD, "/hello.txt"); 


  //SerialBT.end();  //블루투스 끌수 있음

}

void loop() 
{ 
 //readFile(SD, "/hello.txt");
 /* 
 if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  */
    
}
