#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <SD.h>
#include <ESP8266WiFi.h>

#define tot 5     //sets total number of people
#define totfac 1  //sets total number of faculty
//WIFI
const char* ssid = "DIPTAYAN-JASH 0982";
const char* password = "G662+28k";

//DISPLAY
LiquidCrystal_I2C lcd(0x3F, 16, 2);
int flag;
int fac_flag;
int pos;
int fac_pos;

//RFID
constexpr uint8_t RST_PIN = D3;
constexpr uint8_t SS_PIN = D4;
MFRC522 rfid(SS_PIN, RST_PIN);  // Instance of the class
MFRC522::MIFARE_Key key;
String tag;
String faculty_tag;

//SDCARD
File myfile;                //create a file named attendance
constexpr uint8_t cs = D8;  //NODE MCU


//ID CARD DETAILS
String facUID[totfac] = { "249224147101" };
char facname[totfac] = { 'm' };
String UID[tot] = { "20115312290", "105199145101", "2176091112", "89195136101" };
String name[tot] = { "Krrish", "Ayan", "Daniel", "Siddhima" };

char fac;

void setup() {
  lcd.init();
  lcd.clear();
  lcd.backlight();
  Serial.begin(9600);
  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522

  //SD CARD
  sdinitialize();
  //wifi
  //connectwifi();
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Faculty scan");
  Serial.println("FACULTY SCAN");
  lcd.setCursor(1, 1);
}

void loop() {
  String data = "";
  if (!rfid.PICC_IsNewCardPresent())
    return;
  if (rfid.PICC_ReadCardSerial()) {
    for (byte i = 0; i < 4; i++) {
      faculty_tag += rfid.uid.uidByte[i];
    }
    Serial.println("faculty tag=========" + faculty_tag);
    for (int i = 0; i < totfac; i++) {
      if (faculty_tag == facUID[i]) {
        fac_flag = 1;
        fac_pos = i;
        break;
      } else
        fac_flag = 0;
    }
    if (fac_flag == 1) {
      switch (facname[fac_pos]) {
        case 'm':
          Serial.println("faculty : " + faculty_tag + "\t" + facname[fac_pos]);
          lcd.clear();
          lcd.setCursor(2, 0);
          lcd.print(" student scan");
          Serial.println(" Student scan");
          /**     read RFID again
                for loop
                if (tag==UID[i])
                  print and store and send to data base
                if (tag == facUID)
                  System.exit(0);
        **/
          if (!rfid.PICC_IsNewCardPresent())
            return;
          if (rfid.PICC_ReadCardSerial()) {
            for (byte i = 0; i < 4; i++) {
              tag += rfid.uid.uidByte[i];
            }
            for (int i = 0; i < tot; i++) {
              if (tag == UID[i]) {
                flag = 1;
                pos = i;
                break;
              } else
                flag = 0;
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
          }
          break;
        default:
          Serial.println("INVALID");
      }
    } else Serial.println("INVALID FACULTY");

    tag = "";
    faculty_tag = "";
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
}
void connectwifi() {
  Serial.print("Wifi connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  Serial.println();
  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wifi Connected Success!");
  Serial.print("NodeMCU IP Address : ");
  Serial.println(WiFi.localIP());
}
void sdinitialize() {
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