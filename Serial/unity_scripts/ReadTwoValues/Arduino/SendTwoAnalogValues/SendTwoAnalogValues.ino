
int sensor1Value = 0;
int sensor2Value = 0;
int sensor3Value = 0;
int sensor4Value = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);
  pinMode(13,OUTPUT);
  pinMode(2,INPUT);
  pinMode(4,INPUT);
}

void loop() {
  digitalWrite(13,LOW);
  sensor1Value = analogRead(A0);
  sensor2Value = analogRead(A1);
  sensor3Value = analogRead(A2);
  sensor4Value = analogRead(A3);
  Serial.print(map(sensor1Value,0,900,0,100));
  Serial.print(",");
  Serial.print(map(sensor2Value,0,900,0,100));
  Serial.print(",");
  Serial.print(digitalRead(2));
  Serial.print(",");
  Serial.print(digitalRead(4));
  Serial.print(",");
  Serial.print(map(sensor3Value,0,900,0,100));
  Serial.print(",");
  Serial.println(map(sensor4Value,0,900,0,100));
  delay(50);
  digitalWrite(13,HIGH);

}
