//Inicializa la configuración del Fungi como punto de acceso
void initServer() {
  Serial.println();
  Serial.println("Configurando punto de acceso...");
  WiFi.softAP(ssid1);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  resetEEprom = true;
}

//Inicializa y configura el WebServer, en el cual se configuran las credenciales de red nuevas
void ConfigServer() {

  esp_task_wdt_deinit();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", menu_html);
  });

  server.on("/saveWifi", HTTP_GET, [] (AsyncWebServerRequest * request) {

    String inputUser, inputPassword;
    
    if (request->hasParam(USER)) {
      inputUser = request->getParam(USER)->value();
    }
    if (request->hasParam(PASSWORD)) {
      inputPassword = request->getParam(PASSWORD)->value();
    }

    pass = inputPassword.c_str();
    ssid = inputUser.c_str();

    pass_ = inputPassword;
    ssid_ = inputUser;

    Serial.print("ssid ");
    Serial.println(ssid);
    Serial.print("pass ");
    Serial.println(pass);
    flagConfigSsid = true;
    request->send(200, "text/html", "USER: " + inputUser + "<br> Pass: " + inputPassword);
  });

  server.on("/WifiConfig", HTTP_GET, [] (AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", wifi_html);
  });

  server.on("/info", HTTP_GET, [] (AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", info_html);
  });

  server.on("/close", HTTP_GET, [] (AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", close_html);
    server.end();
    WiFi.disconnect();
  });

    server.on("/ayuda", HTTP_GET, [] (AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", ayuda_html);
  });

  server.on("/sdCard", HTTP_GET, [] (AsyncWebServerRequest * request) {
    flagConfigSensors = true;
    server.end();
    WiFi.disconnect();    
  });

  server.onNotFound([](AsyncWebServerRequest * request) {

    request->send(404, "text/plain", "The content you are looking for was not found.");
  });
  server.begin();

}

//Inicializa el WIFI con las credenciales almacenadas
void configWifi(void){
  int counter = 0;
  Serial.printf("\nConnecting to %s", ssid_);
  WiFi.begin(ssid_.c_str(), pass_.c_str());
  int count = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    counter++;
    Serial.print(".");
    if(counter>10){
      counter = 0;
      break;
    }
  }
   if (WiFi.isConnected()) {
      Serial.println("\nConnected to network");
      Serial.print("My IP address is: ");
      Serial.println(WiFi.localIP());
      Serial.println("WIFI configuration finished");
   }else{
      Serial.println("WIFI connection failure");
      Serial.println("WIFI configuration finished");
   }
}

//Verifica la existencia de las credenciales de red
void existWifi(){
  if (ssid_ == "")
  {
    initServer();
    ConfigServer();
  }
  else {
    flagConfigWifi = true;
  }
}
