#include <SoftwareSerial.h>
#include <DHT.h>

SoftwareSerial BtSerial(10, 11); // RX -> 11 / TX -> 10

int IdEstacion;
#define DHT_PIN 3
#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  BtSerial.begin(9600);
  pinMode(5, INPUT);
  pinMode(4, INPUT);
  pinMode(8, INPUT);

  IdEstacion = 1;
  dht.begin();


}

void loop(){
  //HumedadSuelo ------------------------------------
  int humedadSuelo = analogRead(0);
  //-------------------------------------------------
  //Lluvia ------------------------------------------
  int lluvia = digitalRead(5);
  //-------------------------------------------------
  //TemperaturaHumedad-------------------------------ERROR SENSOR ROTO (NaN)
  float temperatura = dht.readTemperature();
  float humedad = dht.readHumidity();
  //-------------------------------------------------
  //Sonido---------------------------------------
  int sonido = digitalRead(8);

  //-------------------------------------------------
  //---------BLUETOOTH-------------------------------
  
  BtSerial.print("#Est.:");
  BtSerial.print(String(IdEstacion));
  BtSerial.print("#HumedadSuel:");
  BtSerial.print(String(humedadSuelo));
  BtSerial.print("#Lluvia:");
  BtSerial.print(String(lluvia));
  BtSerial.print("#Temperatura:");
  BtSerial.print(String(temperatura));
  BtSerial.print("#Humedad:");
  BtSerial.print(humedad);
  BtSerial.print("#Sonido: ");
  BtSerial.println(sonido);
  delay(1000);

}
