#include "MySHT31.h"
#include "RTC.h"
#include "Relay.h"
#include "myWIFI.h"
#include <Wire.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define SCL 18
#define SDA 19
//int i=0;
//bool status;

SHT3x _SHT31; 
WIFI _WiFi; 
RTC _RTC; 
String _ID;

Relay _Relay; 
int _LenhONOFFK1; 
int _LenhONOFFK2; 
int relayMode;
int RSSI;

#define API_KEY "Put your own API key here"
#define DATABASE_URL "Your own database url" 

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
unsigned long reconnectWiFiPrevMillis = 0;
unsigned long reconnectFirebasePrevMillis = 0;
const unsigned long reconnectWiFiInterval = 2000; // Set thời gian tái kết nối Wifi.
const unsigned long reconnectFirebaseInterval = 2000; // Set thời gian tái kết nối Database.
float temperature,humidity;
float temp,humid;
bool signupOK = false;
bool ONOFF_K1 = false;
bool ONOFF_K2 = false;

//--- Setup ---//
void setup() {

  // Gọi các hàm của đối tượng Relay
  _Relay.KhoiTaoCacChan();
  _Relay.OFFCacRole();
  _Relay.MODE = _AUTO;
  //_Relay.MODE = _MANUAL;

  //=============== CÀI ĐẶT KẾT NỐI =================//
  Serial.begin(115200); // Khởi động cổng Serial.
  Wire.begin(SDA,SCL); // Khởi động giao thức I2C.
  _WiFi.KetNoiWiFi(10); // Khởi động kết nối Wifi.
  config.api_key = API_KEY; // Lấy API của Database.
  config.database_url = DATABASE_URL; // Lấy URL của Database.

  // Kết nối với Databse Firebase.
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback; // Tạo addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

}// Điều Khiển Relay
void relayOperation (void)
{
  // Quy định trạng thái ban đầu của Relay.
    if (_Relay.MODE == _MANUAL) {
    // Điều khiển đóng ngắt Relay thủ công.
    ONOFF_K1 = _Relay.ONOFFBangTayK1(_LenhONOFFK1);
    ONOFF_K2 = _Relay.ONOFFBangTayK2(_LenhONOFFK2); 
    }
    else if (_Relay.MODE == _AUTO) {
    _SHT31.DocCamBienNhietDoVaDoAmSHT31();
    double MIN_NhietDo = 20; // Giá trị Nhiệt Độ tối thiểu.
    double MAX_NhietDo = 31; // Giá trị Nhiệt Độ tối đa.
    double MIN_DoAm = 78; // Giá trị Độ Ẩm tối thiểu.
    double MAX_DoAm = 81; // Giá trị Độ Ẩm tối đa.
    // Đóng ngắt Relay theo ngưỡng giá trị đã set.
    ONOFF_K1 = _Relay.TuDongDongNgatKenh1TheoNhietDo(_SHT31.NhietDo, MIN_NhietDo, MAX_NhietDo);
    ONOFF_K2 = _Relay.TuDongDongNgatKenh2TheoDoAm(_SHT31.DoAm, MIN_DoAm, MAX_DoAm);
    }
}

// Trao đổi dữ liệu với Database cho điểu khiển Relay
void readDataFromFirebaseForRELAY() {
  // Đọc giá trị CHẾ ĐỘ HOẠT ĐỘNG từ Database.
  if (Firebase.RTDB.getInt(&fbdo, "mode", &relayMode)) {
    Serial.println("mode data received successfully");
    Serial.println("Che Do: " + String(relayMode));
    if (relayMode == 0) {
      _Relay.MODE = _MANUAL;
    } else if (relayMode == 1) {
      _Relay.MODE = _AUTO;
    }
  }
  // Đọc giá trị hoạt động của RELAY 1 từ Database.
  if (Firebase.RTDB.getInt(&fbdo, "ONOFFK1", &_LenhONOFFK1)) {
    Serial.println("ONOFFK1 data received successfully");
    Serial.println("ONOFFK1: " + String(_LenhONOFFK1));
  } else {
    Serial.println("Failed to get ONOFFK1 data: " + fbdo.errorReason());
  }
  // Đọc giá trị hoạt động của RELAY 2 từ Database.
  if (Firebase.RTDB.getInt(&fbdo, "ONOFFK2", &_LenhONOFFK2)) {
    Serial.println("ONOFFK2 data received successfully");
    Serial.println("ONOFFK2: " + String(_LenhONOFFK2));
  } else {
    Serial.println("Failed to get ONOFFK2 data: " + fbdo.errorReason());
  }
}

void loop()
{
  // Hàm duy trì kết nối Wifi.
  if (WiFi.status() != WL_CONNECTED) {
    unsigned long currentMillis = millis();
    if (currentMillis - reconnectWiFiPrevMillis >= reconnectWiFiInterval) {
      reconnectWiFiPrevMillis = currentMillis;
      Serial.println("Reconnecting to Wi-Fi...");
      _WiFi.KetNoiWiFi(10);
    }
  }

  // Khởi tạo các giá trị để trao đổi với Database.
  _SHT31.DocCamBienNhietDoVaDoAmSHT31();
  _ID = _WiFi.LaySoMAC();
  RSSI = _WiFi.TinhDoManhCuaWiFi();
  _RTC.LayRTCTuServerCaiDatChoDS3231();
  String s3 = _RTC.ChuanHoaChuoiRTCDeGuiVeServer();

  // Thực hiện trao đổi dữ liệu với Database.
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 500 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    Firebase.RTDB.setFloat(&fbdo, "temp", _SHT31.NhietDo);
    Firebase.RTDB.setFloat(&fbdo, "humid", _SHT31.DoAm);
    Firebase.RTDB.setString(&fbdo, "MAC", _ID );
    Firebase.RTDB.setFloat(&fbdo, "RSSI", RSSI );
    Firebase.RTDB.setString(&fbdo, "RTC", s3 );
    readDataFromFirebaseForRELAY();
    relayOperation(); 
  }
  delay(500);

  // Thực hiện in ra cửa sổ Serial Monitor để theo dõi hoạt động của mạch.
  Serial.print("Nhiệt độ: ");
  Serial.println(_SHT31.NhietDo);
  Serial.print("Độ ẩm: ");
  Serial.println(_SHT31.DoAm);
  Serial.print("RSSI: ");
  Serial.println(RSSI);
  Serial.print("Thời gian thực trên board: ");
  Serial.println(s3);
  Serial.print("Chế độ hoạt động: ");
  Serial.println(_Relay.MODE);
}
