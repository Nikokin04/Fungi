//Inicializa el NTC
void configNTC() {
  Serial.println("NTC configuration finished");
}

//Obtiene la fecha y hora actual del dispositivo
void getTimeNTC() {
  
  timeClient.update();

  unsigned long epochTime = timeClient.getEpochTime();
  Serial.print("Epoch Time: ");
  Serial.println(epochTime);
  
  struct tm *ptm = gmtime ((time_t *)&epochTime);

  timeRTC.HourRTC = timeClient.getHours();
  timeRTC.MinuteRTC = timeClient.getMinutes();
  timeRTC.SecondRTC = timeClient.getSeconds();
  timeRTC.DayRTC = ptm->tm_mday;
  timeRTC.MonthRTC = ptm->tm_mon+1;
  timeRTC.YearRTC = ptm->tm_year+1900;
}

//Imprime la fecha y hora actual del dispositivo, en el monitor serial
void viewTimeRtc() {
  Serial.print(timeRTC.YearRTC);
  Serial.print("-");
  Serial.print(timeRTC.MonthRTC);
  Serial.print("-");
  Serial.print(timeRTC.DayRTC);
  Serial.print(" ");
  Serial.print(timeRTC.HourRTC);
}
