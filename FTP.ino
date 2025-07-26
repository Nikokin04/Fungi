//Configura el retorno del programa, en caso de intentar la conexión a través del protocolo FTP
void _callback(FtpOperation ftpOperation, unsigned int freeSpace, unsigned int totalSpace){
  switch (ftpOperation) {
    case FTP_CONNECT:
      Serial.println(F("FTP Connected!"));
      break;
    case FTP_DISCONNECT:
      Serial.println(F("FTP Disconnected!"));
      break;
    case FTP_FREE_SPACE_CHANGE:
      Serial.printf("FTP: Free space change, free %u of %u!\n", freeSpace, totalSpace);
      break;
    default:
      break;
  }
};

//Configura el retorno del programa, en caso de realizar transferencias de archivos 
void _transferCallback(FtpTransferOperation ftpOperation, const char* name, unsigned int transferredSize){
  switch (ftpOperation) {
    case FTP_UPLOAD_START:
      Serial.println(F("FTP Upload start!"));
      break;
    case FTP_UPLOAD:
      Serial.printf("FTP Upload of file %s byte %u\n", name, transferredSize);
      break;
    case FTP_TRANSFER_STOP:
      Serial.println(F("FTP Finish transfer!"));
      break;
    case FTP_TRANSFER_ERROR:
      Serial.println(F("FTP Transfer error!"));
      break;
    default:
      break;
  }
};

//Inicializa el protocolo FTP 
void configFTP(void) {
  //Configura los retornos de conexión y transferencia e inicializa el protocolo con las credenciales escritas
  if (SD.begin()){
      ftpSrv.setCallback(_callback);
      ftpSrv.setTransferCallback(_transferCallback);
      Serial.println("FTP configuration finished!");  
      ftpSrv.begin("DIT","FUNGI2024"); //Username and password for FTP. Default port is 21
  }   
}

//Método para asegurar la funcionalidad del FTP 
void loopFTP(void) {
  ftpSrv.handleFTP();       
}
