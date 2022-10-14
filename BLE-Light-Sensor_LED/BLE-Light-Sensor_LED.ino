#include <LBLE.h>
#include <LBLEPeriphral.h>
#include <Wire.h>
#include <SeeedOLED.h>
#include <SPI.h>
#include <MFRC522.h>

String read_id;
MFRC522 rfid(/*SS_PIN*/ 10, /*RST_PIN*/ 9);

String mfrc522_readID()
{
  String ret;
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial())
  {
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

    for (byte i = 0; i < rfid.uid.size; i++) {
      ret += (rfid.uid.uidByte[i] < 0x10 ? "0" : "");
      ret += String(rfid.uid.uidByte[i], HEX);
    }
  }

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
  return ret;
}

//above for card sensor

int item;

// below are for app inventor
LBLEService AService("19B10010-E8F2-537E-4F6C-D104768A1214");
LBLECharacteristicInt ARead("19B10011-E8F2-537E-4F6C-D104768A1214", LBLE_READ | LBLE_WRITE);
void setup()
{
  SPI.begin();
  rfid.PCD_Init();
  // above 2 lines are for card 
  // Initialize LED pin
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  Serial.begin(9600);
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
  // above for app inventor and light sensor
  Wire.begin();
  SeeedOled.init();
  SeeedOled.deactivateScroll();
  SeeedOled.setPageMode();
  // for oled

}


void loop()
{
  if (ARead.isWritten()) {
    const char value = ARead.getValue();
    switch (value) {
      case 1:
        digitalWrite(LED_BUILTIN, HIGH);
        break;
      case 0:
        digitalWrite(LED_BUILTIN, LOW);
        break;
      default:
        Serial.println("Unknown value written");
        break;
    }
  }
  
  item = analogRead(A0);
  Serial.println(item);
  ARead.setValue(item);
  //above for for app inventor

  SeeedOled.clearDisplay();
  SeeedOled.setTextXY(0, 0);
  if(item>2300){
    SeeedOled.putString("light");
  }
  else{
    SeeedOled.putString("dark");
  }
  SeeedOled.setTextXY(1, 0);
  read_id = mfrc522_readID();
  if (read_id != "") {
    Serial.print("偵測到 RFID: ");
    Serial.println(read_id);
    if(read_id == "961a11f9"){
      SeeedOled.putString("hey husky");
    }else{
      SeeedOled.putString("hey stranger");
    }
  }

  //above for main programing aka all
  
  delay(2000);
}
