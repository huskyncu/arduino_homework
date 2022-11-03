/*
   Generated using BlocklyDuino:

   https://github.com/MediaTek-Labs/BlocklyDuino-for-LinkIt

   Date: Tue, 29 Jan 2019 06:28:05 GMT
*/
#include <Arduino.h>
#include <LRemote.h>
#include "DHT.h"
#include "sensirion_common.h"
#include "sgp30.h"
//溫溼度跟亮度
int temperature;

int humidity;

int luminance_read;

//原本的label for溫溼度
LRemoteLabel label1_0;
LRemoteLabel label1_1;
LRemoteLabel label1_2;
LRemoteLabel label1_3;
LRemoteLabel label2_0;
LRemoteLabel label2_1;
LRemoteLabel label2_2;
LRemoteLabel label2_3;
LRemoteLabel label3_0;
LRemoteLabel label3_1;
LRemoteLabel label3_2;
LRemoteLabel label3_3;
//偵測溫溼度的東西。
DHT __dht2(2, DHT22);

//宣告遙控車需要的variable。
LRemoteLabel label1;
LRemoteButton button_forward;
LRemoteButton button_backward;
LRemoteButton button_left;
LRemoteButton button_right;
LRemoteButton button_stop;

//氣體感測
void setup()
{
  //以下是本來遙控車給的介面code，到時候再請你們修正！
  LRemote.setName("7679 Remote");
  LRemote.setOrientation(RC_PORTRAIT);
  LRemote.setGrid(3, 8);
  label1.setPos(0, 0);
  label1.setText("7697 Robot");
  label1.setSize(3, 1);
  label1.setColor(RC_ORANGE);
  LRemote.addControl(label1);

  button_forward.setPos(1, 1);
  button_forward.setText("^");
  button_forward.setSize(1, 1);
  button_forward.setColor(RC_BLUE);
  LRemote.addControl(button_forward);

  button_backward.setPos(1, 3);
  button_backward.setText("v");
  button_backward.setSize(1, 1);
  button_backward.setColor(RC_BLUE);
  LRemote.addControl(button_backward);

  button_left.setPos(0, 2);
  button_left.setText("<");
  button_left.setSize(1, 1);
  button_left.setColor(RC_BLUE);
  LRemote.addControl(button_left);

  button_right.setPos(2, 2);
  button_right.setText(">");
  button_right.setSize(1, 1);
  button_right.setColor(RC_BLUE);
  LRemote.addControl(button_right);

  button_stop.setPos(1, 2);
  button_stop.setText("o");
  button_stop.setSize(1, 1);
  button_stop.setColor(RC_YELLOW);
  LRemote.addControl(button_stop);
  LRemote.begin();
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  //溫溼度setup
  LRemote.begin();
  __dht2.begin();

  //共陽極setup
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);

  //氣體感測(我看不懂他在寫什麼但他用了我就用，參考網址：https://github.com/Seeed-Studio/SGP30_Gas_Sensor/blob/master/examples/base_example/base_example.ino)
  s16 err;
  u16 scaled_ethanol_signal, scaled_h2_signal;
  //Serial.begin(115200);
  Serial.println("serial start!!");

//  /*For wio link!*/
//#if defined(ESP8266)
//  pinMode(15, OUTPUT);
//  digitalWrite(15, 1);
//  Serial.println("Set wio link power!");
//  delay(500);
//#endif
  /*  Init module,Reset all baseline,The initialization takes up to around 15 seconds, during which
      all APIs measuring IAQ(Indoor air quality ) output will not change.Default value is 400(ppm) for co2,0(ppb) for tvoc*/
  while (sgp_probe() != STATUS_OK) {
    Serial.println("SGP failed");
    while (1);
  }
  /*Read H2 and Ethanol signal in the way of blocking*/
  err = sgp_measure_signals_blocking_read(&scaled_ethanol_signal,
                                          &scaled_h2_signal);
  if (err == STATUS_OK) {
    Serial.println("get ram signal!");
  } else {
    Serial.println("error reading signals");
  }
  err = sgp_iaq_init();
  //
}


void loop()
{
  //溫溼度感測
  temperature = __dht2.readTemperature();
  humidity = __dht2.readHumidity();
  label1_2.updateText(String(temperature));
  label2_2.updateText(String(humidity));

  //亮度感測
  luminance_read = analogRead(A0);
  //label3_2.updateText(String(luminance_read));
  if (luminance_read > 100) {
    //熄滅。
    digitalWrite(15, HIGH);
    digitalWrite(16, HIGH);
    digitalWrite(17, HIGH);
  }
  else {
    //變亮(白色)
    digitalWrite(15, LOW);
    digitalWrite(16, LOW);
    digitalWrite(17, LOW);
    delay(5000);//亮5秒
    //熄滅
    digitalWrite(15, HIGH);
    digitalWrite(16, HIGH);
    digitalWrite(17, HIGH);
  }

  //氣體感測(參考網址：https://github.com/Seeed-Studio/SGP30_Gas_Sensor/blob/master/examples/base_example/base_example.ino)
  s16 err = 0;
  u16 tvoc_ppb, co2_eq_ppm; //氣體的變數。
  err = sgp_measure_iaq_blocking_read(&tvoc_ppb, &co2_eq_ppm);
  if (err == STATUS_OK) {
    Serial.print("tVOC  Concentration:");
    Serial.print(tvoc_ppb);
    Serial.println("ppb");

    Serial.print("CO2eq Concentration:");
    Serial.print(co2_eq_ppm);
    Serial.println("ppm");
  } else {
    Serial.println("error reading IAQ values\n");
  }

  //原本遙控車的code！
  LRemote.process();
  if (button_forward.isValueChanged()) {
    if (button_forward.getValue() == 1) {
      label1.updateText(String("Forward"));
      digitalWrite(10, HIGH);
      digitalWrite(11, LOW);
      digitalWrite(12, HIGH);
      digitalWrite(13, LOW);

    } else {
      label1.updateText(String("STOP"));
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(13, LOW);

    }

  }
  if (button_backward.isValueChanged()) {
    if (button_backward.getValue() == 1) {
      label1.updateText(String("Backward"));
      digitalWrite(10, LOW);
      digitalWrite(11, HIGH);
      digitalWrite(12, LOW);
      digitalWrite(13, HIGH);

    } else {
      label1.updateText(String("STOP"));
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(13, LOW);

    }

  }
  if (button_left.isValueChanged()) {
    if (button_left.getValue() == 1) {
      label1.updateText(String("LEFT"));
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, HIGH);
      digitalWrite(13, LOW);

    } else {
      label1.updateText(String("STOP"));
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(13, LOW);

    }

  }
  if (button_right.isValueChanged()) {
    if (button_right.getValue() == 1) {
      label1.updateText(String("RIGHT"));
      digitalWrite(10, HIGH);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(13, LOW);

    } else {
      label1.updateText(String("STOP"));
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(13, LOW);

    }

  }
  if (button_stop.isValueChanged()) {
    if (button_stop.getValue() == 1) {
      label1.updateText(String("STOP"));
      digitalWrite(10, LOW);
      digitalWrite(11, LOW);
      digitalWrite(12, LOW);
      digitalWrite(13, LOW);
    }

  }
}
