//Configura e inicializa el sensor DE CO2
void configCO2() {
  Serial.println("CO2 configuration finished");
}

//Realiza la lectura de la temperatura y la humedad del sensor
void CO2() {
  Wire.requestFrom(I2C_SLAVE, 12); //Solicita al dispositivo esclavo la información a capturar, en este caso, 12 bytes

  //Mientras el I2C esté disponible, en cada posición del arreglo dataC02, guarda la información del sensor
  while (Wire.available()) 
  { 
    dataCO2[indexCO2] = Wire.read();
    checkSum();
    indexCO2++;
  }
  if (indexCO2 >= 12)
  {
    checkSum();
    indexCO2 = 0;
    if (checksum == dataCO2[CHECKSUM]) {
      checksum = 0;
      data_DB.CO2 = (((int)dataCO2[CO2_HIGH] << 8) | (int)dataCO2[CO2_LOW]);
    }
  }
}

//Recorre el arreglo y almacena los datos leidos en las posiciones del mismo, de manera consecutiva
void checkSum(void) {
  if (indexCO2 < 10)
  {
    checksum += dataCO2[indexCO2];
  }
  if (indexCO2 >= 12)
  {
    checksum = checksum & 0xFF;
  }
}
