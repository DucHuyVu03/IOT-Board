#ifndef _WiFi_h
#define _WiFi_h


#include "IoTVision.h"  // Có define debug để bật/tắt các debug ra Serial.c
#include <Arduino.h>    // Để khai báo kiểu String


//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//===================== Begin: classWiFi ==================================//
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
#pragma region classWiFi
class WIFI {
public:
  String TenWiFi = "dy";          // Thay tên WIFI ở đây.
  String MatKhauWiFi = "yetsir0000"; // Thay mật khẩu WIFI ở đây.
  // String TenWiFi = "Chủ Tọa";          // Thay tên WIFI ở đây.
  // String MatKhauWiFi = "080500011200"; // Thay mật khẩu WIFI ở đây.



public:
  String LaySoMAC(void);
  void KetNoiWiFi(int ThoiGianChoKetNoi); // Kết nối WiFi.
  int TinhDoManhCuaWiFi(void);
};
#pragma endregion classWiFi
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM
//===================== End: classWiFi ====================================//
//MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM


#endif
