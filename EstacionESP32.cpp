// Importa las bibliotecas necesarias
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <DHTesp.h>

const char* ssid = "JVA";
const char* password = "11111111";

const char* host = "192.168.59.130";
const int port = 8080;

float humedad;
float temperatura;

DHTesp dht;
WiFiClient cliente;
char mensaje[50];


void setup() {
  Serial.begin(115200);
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
  pinMode(32, INPUT);
  dht.setup(32, DHTesp::DHT11);
}
void loop() {
  delay(1000);
  humedad = dht.getHumidity();
  Serial.print("humedad");
  temperatura = dht.getTemperature();
  Serial.print("temperatura");
  sprintf(mensaje, "T#%d#H#%d#\n", temperatura, humedad);
  Serial.printf("mensaje0:");
  delay(100);
  cliente.write(mensaje, sizeof(mensaje));
  Serial.printf(mensaje);
 }
