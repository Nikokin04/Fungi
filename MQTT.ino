//
void SendDB(void) {   

  //Realiza la conexión MQTT
  if (!pg_status)
  {
	  Serial.print("Trying MQTT connection...");
    clientId = "\"ID\":\"0001\"";
    Serial.println(clientId);
    clientIdSD = "0001";
		if (cliente.connect(clientId.c_str(),mqtt_user,mqtt_pass)) {
      Serial.println("Connected!");
			if(cliente.subscribe(root_topic_subscribe)){
        Serial.println("Suscribed");
        pg_status = 1;
      }else{
        Serial.println("Subscription Failure");
        pg_status=0;
      }
		} else {
			Serial.print("Process Failed. Error -> ");
			Serial.print(cliente.state());
      pg_status=0;
		}
  }

  //Crea el directorio donde se guardan los archivos de texto
  if (pg_status == 1) {
    makepath();
    if(!SdOk){
      pg_status = 2;
      return;
    }
    if (!SD.exists(lastSyncDate.path)) {
      Serial.print("\n" + lastSyncDate.path + "\n");
      viewTimeRtc();
      pg_status = 1;
    }
    else{
      pg_status = 2;
    }    
   
  }

  //Asegura el estado adecuado de la SD
  if (pg_status == 2) {
    if (!SD.begin()) {
      Serial.println("Sincronization failed! Can't open SD Card");
      dbOk = false;
      return;
    }
    pg_status = 3;
  }

  //Transmite el mensaje al servidor MQTT, en formato JSON
  if (pg_status == 3) {
        Serial.println("Transmitting MQTT Message");

    if (cliente.connected()){

      String PM1P0 = "\"PM1_0\":" + String(data_DB.PM_AE_UG_1_0);
      String PM2P5 = "\"PM2_5\":" + String(data_DB.PM_AE_UG_2_5);
      String PM10 = "\"PM10\":"+ String(data_DB.PM_AE_UG_10_0);

      String CO2 = "\"CO2\":" + String(data_DB.CO2);

      String Temperatura = "\"Temp\":" + String(data_DB.Temperature);

      String Humedad = "\"Hum\":" + String(data_DB.Humidity);

      String Latitud = "\"Lat\":" + String(data_DB.latitud, 4);

      String Longitud = "\"Long\":" + String(data_DB.longitud, 3);

      String Fecha = "\"Fecha\":\"" + String(timeRTC.YearRTC) + "-" + sMes + "-" + sDia + "T" + sHora + ":" + sMinuto + ":" + sSegundo + "Z\"";

      String IP = "\"IP\":\"" + WiFi.localIP().toString() + "\"";

      String mensaje = "{" + clientId + "," + PM1P0 + "," + PM2P5 + "," + PM10 + "," + CO2 + "," + Temperatura + "," + Humedad + "," + Latitud + "," + Longitud + "," + Fecha + "," + IP + "}";

      char message[mensaje.length() + 1];

      mensaje.toCharArray(message, mensaje.length() + 1);

      cliente.publish(root_topic_publish,message);
      
      pg_status = 1;

    } 
  }
}

//Crea el directorio donde se guardan los archivos de texto
void makepath(void) {
  if (timeRTC.SecondRTC < 10) {
    sSegundo = "0" + String(timeRTC.SecondRTC);
  } else {
    sSegundo = String(timeRTC.SecondRTC);
  }
  if (timeRTC.MinuteRTC < 10) {
    sMinuto = "0" + String(timeRTC.MinuteRTC);
  } else {
    sMinuto = String(timeRTC.MinuteRTC);
  }
  if (timeRTC.HourRTC < 10) {
    sHora = "0" + String(timeRTC.HourRTC);
  } else {
    sHora = String(timeRTC.HourRTC);
  }
  if (timeRTC.DayRTC < 10) {
    sDia = "0" + String(timeRTC.DayRTC);
  } else {
    sDia = String(timeRTC.DayRTC);
  }
  if (timeRTC.MonthRTC < 10) {
    sMes = "0" + String(timeRTC.MonthRTC);
  } else {
    sMes = String(timeRTC.MonthRTC);
  }
  lastSyncDate.path = "/" + String(timeRTC.YearRTC) + sMes + "/" + sDia + "/" + sHora + ".txt";
}


