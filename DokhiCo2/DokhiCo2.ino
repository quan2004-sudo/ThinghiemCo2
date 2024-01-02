#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#define Co2in A2
#define Co2out A3
#define Relaypin 7
#define Buzz 8
SoftwareSerial mySerial(3, 2);
LiquidCrystal_I2C lcd(0x27, 16, 2);
int Nguong = 200, Nguongcu, Co2In_Value, Co2Out_Value;
String canhbao = "0", State = "0", FanValue = "0";
unsigned long lastTime = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(Buzz, OUTPUT);
  pinMode(Relaypin, OUTPUT);

  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  dataReceived();
  Co2In_Value = round(trungbinhvao());
  Co2Out_Value = round(trungbinhra());
  display(Co2In_Value, Co2Out_Value);
  if (FanValue == "1") {
    digitalWrite(Relaypin, HIGH);
  } else {
    digitalWrite(Relaypin, LOW);
  }
  if (Co2Out_Value > Nguong && canhbao == "1") {
    digitalWrite(Buzz, HIGH);
    State = "1";
  } else {
    digitalWrite(Buzz, LOW);
    State = "0";
  }

  if (millis() - lastTime > 3000) {
    dataSend();
    lastTime = millis();
  }
}

void display(int Co2InValue, int Co2OutValue) {

  lcd.setCursor(0, 0);
  lcd.print("Co2 in: ");
  lcd.print(Co2InValue);
  lcd.setCursor(0, 1);
  lcd.print("Co2 out: ");
  lcd.print(Co2OutValue);
  delay(500);
  lcd.clear();
}


void dataReceived() {
  if (mySerial.available()) {                              // Kiểm tra xem có dữ liệu đang được gửi đến không
    String receivedData = mySerial.readStringUntil('\n');  // Đọc chuỗi dữ liệu cho đến khi gặp ký tự '\n'
    String data[3];
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

    // Xử lý dữ liệu nhận được
    for (int i = 0; i < 3; i++) {
      Serial.print("Data " + String(i + 1) + ": ");
      Serial.print(data[i]);
      Serial.print("\t");
    }
    Serial.println();
    FanValue = data[0];
    Nguong = data[1].toInt();
    canhbao = data[2];
    if (Nguong != Nguongcu) {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Nguong : ");
      lcd.print(Nguong);
      delay(5000);
    }
    Nguongcu = Nguong;
  }
}

void dataSend() {
  String data1[3] = { String(Co2In_Value), String(Co2Out_Value), String(State) };



  String combinedData1 = "";  // Khởi tạo chuỗi kết quả

  combinedData1 = data1[0] + "," + data1[1] + "," + data1[2] + "\n";
  // Kết hợp các dữ liệu vào chuỗi

  // Gửi chuỗi dữ liệu qua UART
  mySerial.print(combinedData1);
  // Serial.print(combinedData1);
}

float trungbinhvao() {
  int Co2In = analogRead(Co2in);
  float tong;
  for (int i = 0; i < 400; i++) {
    tong += Co2In;
  }
  return tong/400;
}
float trungbinhra() {
 int Co2Out = analogRead(Co2out);
  float tong;
  for (int i = 0; i < 400; i++) {
    tong += Co2Out;
  }
  return tong/400;
}
