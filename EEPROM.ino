//Inicializa la memoria EEPROM
void initEeprom(){
  if (!EEPROM.begin(EEPROM_SIZE)){
    Serial.println("failed to initialise EEPROM"); delay(1000000);
  }
  else{
     Serial.println("EEPROM configuration finished!");
  }
}

//
void eepromData(){
  escribirDatos(); //Escribe las credenciales de red en la memoria EEPROM
  showWifiConnection(); //Realiza la impresión de las credenciales de red guardadas en la memoria EEPROM, en el monitor serie
  flagConfigWifi = true;
}

//Escribe las credenciales de red en la memoria EEPROM
void escribirDatos(){
  EEPROM.writeString(0, ssid_); //Escribe el identificador o nombre de red en la dirección de memoria 0 en la EEPROM
  EEPROM.writeString(30,pass_); //Escribe el identificador o nombre de red en la dirección de memoria 30 en la EEPROM
  EEPROM.commit(); //Guarda y confirma los cambios realizados a las direcciones de memoria
}

//Imprime el contenido de la memoria EEPROM, en el monitor serial
void memoria(){
  Serial.println("EEPROM ");
  for (int i = 0; i < EEPROM_SIZE; i++)
  {
    Serial.print((char)EEPROM.read(i));
  }
}

//Resetea las direcciones de memoria de la EEPROM destinadas a las credenciales
void btCallback() {
  EEPROM.writeString(0, "");
  EEPROM.writeString(30, "");
  EEPROM.commit();
  memoria();
  preferences.clear();
  preferences.end();
  ESP.restart();
}

//Configura la función de la interrupción y ejecuta la función que resetea las credenciales guardadas en la EEPROM
void IRAM_ATTR isr() {
  btCallback();
  Serial.println("Restart Memory");  
}

