#ifndef _SHT31_h
#define _SHT31_h

#include "SHT31.h"

#define SHT31_ADDRESS 0x44
#define dataPin 19   //Chân SDA
#define clockPin 18  //Chân SCL
//-----------------------------------------------------------------



class SHT3x {
private:
  SHT31 sht31;


public:
  double NhietDo;
  double DoAm;


public:
  void KhoiTaoSHT31(void);
  void DocCamBienNhietDoVaDoAmSHT31(void);
};


#endif
