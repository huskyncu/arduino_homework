#include <LBLE.h>
#include <LBLEPeriphral.h>
#include <DHT.h>

int item, temperature, humidity, data_choice=0,h,t;
DHT __dht2(2, DHT22);

LBLEService AService("f08dcf5a-dadd-418c-84c9-1f15680be364");
LBLECharacteristicInt ARead("f08dcf5a-dadd-418c-84c9-1f15680be364", LBLE_READ | LBLE_WRITE);

void setup()
{
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  
  Serial.begin(9600);
  __dht2.begin();
  LBLE.begin();
  while (!LBLE.ready()) { delay(100); }
  Serial.println("BLE ready");

  Serial.print("Device Address = [");
  Serial.print(LBLE.getDeviceAddress());
  Serial.println("]");

  AService.addAttribute(ARead);

  LBLEPeripheral.addService(AService);
  LBLEPeripheral.begin();
  LBLEAdvertisementData advertisement;
  advertisement.configAsConnectableDevice("BLE Ana");
  LBLEPeripheral.advertise(advertisement);


}


void loop()
{
  //Serial.println(ARead.getValue());
  //item = digitalRead(2);
  temperature = __dht2.readTemperature();
  humidity = __dht2.readHumidity();
  if(temperature >= 22 && temperature <=38 ){
      digitalWrite(15,LOW);
      digitalWrite(16,HIGH);
      digitalWrite(17,HIGH);
  }
  if(temperature >= 10 && temperature <22 ){
      digitalWrite(15,HIGH);
      digitalWrite(16,LOW);
      digitalWrite(17,HIGH);
  }
  if(temperature >= 0 && temperature <10 ){
      digitalWrite(15,HIGH);
      digitalWrite(16,HIGH);
      digitalWrite(17,LOW);
  }
  if(data_choice ==1)
  {
    h = humidity+100;
    data_choice = 0;
  }
  else
  {
    h = temperature;
    data_choice = 1;
  }
  //Serial.println(item);
  Serial.print("當前濕度:");
  Serial.print(humidity);
  Serial.println("%");
  Serial.print("當前溫度:");
  Serial.print(temperature);
  Serial.println("度");
  ARead.setValue(h);
  delay(1000);

}
