#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME "smart cooling"
#define BLYNK_AUTH_TOKEN ""

#define BLYNK_PRINT Serial

#include <DHT.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define DHTPIN D4  //sensor value
#define DHTTYPE DHT11
#define fan D2
#define pump D3

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "q";
char pass[] = "qwertyuiop";

DHT dht(DHTPIN, DHTTYPE);

int temp;
int hum;
int chill_level = 33;  // set temperature level
bool fan_Blynk = false;
bool pump_Blynk = false;
bool master = false;

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  pinMode(fan, OUTPUT);
  pinMode(pump, OUTPUT);
}

void loop() {

  Blynk.run();
  value_read();

  Serial.print("Temp ");
  Serial.print(temp);
  Serial.print("Hum ");
  Serial.println(hum);

  delay(2000);

  if (master) master_control();
  
  else{
    action();
    net_write();
  }

}

BLYNK_WRITE(V0) {
  fan_Blynk = param.asInt();
}
BLYNK_WRITE(V1) {
  pump_Blynk = param.asInt();
}
BLYNK_WRITE(V4) {
  master = param.asInt();
}

void action() {
  if (temp > chill_level) {
    digitalWrite(fan, LOW);
    digitalWrite(pump, LOW);
    Blynk.virtualWrite(V0, HIGH);
    Blynk.virtualWrite(V1, HIGH);
    Serial.print("on");
  } else {
    digitalWrite(fan, HIGH);
    digitalWrite(pump, HIGH);
    Blynk.virtualWrite(V0, LOW);
    Blynk.virtualWrite(V1, LOW);
    Serial.print("off");
  }
}

void master_control() {
  if (fan_Blynk) digitalWrite(fan, HIGH);
  else digitalWrite(fan, LOW);
  if (pump_Blynk) digitalWrite(pump, HIGH);
  else digitalWrite(pump, LOW);
}

void value_read() {
  temp = dht.readTemperature();
  hum = dht.readHumidity();
}  

void net_write() {
  Blynk.virtualWrite(V2, temp);
  Blynk.virtualWrite(V3, hum);
}
