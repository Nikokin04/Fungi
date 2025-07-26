//Realiza la impresión de las variables de interés, en el monitor serie
void showData(){

    Serial.println();

    Serial.print("PM 1.0: ");
    Serial.print(data_DB.PM_AE_UG_1_0);

    Serial.print("  PM 2.5:  ");
    Serial.print(data_DB.PM_AE_UG_2_5);

    Serial.print("  PM 10.0: ");
    Serial.print(data_DB.PM_AE_UG_10_0);

    Serial.print("  CO2: ");
    Serial.print(data_DB.CO2);
    
    Serial.print("  T: ");
    Serial.print(data_DB.Temperature);

    Serial.print("  H: ");
    Serial.print(data_DB.Humidity);
    
    Serial.print(F("   Lat: "));
    Serial.print(data_DB.latitud,4);
    Serial.print(F("   Long: "));
    Serial.print(data_DB.longitud,4);

    Serial.print(" Hora: ");
    Serial.print(timeRTC.YearRTC);
    Serial.print("-");
    Serial.print(timeRTC.MonthRTC);
    Serial.print("-");
    Serial.print(timeRTC.DayRTC);
    Serial.print(" ");
    Serial.print(timeRTC.HourRTC);
    Serial.print(":");
    Serial.print(timeRTC.MinuteRTC);
    Serial.print(":");
    Serial.println(timeRTC.SecondRTC);

    Serial.println();
}

//Realiza la impresión de la dirección MAC del punto de acceso al cual está conectada la ESP, en el monitor serie
void showMAC(){
  data_DB.mac = WiFi.macAddress();
  Serial.printf("MAC = %012X", data_DB.mac);
  Serial.println();
}

//Realiza la impresión de las credenciales de red guardadas en la memoria EEPROM, en el monitor serie
void showWifiConnection(){
  Serial.println("ssid EEPROM ");
  ssid_ = EEPROM.readString(0);
  Serial.println(ssid_);

  pass_ = EEPROM.readString(30);
  Serial.println("pass EEPROM ");
  Serial.println(pass_);

  Serial.println();
}
