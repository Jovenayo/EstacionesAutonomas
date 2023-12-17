#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>

const char* ssid = "JVA";
const char* password = "********";

const char* host = "192.168.121.130";
const int port = 8080;
WiFiClient cliente;

void setup() {
  Serial.begin(115200);
 
}

void loop() {

  delay(1000);
  cliente = WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi conectado");
  Serial.println(WiFi.localIP());

  
  if (!cliente.connect(host, port)) {
    Serial.println("No se puede conectar al host");
    return;
  }
  Serial.println("Conectado al host");
  char mensaje[] = "mensaje de ejemplo";
  cliente.write(mensaje, sizeof(mensaje));
  WiFi.disconnect();
 }
