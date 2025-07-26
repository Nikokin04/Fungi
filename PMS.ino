//Inicializa el sensor PMS, conectado por comunicación serial
void configPMS(){
  Serial2.begin(PMS::BAUD_RATE);
  Serial.println("PMS configuration finished");
  Serial.println("----------------------------------------------------------------------------");
}

//Realiza la lectura del sensor PMS5003 dependiendo del tiempo de inicialización del mismo y el intervalo en el que se desea transmitir
void Pms(){
  static uint32_t timerLast = 0;
  uint32_t timerNow = millis();
  if (timerNow - timerLast >= timerInterval)
  {
    timerLast = timerNow;
    timerCallback();
    timerInterval = timerInterval == PMS_READ_DELAY ? PMS_READ_INTERVAL : PMS_READ_DELAY;
  }
}

void timerCallback() {
  if (timerInterval == PMS_READ_DELAY)
  {
    readData();
  }
  else
  {
    Serial.println("----------------------------------------------------------------------------");
  }
}

void readData(){
  pms.requestRead(); //Pregunta para hacer la lectura del sensor PMS
  if (pms.readUntil(data))
  {
    data_DB.PM_AE_UG_1_0  = data.PM_AE_UG_1_0;
    data_DB.PM_AE_UG_2_5  = data.PM_AE_UG_2_5;
    data_DB.PM_AE_UG_10_0 = data.PM_AE_UG_10_0;

    /*
    Al usar el sensor AM2320, se comentan estas líneas por redundancia en lectura de las mismas variables

    data.Temperature /= 10;
    data_DB.Temperature = data.Temperature;

    data.Humidity /= 10;
    data_DB.Humidity = data.Humidity;

    */

  }
}
