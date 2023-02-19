//motor test
int in1=6;
int in2=7;
int en=8;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

}

void loop() {
  digitalWrite(en, 127);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  delay(1000);

}
