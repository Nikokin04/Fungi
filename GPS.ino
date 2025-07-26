//DEFINICIONES
#define SFE_UBLOX_REDUCED_PROG_MEM

//Inicializa el GPS por I2C 
void configGPS(){  
  if (myGPS.begin() == false)
  {
    Serial.println(F("Ublox GPS not detected at default I2C address. Please check wiring."));
  }
  myGPS.setI2COutput(COM_TYPE_UBX); //Set the I2C port to output UBX only (turn off NMEA noise)
  myGPS.saveConfiguration();        //Save the current settings to flash and BBR
  Serial.println("GPS configuration finished");
}

void gps(){  

  myGPS.checkUblox(); //Solicita la lectura de los valores del GPS cada segundo, para evitar el tráfico de la comunicación I2C
      
  data_DB.latitud = (float)myGPS.getLatitude()/10000000; //Se debe dividir por 1e7, por la codificación de la librería
  Serial.println(String(data_DB.latitud));
   
  data_DB.longitud = (float)myGPS.getLongitude()/10000000; //Se debe dividir por 1e7, por la codificación de la librería
  Serial.println(String(data_DB.longitud));

  //Guarda las variables de longitud y latitud en variables auxiliares, en caso de desconexión del GPS
  if((data_DB.latitud && data_DB.longitud) != 0.0000){
      data_DB.lat_aux = data_DB.latitud;
      data_DB.long_aux = data_DB.longitud;
  }
  else{
    data_DB.latitud = data_DB.lat_aux;
    data_DB.longitud = data_DB.long_aux;
  }

  if(data_DB.latitud && data_DB.longitud){

    preferences.putFloat("latitud", data_DB.latitud); //Guarda la información de la variable en la memoria flash no volátil
    preferences.end();

    preferences.putFloat("longitud", data_DB.longitud); //Guarda la información de la variable en la memoria flash no volátil
    preferences.end();

  }
  else{
       data_DB.latitud = preferences.getFloat("latitud", 0); //Obtiene la información de la variable en la memoria flash no volátil (en caso de no existir, se asigna en ceros)
       data_DB.longitud = preferences.getFloat("longitud", 0); //Obtiene la información de la variable en la memoria flash no volátil (en caso de no existir, se asigna en ceros)
  }
}
