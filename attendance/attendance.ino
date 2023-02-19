#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <SD.h>

#define tot 5  //sets total number of people

//DISPLAY
LiquidCrystal_I2C lcd(0x3F, 16, 2);
int flag;
int pos = 0;
//RFID
constexpr uint8_t RST_PIN = D3;
constexpr uint8_t SS_PIN = D4;
MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of the class
MFRC522::MIFARE_Key key;
String tag;

//SDCARD
File myfile;                //create a file named attendance
constexpr uint8_t cs = D8;  //NODE MCU


//ID CARD DETAILS
String UID[tot] = { "20115312290", "249224147101", "105199145101", "2176091112", "89195136101" };
String name[tot] = { "Krrish", "Diptayan", "Ayan", "Daniel", "Siddhima" };

void setup() {
  lcd.init();
  lcd.clear();
  lcd.backlight();
  Serial.begin(9600);
  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522

  //SD CARD
  Serial.println("Initializing SD card");
  if (!SD.begin(cs)) {
    Serial.println("initialization failed");
    while (1)
      ;
  }
  Serial.println("initialization DONE");
  if (SD.exists("attendance.csv"))
    Serial.println("FILE EXISTS");
  else
    Serial.println("FILE DOESN'T EXISTS");
}

void loop() {
  String data = "";
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("scan");
  lcd.setCursor(1, 1);
  if (!rfid.PICC_IsNewCardPresent())
    return;
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    //checking
    for (int i = 0; i < tot; i++) {
      if (tag == UID[i]) {
        flag = 1;
        pos = i;
        break;
      } else {
        flag = 0;
      }
    }

    if (flag == 1) {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Welcome");
      lcd.setCursor(1, 1);
      Serial.println(name[pos]);
      lcd.println(name[pos]);
      delay(2000);
      myfile = SD.open("attendance.csv", FILE_WRITE);
      data = name[pos] + "," + UID[pos];
      myfile.println(data);
      Serial.println("WROTE IN FILE SUCCESSFUL");
      myfile.close();
    } else {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Who Tf is You ?");
      lcd.setCursor(1, 1);
      lcd.print(tag);
      Serial.println(tag);
      delay(2000);
    }
    tag = "";
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
}