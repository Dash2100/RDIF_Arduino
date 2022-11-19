#include <SPI.h>
#include <MFRC522.h>
#include <MFRC522Hack.h>

constexpr uint8_t RST_PIN = 9;
constexpr uint8_t SS_PIN = 10;

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522Hack mfrc522Hack(&mfrc522);

byte newUid[] = {0x00, 0x00, 0x00, 0x00};

MFRC522::MIFARE_Key key;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println(F("Program Started"));
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  Serial.println(newUid);
  Serial.println(F("請靠卡感應..."));
}

void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
    delay(50);
    return;
  }

  // Dump UID
  Serial.println(F("卡片原始:"));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  } 
  // Set new UID
  if ( mfrc522Hack.MIFARE_SetUid(newUid, (byte)4, true) ) {
    Serial.println(F("已設定新的UID"));
  }

  mfrc522.PICC_HaltA();
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
    return;
  }
  
  delay(2000);
}
