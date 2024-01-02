/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL6MB7gPyEC"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "L4-Y7i0NuBM6HsgZWzmP2xVVMJ6vx1S-"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial
#define EEPROM_SIZE 12

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiManager.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(D1, D2);
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "AmericanStudy T1";
char pass[] = "66668888";
unsigned long lasttimedata = 0;
int FanValue, Nguong, canhbao;
int addr = 0;

BlynkTimer timer;

String data[10];
WiFiManager wm;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V0) {
  // Set incoming value from pin V0 to a variable
  FanValue = param.asInt();
}
BLYNK_WRITE(V1) {
  // Set incoming value from pin V0 to a variable
  Nguong = param.asInt();
}
BLYNK_WRITE(V2) {
  // Set incoming value from pin V0 to a variable
  canhbao = param.asInt();
}



// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED() {
  Blynk.virtualWrite(V1, "Xin chào!!!");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent() {
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V1, "Time Up: " + String(millis() / 1000));
}

void setup() {
  // Debug console
  Serial.begin(115200);
  mySerial.begin(9600);
  bool res;
  res = wm.autoConnect("May Do Khi Co2", "88888888");  // password protected ap
  if (!res) {
    Serial.println("Failed to connect");
    // ESP.restart();
  } else {
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
  }

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(3000L, myTimerEvent);
}

void loop() {
  Blynk.run();
  timer.run();

  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!

  // chỗ này update dữ liệu
  if (millis() - lasttimedata > 2000) {
    lasttimedata = millis();
    sendUart();
  }
  nhandulieu();
}


void sendUart() {
  // float data1[3] = { mode_1, state1_1, state2_2 };  // Dữ liệu của 3
  String data1[3] = { String(FanValue), String(Nguong), String(canhbao) };



  String combinedData1 = "";  // Khởi tạo chuỗi kết quả

  combinedData1 = data1[0] + "," + data1[1] + "," + data1[2] + "\n";
  // Kết hợp các dữ liệu vào chuỗi

  // Gửi chuỗi dữ liệu qua UART
  mySerial.print(combinedData1);
  Serial.print(combinedData1);
}


void nhandulieu() {
  if (mySerial.available()) {                              // Kiểm tra xem có dữ liệu đang được gửi đến không
    String receivedData = mySerial.readStringUntil('\n');  // Đọc chuỗi dữ liệu cho đến khi gặp ký tự '\n'

    // Tách dữ liệu bằng dấu phẩy

    int startIndex = 0;
    int commaIndex;

    for (int i = 0; i < 3; i++) {
      commaIndex = receivedData.indexOf(',', startIndex);
      if (commaIndex != -1) {
        data[i] = receivedData.substring(startIndex, commaIndex);
        startIndex = commaIndex + 1;
      } else {
        data[i] = receivedData.substring(startIndex);
        break;
      }
    }
    // for (int i = 0; i < 10; i++) {
    //   Serial.print("Data " + String(i + 1) + ": ");
    //   Serial.println(data[i]);
    // }
    String Co2In = data[0];
    String Co2Out = data[1];
    String Alert = data[2];
    Blynk.virtualWrite(V4, "Co2In:" + Co2In + " | Co2Out:" + Co2Out);
    if (Alert == "1") {
      Blynk.virtualWrite(V5, "Cảnh báo: Quá nhiều Co2 ở đầu ra");
    }  else {
      Blynk.virtualWrite(V5, "Cảnh báo: Quá trình trung hòa đang diễn ra");
    }
  }
}