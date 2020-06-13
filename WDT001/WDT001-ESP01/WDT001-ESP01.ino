#include <WifiEspNow.h>
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

const int relay_pin = 2;
static uint8_t PEER[] {0xB6, 0xE6, 0x2D, 0x44, 0x74, 0x5F}; // MAC Address

void Callback(const uint8_t mac[6], const uint8_t* buf, size_t count, void* cbarg) {
  Serial.printf("Message from %02X:%02X:%02X:%02X:%02X:%02X\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  char msg [buf];
  for (int i = 0; i < count; ++i) {
    msg[i] = bufi[i]
    Serial.print(static_cast<char>(buf[i]));
  }
  Serial.println();
  if (msg == 'L' ){
  digitalWrite(relay_pin, HIGH);
  }
  else (if msg == 'D'){
    digitalWrite(relay_pin, LOW);
  }
}

void setup() {
  
  Serial.begin(115200);
  Serial.println();
  pinMode (relay_pin, OUTPUT);
  digitalWrite(relay_pin,LOW);
  WiFi.persistent(false);
  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESPNOW", nullptr, 3);
  WiFi.softAPdisconnect(false);
  Serial.print("MAC address of this node is ");
  Serial.println(WiFi.softAPmacAddress());
  bool ok = WifiEspNow.begin();
  if (!ok) {
    Serial.println("WifiEspNow.begin() failed");
    ESP.restart();
  }
  WifiEspNow.onReceive(Callback, nullptr);
  ok = WifiEspNow.addPeer(PEER);
  if (!ok) {
    Serial.println("WifiEspNow.addPeer() failed");
    ESP.restart();
  }
}

void loop() {
  char msg[60];
  int len = snprintf(msg, sizeof(msg), "hello ESP-NOW from %s at %lu", WiFi.softAPmacAddress().c_str(), millis());
  WifiEspNow.send(PEER, reinterpret_cast<const uint8_t*>(msg), len);
  delay(1000);
}
