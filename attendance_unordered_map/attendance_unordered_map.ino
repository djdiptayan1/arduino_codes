#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <SD.h>
#include <ESP8266WiFi.h>
#include <unordered_map>
#include <string>

#define tot 5  //sets total number of people
//WIFI
const char* ssid = "DIPTAYAN-JASH 0982";
const char* password = "G662+28k";

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

//relay
constexpr uint8_t RELAY_PIN = D0;

//ID CARD DETAILS
std::unordered_map<std::string, std::string> id = {
  { "249224147101", "Diptayan" },
  { "20115312290", "Krrish" },
  { "105199145101", "Ayan" },
  { "2176091112", "Daniel" },
  { "89195136101", "Siddhima" },
  { "732146101", "Akhil" },
  { "12146146101", "Sanjeevini" },
  { "20124314493", "Divya" },
  { "5714814893", "Kannan" },
  { "2011015993", "Anand" },
  { "2124819651", "Roel" },
  { "24921115293", "Hanna" },
  { "2282520551", "Adheesh" },
  { "23318615593", "Vijay" },
  { "24917615793", "Rupkatha" }
};

void setup() {
  lcd.init();
  lcd.clear();
  lcd.backlight();
  Serial.begin(9600);
  SPI.begin();      // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522
  pinMode(RELAY_PIN, OUTPUT);

  //SD CARD
  sdinitialize();
  //wifi
  //connectwifi();
}

void loop() {
  digitalWrite(RELAY_PIN, HIGH);
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
    Serial.println(tag);

    //checking using UNORDERED MAPS
    std::string find = tag.c_str();
    if (id.find(find) != id.end()) {
      flag = 1;
    } else
      flag = 0;

    if (flag == 1) {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Welcome");
      lcd.setCursor(1, 1);
      String strfind = id[find].c_str();
      Serial.println(strfind);  //check
      lcd.println(strfind);     //check
      delay(2000);
      myfile = SD.open("attendance.csv", FILE_WRITE);
      data = strfind + "," + tag;  //check
      myfile.println(data);
      Serial.println("WROTE IN FILE SUCCESSFUL");
      myfile.close();
      digitalWrite(RELAY_PIN, LOW);
      delay(6000);
      digitalWrite(RELAY_PIN, HIGH);
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