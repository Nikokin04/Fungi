/*
 ----------------------------------------------------------------------------------------------------
 |                     VERSIÓN NO.5 DEL CÓDIGO PARA LA PROGRAMACIÓN DEL FUNGI                       |
 ----------------------------------------------------------------------------------------------------

  NOVEDADES:
    - Se cambió el método de transmisión de los dispositivos, agregando la librería PubSubClient y el protocolo de red MQTT.
    - Se cambió el método de obtención de la información sobre la hora y fecha, por medio de la utilización del NTP (Network Time Protocol).
  CHANGES MADE BY: Nicolás Quintero

*/

//LIBRERÍAS
#include "MAIN.h"

//INSTANCIAS
TaskHandle_t ReadingAndTransmission; //Definición de la tarea de configuración, lectura y transmisión de datos
TaskHandle_t LedRutineAndFTP; //Definición de la tarea de rutina de leds y protocolo FTP

//VARIABLES

//-----------------------------//
//Variables para la configuración del protocolo MQTT
const char *mqtt_server = "***.**.***.***";
const int mqtt_port = *****;
const char *mqtt_user = "";
const char *mqtt_pass = "";
const char *root_topic_subscribe = "Fungi/input";
const char *root_topic_publish = "Fungi/output";
//-----------------------------//


String formattedDate; //Variable para obtener la fecha en el formato yyyy-mm-dd

//Variables adicionales
bool flagMedir = true; 
bool flagTx = false;


void setup() {

  //Inicializa el almacenamiento de información en el espacio "my-app", configurable en la memoria flash no volátil del dispositivo y ordena no borrar la información antes almacenada
  preferences.begin("my-app", false); 

  //Asigna la actividad del botón como interrupción, para configurar el Fungi como punto de acceso
  pinMode(BUTTON_PIN,  INPUT); 
  attachInterrupt(BUTTON_PIN, isr, FALLING);


  Serial.begin(SERIAL_BAUD); //Se inicializa la comunicación Serial con el MCU

  Wire.begin(); //Inicia la comunicación I2C, para la recepción de la información suminstrada por el sensor de C02.

  configNTC(); //Inicializa el NTC
  showMAC(); //Imprime en el monitor serial, la dirección MAC del punto de acceso al cual está conectada la ESP
  initIndicator(); //Inicializa el arreglo de LEDS
  initEeprom(); //Inicializa la memoria EEPROM
  showWifiConnection(); //Imprime en el monitor serial, las credenciales de red almacenadas en la EEPROM 
  existWifi(); //Verifica la existencia de las credenciales de red

  cliente.setServer(mqtt_server, mqtt_port); //Se configura el cliente MQTT con las credenciales del servidor MQTT

  //Inicialización del sistema, visualizada a través del arreglo de LEDS
  rainbowFade2White(3, 5, 1);

  //Se crea la tarea No.1, relacionada a la lectura y transmisión de datos, con prioridad de 1 y asignado al primer núcleo
  xTaskCreatePinnedToCore(Ejecuta_Tarea1,"ReadingAndTransmission",10000,NULL,1,&ReadingAndTransmission,1); 

  //Se crea la tarea No.2, relacionada a la rutina de LEDS y al protocolo FTP, con prioridad de 2 y asignado al primer núcleo
  xTaskCreatePinnedToCore(Ejecuta_Tarea2,"LedRutineAndFTP",10000,NULL,2,&LedRutineAndFTP,1);  

}

//FUNCIONES

//Función que define la tarea No.1
void Ejecuta_Tarea1( void * pvParameters ){
  for(;;){

    //Inicializa el temporizador de envío, para que cada 1 min transcurrido, se realice la lectura y envío de datos
    if (flagMedir) {
      timerData();
    }

    //Configura las credenciales de red, en caso que el fungi haya entrado en modo de punto de acceso
    if (flagConfigSsid)
    {
      flagConfigSsid = false;
      eepromData();
    }

    if (flagConfigWifi)
    {
      Serial.println("Config Wifi");
      flagConfigWifi = false;

      server.end();//Termina cualquier conexión con el WebServer
      WiFi.disconnect(); //Desconecta la ESP de la red WIFI

      configWifi(); //Configura la conexión WIFI de la ESP 
      
      //En caso de conexión a WIFI, se inicializa el cliente NTP, para obtener la fecha y hora del dispositivo.
      if (WiFi.isConnected()) {
        timeClient.begin();
        timeClient.update();

        formattedDate = timeClient.getFormattedDate();
        Serial.println(formattedDate);
      }
      flagConfigSensors = true;
    }      

    //Configuración de los sensores y protocolos
    if(flagConfigSensors)
    {
      flagConfigSensors = false;
      configSD();
      configCO2();
      configGPS();
            
      configFTP();
      ConfigAmd();
      configPMS();
      
      getTimeNTC(); //Obtiene la fecha y hora actual del dispositivo
      ConfigEnd = true;
    }
  
    if (ConfigEnd)
    {
      Pms(); //Realiza la lectura del sensor PMS5003 dependiendo del tiempo de inicialización del mismo
      
      if (flagPMS)
      {
        //Asegura la conexión WIFI antes de realizar la lectura general de todos los sensores, protocolos y dispositivos
         if (!WiFi.isConnected()) {

          configWifi(); //Configura la conexión WIFI de la ESP 

          //Inicializa el cliente NTP, para obtener la fecha y hora del dispositivo
          timeClient.begin();

          timeClient.update();

          formattedDate = timeClient.getFormattedDate();
          Serial.println(formattedDate); 
        }
  
        flagPMS = false;

        gps(); //Lee los valores del GPS
        getTimeNTC(); //Obtiene la fecha y hora actual del dispositivo
        createFile(); //Crea el archivo de texto dentro de la SD
        CO2(); //Lee los valores del sensor de CO2
        Amd(); //Lee los valores del sensor AM2320
        sdCard_addData(); //Escribe las variables de interés en el archivo de texto
        showData(); //Imprime en el monitor serial, las variables de interés 
        pg_status = 0;
        flagMedir= true;
      }
      //Verifica la conexión WIFI para realizar el envío de datos
      if (WiFi.isConnected()) {
        if (flagTx){
          SendDB();
          flagTx=false;
        }
      }
    }
  } 
}

//Función que define la tarea No.2
void Ejecuta_Tarea2( void * pvParameters ){
  for(;;){
    //Ejecuta la rutina de LEDS general
    if (flagCallbackTimer) {
        timerLeds(); //Inicializa el temporizador de envío, para que cada 1 min transcurrido, se realice la lectura y envío de datos
      }

    indicativeCO2(); //Ejecuta la rutina de LEDS en relación al CO2 presente en el ambiente, además de los LEDS indicadores de los estados del WIFI, la SD y el GPS

    //Inicializa el protocolo FTP
    if (WiFi.status() == WL_CONNECTED) {
      loopFTP();
    }
  }
}

void loop(){

}

//Función que inicializa el temporizador de envío, para que cada 1 min transcurrido, se realice la lectura y envío de datos
void timerData() {
  static uint32_t timerLast = 0;
  uint32_t timerNow = millis();
  if (timerNow - timerLast >= 30000)
  {
    timerLast = timerNow;
    flagPMS = true;
    flagMedir= false;
    flagTx = true;
  }
}
