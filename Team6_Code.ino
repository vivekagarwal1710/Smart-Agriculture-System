#include <ThingSpeak.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <dht.h>
#include <Adafruit_BMP085.h>
#include <SoftwareSerial.h>
#include <String.h>
dht DHT;
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial gprsSerial(10,11);
#define DHT11_PIN A3
int chk;
float humi = 0;
float temp = 0;
int soil = 0;
int light = 0;
int gas = 0;
boolean HT;

void setup()

{ gprsSerial.begin(9600);
  Serial.begin(9600);
  lcd.begin(16,2);
  gprsSerial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Please wait for");
  lcd.setCursor(0, 1);
  lcd.print("60 seconds.");
  delay(20000);
  delay(20000);
  delay(20000);
  lcd.clear();
  
}

void loop()
{
  chk = DHT.read11(DHT11_PIN);
  temp = DHT.temperature;
  humi = DHT.humidity;
  soil = analogRead(A0);
  light = analogRead(A1);
  gas = analogRead(A2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Soil:");
  soil = map(soil, 0, 1023, 100, 0);
  lcd.print(soil);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print("Light:");
  light = map(light, 0, 1023, 0, 100);
  lcd.print(light);
  lcd.print("%");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  switch (chk)
  {
    case DHTLIB_OK:
      HT = true;
      break;
    default:
      HT = false;
      break;
  }
  if (HT == true)
  {
    lcd.print("Temp:");
    lcd.print(temp);
    lcd.print(" *C");
    lcd.setCursor(0, 1);
    lcd.print("Humidity:");
    lcd.print(humi);
    lcd.print("%");
  }
  else
  {
    temp = 0;
    humi = 0;
    lcd.print("Temp:");
    lcd.print("No Data");
    lcd.setCursor(0, 1);
    lcd.print("Humidity:");
    lcd.print("No Data");
  }
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Air Qlt: ");
  gas = map(gas, 0, 1023, 0, 100);
  lcd.print(gas);
  lcd.print("%");
  lcd.setCursor(0, 1);
  delay(3000);
  Send_data();

} 
  


void Send_data()
{ 
  Serial.println("Send Data");
  lcd.clear();
  lcd.print("Sending the data");
  lcd.setCursor(0, 1);
  lcd.print("to Thingspeak...");
  delay(1500);
  
  if (gprsSerial.available())
    Serial.write(gprsSerial.read());
 
  gprsSerial.println("AT");
  delay(1000);
 
  gprsSerial.println("AT+CPIN?");
  delay(1000);
 
  gprsSerial.println("AT+CREG?");
  delay(1000);
 
  gprsSerial.println("AT+CGATT?");
  delay(1000);
 
  gprsSerial.println("AT+CIPSHUT");
  delay(1000);
 
  gprsSerial.println("AT+CIPSTATUS");
  delay(2000);
 
  gprsSerial.println("AT+CIPMUX=0");
  delay(2000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CSTT=\"airtelgprs.com\"");//start task and setting the APN,
  delay(1000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIICR");//bring up wireless connection
  delay(3000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIFSR");//get local IP adress
  delay(2000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSPRT=0");
  delay(3000);
 
  ShowSerialData();
  
  gprsSerial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection
  delay(6000);
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSEND");//begin send data to remote server
  delay(4000);
  ShowSerialData();
  
  String str="GET https://api.thingspeak.com/update?api_key=ULDRU2ZMYGNI71D1&field1=" + String(soil) +"&field2="+String(light)+"&field3="+String(gas)+"&field4="+String(temp)+"&field5="+String(humi);
  Serial.println(str);
  gprsSerial.println(str);//begin send data to remote server
  
  delay(4000);
  ShowSerialData();
 
  gprsSerial.println((char)26);//sending
  delay(5000);//waitting for reply, important! the time is base on the condition of internet 
  gprsSerial.println();
 
  ShowSerialData();
 
  gprsSerial.println("AT+CIPSHUT");//close the connection
  delay(100);
  ShowSerialData();
} 
void ShowSerialData()
{
  while(gprsSerial.available()!=0)
  Serial.write(gprsSerial.read());
  delay(5000); 
  
}
