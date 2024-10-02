#include <WiFi.h>         // Thư viện WiFi của ESP32.
#include "myWIFI.h"      //
#include <WebServer.h>    //
#include <WiFiAP.h>       //

//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
String WIFI::LaySoMAC(void) {
#pragma region LaySoMAC
//===========================================================================
  //---------------------------------------------------------------
  // Lấy số MAC của ESP32, đây là số IMEI của board.
  //---------------------------------------------------------------
  String MAC;
  MAC = WiFi.macAddress();
  MAC.replace(":", "");


#ifdef debug
  Serial.println("");
  Serial.print("MAC: ");
  Serial.println(MAC);
#endif
  //---------------------------------------------------------------


  return MAC;  //===========================================================================
#pragma endregion LaySoMAC
}

//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
// Kết nối WiFi.
void WIFI::KetNoiWiFi(int ThoiGianChoKetNoi) {
#pragma region KetNoiWiFi
//===========================================================================
  // Xóa các cấu hình WiFi cũ đã lưu trước đó trong ESP.
  //ESP.eraseConfig();


  // Chuyển sang MODE STA (Stations) để dừng phát Access Point.
  WiFi.mode(WIFI_STA);
  delay(250);


  // Bắt đầu kết nối WiFi.
  WiFi.begin(this->TenWiFi, this->MatKhauWiFi);


#ifdef debug
  Serial.print("Đang kết nối WiFi... ");
#endif  


  // Kiểm tra kết nối WiFi
  //---------------------------------------------------------------
  uint16_t count = 0;
  while (count < ThoiGianChoKetNoi) {
    if (WiFi.status() == WL_CONNECTED) {


#ifdef debug
      Serial.println("Kết nối WiFi thành công!");
#endif      
      break;
    }
    delay(1000);
    count++;


#ifdef debug
    Serial.print(count);
    Serial.print(" > ");
#endif    
  }


#ifdef debug
  if (count >= ThoiGianChoKetNoi) {
    Serial.println("");
    Serial.println("Đã hết thời gian, kết nối WiFi thất bại!");
  }
#endif  
  //---------------------------------------------------------------
//===========================================================================
#pragma endregion KetNoiWiFi
}

//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
int WIFI::TinhDoManhCuaWiFi(void) {
#pragma region TinhDoManhCuaWiFi
//===========================================================================
  if (WiFi.status() != WL_CONNECTED)
    return -1;


  int dBm = WiFi.RSSI();


#ifdef debug
  Serial.print("WiFi's RSSI: ");
  Serial.println(dBm);
#endif


  if (dBm <= -100)
    return 0;
  if (dBm >= -50)
    return 100;
  return 2 * (dBm + 100);
  //===========================================================================
#pragma endregion TinhDoManhCuaWiFi
}
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM


