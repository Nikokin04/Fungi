//LIBRERÍAS
#include "Adafruit_Sensor.h"
#include "Adafruit_AM2320.h"

Adafruit_AM2320 am2320 = Adafruit_AM2320(); //Objeto Sensor AM2320

//Configura e inicializa el sensor AM2320
void ConfigAmd() {
  if (!am2320.begin()) {
     Serial.println("Sensor AMD not found, check wiring & pullups!");

  }
  delay(2000);
  Serial.println("AM2315 configuration finished");
}

//Realiza la lectura de la temperatura y la humedad del sensor
void Amd() {
  float aux1, aux2;
  aux1 = am2320.readTemperature();
  aux2 = am2320.readHumidity();

  if(aux1 != NAN){
    data_DB.Temperature = aux1;
  }
  else{
    Serial.print(aux1);
  }

  if(aux2 != NAN){
    data_DB.Humidity = aux2;
  }
  else{
    Serial.print(aux2);
  }   
}
