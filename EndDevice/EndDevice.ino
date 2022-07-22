#include<SoftwareSerial.h>
#include <LiquidCrystal.h>
int Contrast = 20;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

SoftwareSerial xbee(10,9);

String data;

void setup()
{
  analogWrite(6, Contrast);
  lcd.begin(16, 2);
  Serial.begin(115200);
  xbee.begin(9600);
  while(!xbee)
  {
    ;
  }
}

void loop()
{
  if(xbee.available())
  {
    data=xbee.readString();
    lcd.setCursor(0, 0);
  Serial.println(data);
  lcd.print(data);
   
  }
 
}
