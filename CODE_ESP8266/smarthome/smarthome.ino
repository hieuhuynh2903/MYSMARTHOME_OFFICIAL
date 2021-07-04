#include <common.h>
#include <FirebaseESP8266.h>
#include <FirebaseFS.h>
#include <Utils.h>


#include <ESP8266WiFi.h>
#include <WiFiManager.h>

#include <SerialCommand.h>
#include <SoftwareSerial.h>


#define D3 (2)
#define D4 (0)

SerialCommand CMD;
SoftwareSerial mySerial;
WiFiManager wifiManager;

//Wifi parameters
#define WIFI_SSID "hieupc"
#define WIFI_PASSWORD "12345678"

//Firebase parameters
#define FIREBASE_HOST "thesis-720fc-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "9mLBvhsu6h2aC1EnPOBET28cnUvKKg7eWLSrhcG1"   
FirebaseData firebasedata;
FirebaseData firebasedataStream;

//Command Function
void DHT11() {
  String arg1;
  String arg2;
  arg1 = CMD.next();
  Firebase.setString(firebasedata, "/sensor/DHT11/temp", arg1);
  arg2 = CMD.next();
  Firebase.setString(firebasedata, "/sensor/DHT11/humi", arg2);
}

void PIR() {
  //char *arg;
  String arg;
  arg = CMD.next();
  if(arg == "0"){
    Firebase.setBool(firebasedata,"/sensor/PIR",false);
  }else if(arg == "1"){
    Firebase.setBool(firebasedata,"/sensor/PIR",true);
  }
}

void RAIN() {
  String arg;
  arg = CMD.next();
  if(arg == "0"){
    Firebase.setBool(firebasedata,"/sensor/RAIN",false);
  }else if(arg == "1"){
    Firebase.setBool(firebasedata,"/sensor/RAIN",true);
  }
}

void FLAME() {
  String arg;
  arg = CMD.next();
  if(arg == "0"){
    Firebase.setBool(firebasedata,"/sensor/FLAME",false);
  }else if(arg == "1"){
    Firebase.setBool(firebasedata,"/sensor/FLAME",true);
  }
}

void GAS() {
  String arg;
  arg = CMD.next();
  Firebase.setString(firebasedata, "/sensor/GAS", arg);
}

void HALL(){
  String arg;
  arg = CMD.next();
  if (arg == "0"){
    Firebase.setBool(firebasedata,"/sensor/HALL",false);
  }else if(arg == "1"){
    Firebase.setBool(firebasedata,"/sensor/HALL",true);
  }

}

void Homescreen(void){
  String device;
  String status;
  device = CMD.next();
  status = CMD.next();

  if(device == "bell"){
    if(status == "ON"){
      Firebase.setBool(firebasedata,"/stream/Homescreen/bell",true);
    }
    else {
      Firebase.setBool(firebasedata,"/stream/Homescreen/bell",false);
    }
  }

  if(device == "dryingpole"){
    if(status == "ON"){
      Firebase.setBool(firebasedata,"/stream/Homescreen/dryingpole",true);
    }
    else {
      Firebase.setBool(firebasedata,"/stream/Homescreen/dryingpole",false);
    }
  }

  if(device == "gate"){
    if(status == "ON"){
      Firebase.setBool(firebasedata,"/stream/Homescreen/gate",true);
    }
    else {
      Firebase.setBool(firebasedata,"/stream/Homescreen/gate",false);
    }
  }

  if(device == "smartlight"){
    if(status == "ON"){
      Firebase.setBool(firebasedata,"/stream/Homescreen/smartlight",true);
    }
    else {
      Firebase.setBool(firebasedata,"/stream/Homescreen/smartlight",false);
    }
  }
}

void Kitchen(){
  String device;
  String status;
  device = CMD.next();
  status = CMD.next();
  if(device == "window"){
    if(status == "ON"){
      Firebase.setBool(firebasedata,"/stream/Kitchen/window",true);
    }
    else {
      Firebase.setBool(firebasedata,"/stream/Kitchen/window",false);
    }
  }

  if(device == "fan"){
    if(status == "ON"){
      Firebase.setBool(firebasedata,"/stream/Kitchen/fan",true);
    }
    else {
      Firebase.setBool(firebasedata,"/stream/Kitchen/fan",false);
    }
  }

  

} 

void password(){
  String password_update;
  password_update = CMD.next();
  Firebase.setString(firebasedata, "/password", password_update);
}

//Function set up esp
void initWifi(){

  // WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  // while (WiFi.status() != WL_CONNECTED){
  //   delay(300);
  //   Serial.print(".");
  // }
  
  wifiManager.autoConnect("Connect SmartHome to Wifi","12345678");
}


void initFirebase(){
  if(WiFi.status() == WL_CONNECTED){
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);

    Firebase.beginStream(firebasedataStream,"/stream");
  }
}

void initSystem(){
  Serial.println("Begin init system");
  Firebase.setBool(firebasedata,"/stream/Bathroom/light",false);

  Firebase.setBool(firebasedata,"/stream/Bedroom/light",false);
  Firebase.setBool(firebasedata,"/stream/Bedroom/fan",false);
  
  Firebase.setBool(firebasedata,"/stream/Bedroom/window",false);

  Firebase.setBool(firebasedata,"/stream/Homescreen/gate",false);
  Firebase.setBool(firebasedata,"/stream/Homescreen/dryingpole",false);

  Firebase.setBool(firebasedata,"/stream/Kitchen/fan",false);
  Firebase.setBool(firebasedata,"/stream/Kitchen/light",false);
  
  Firebase.setBool(firebasedata,"/stream/Kitchen/window",false);

  Firebase.setBool(firebasedata,"/stream/Livingroom/garagelight",false);
  Serial.println("Finished init system");
}



void setup() {
  // put your setup code here, to run once:

  //Serial init
  Serial.begin(115200);
  mySerial.begin(115200, SWSERIAL_8N1, D3, D4, false, 256);
  Serial.println("Serial begin");
  
  //Call command
  CMD.addCommand("RAIN" , RAIN);
  CMD.addCommand("DHT11", DHT11);
  CMD.addCommand("PIR"  , PIR);
  CMD.addCommand("FLAME", FLAME);
  CMD.addCommand("GAS"  , GAS);
  CMD.addCommand("HALL",HALL);
  
  CMD.addCommand("Homescreen",Homescreen);
  CMD.addCommand("Kitchen",Kitchen);
  CMD.addCommand("password",password);

  //Wifi init
  initWifi();

  //Firebase init.
  initFirebase();

  //Init System
  initSystem();

  //Update password to STM32
  delay(1000);
  Firebase.getString(firebasedata,"/password");
  String get_password = firebasedata.stringData();
  mySerial.print("password ");
  mySerial.print(get_password);
  mySerial.print(" \n");
  Serial.print("password ");
  Serial.print(get_password);
  Serial.print(" \n");
}

void loop() {  

  if(WiFi.status() != WL_CONNECTED){
    wifiManager.autoConnect("Reconnect SmartHome to Wifi","12345678");
  }

  CMD.readSerial();
  
  if(Firebase.ready()){
    if (!Firebase.readStream(firebasedataStream)){
      Serial.println("------------------------------------");
      Serial.println("Can't read stream data...");
      Serial.println("REASON: " + firebasedataStream.errorReason());
      Serial.println("------------------------------------");
      Serial.println();
    }
    if (firebasedataStream.streamTimeout()){
      Serial.println("Stream timeout, resume streaming...");
      Serial.println();
    }

    if(firebasedataStream.streamAvailable()){
      // Serial.printf("sream path: %s\nevent path: %s\ndata type: %s\nevent type: %s\nvalue: %s\n\n",
      //   firebasedataStream.streamPath().c_str(),
      //   firebasedataStream.dataPath().c_str(),
      //   firebasedataStream.dataType().c_str(),
      //   firebasedataStream.eventType().c_str(),
      //   firebasedataStream.stringData().c_str());

      String stream_path = firebasedataStream.dataPath().c_str();
      bool stream_data = firebasedataStream.boolData();
      
      //Bathroom stream functions
      if (stream_path == "/Bathroom/light"){
        if(stream_data == true){
          mySerial.print("Bathroom/light T \n");
          delay(20);
          mySerial.print("Bathroom/light T \n");
          delay(20);
          Serial.print("Bathroom/light T \n");
          delay(20);
        }else if(firebasedataStream.boolData() == false){
          mySerial.print("Bathroom/light F \n");
          delay(20);
          mySerial.print("Bathroom/light F \n");
          delay(20);
          Serial.print("Bathroom/light F \n");
          delay(20);
        } 
      } 

      //Livingroom stream functions
      if (stream_path == "/Livingroom/garagelight"){
        if(firebasedataStream.boolData() == true){
          mySerial.print("Livingroom/garagelight T \n");
          delay(20);
          mySerial.print("Livingroom/garagelight T \n");
          delay(20);
          Serial.print("Livingroom/garagelight T \n");
          delay(20);
        }else if(firebasedataStream.boolData() == false){
          mySerial.print("Livingroom/garagelight F \n");
          delay(20);
          mySerial.print("Livingroom/garagelight F \n");
          delay(20);
          Serial.print("Livingroom/garagelight F \n");
        }  
      }
      
      //Bedroom stream functions
      if(stream_path == "/Bedroom/fan"){
        if(firebasedataStream.boolData() == true){
          mySerial.print("Bedroom/fan T \n");
          delay(20);
          mySerial.print("Bedroom/fan T \n");
          delay(20);
        }else if(firebasedataStream.boolData() == false){
          mySerial.print("Bedroom/fan F \n");
          delay(20);
          mySerial.print("Bedroom/fan F \n");
          delay(20);
        }  
      }
      if(stream_path == "/Bedroom/light"){
        if(firebasedataStream.boolData() == true){
          mySerial.print("Bedroom/light T \n");
          delay(20);
          mySerial.print("Bedroom/light T \n");
          delay(20);
        }else if(firebasedataStream.boolData() == false){
          mySerial.print("Bedroom/light F \n");
          delay(20);
          mySerial.print("Bedroom/light F \n");
          delay(20);
        }  
      }

      if(stream_path == "/Bedroom/window"){
        if(firebasedataStream.boolData() == true){
          mySerial.print("Bedroom/window T \n");
          delay(20);
          mySerial.print("Bedroom/window T \n");
          delay(20);
        }else if(firebasedataStream.boolData() == false){
          mySerial.print("Bedroom/window F \n");
          delay(20);
          mySerial.print("Bedroom/window F \n");
          delay(20);
        }  
      }

      //Homescreen stream functions
      if(stream_path == "/Homescreen/bell"){
        if(firebasedataStream.boolData() == true){
          mySerial.print("Homescreen/bell T \n");
          delay(20);
          mySerial.print("Homescreen/bell T \n");
          delay(20);
        }else if(firebasedataStream.boolData() == false){
          mySerial.print("Homescreen/bell F \n");
          delay(20);
          mySerial.print("Homescreen/bell F \n");
          delay(20);
        }  
      }

      if(stream_path == "/Homescreen/dryingpole"){
        if(firebasedataStream.boolData() == true){
          mySerial.print("Homescreen/dryingpole T \n");
          delay(20);
          mySerial.print("Homescreen/dryingpole T \n");
          delay(20);
        }else if(firebasedataStream.boolData() == false){
          mySerial.print("Homescreen/dryingpole F \n");
          delay(20);
          mySerial.print("Homescreen/dryingpole F \n");
          delay(20);
        }  
      }

      if(stream_path == "/Homescreen/gate"){
        if(firebasedataStream.boolData() == true){
          mySerial.print("Homescreen/gate T \n");
          delay(20);
          mySerial.print("Homescreen/gate T \n");
          delay(20);
        }else if(firebasedataStream.boolData() == false){
          mySerial.print("Homescreen/gate F \n");
          delay(20);
          mySerial.print("Homescreen/gate F \n");
          delay(20);
        }  
      }

      if(stream_path == "/Homescreen/smartlight"){
        if(firebasedataStream.boolData() == true){
          mySerial.print("Homescreen/smartlight T \n");
          delay(20);
          mySerial.print("Homescreen/smartlight T \n");
          delay(20);
        }else if(firebasedataStream.boolData() == false){
          mySerial.print("Homescreen/smartlight F \n");
          delay(20);
          mySerial.print("Homescreen/smartlight F \n");
          delay(20);
        }  
      }

      //Kitchen stream functions
      if(stream_path == "/Kitchen/fan"){
        if(firebasedataStream.boolData() == true){
          mySerial.print("Kitchen/fan T \n");
          delay(20);
          mySerial.print("Kitchen/fan T \n");
          delay(20);
        }else if(firebasedataStream.boolData() == false){
          mySerial.print("Kitchen/fan F \n");
          delay(20);
          mySerial.print("Kitchen/fan F \n");
          delay(20);
        }  
      }

      if(stream_path == "/Kitchen/light"){
        if(firebasedataStream.boolData() == true){
          mySerial.print("Kitchen/light T \n");
          delay(20);
          mySerial.print("Kitchen/light T \n");
          delay(20);
        }else if(firebasedataStream.boolData() == false){
          mySerial.print("Kitchen/light F \n");
          delay(20);
          mySerial.print("Kitchen/light F \n");
          delay(20);
        }  
      }

      if(stream_path == "/Kitchen/window"){
        if(firebasedataStream.boolData() == true){
          mySerial.print("Kitchen/window T \n");
          delay(20);
          mySerial.print("Kitchen/window T \n");
          delay(20);
        }else if(firebasedataStream.boolData() == false){
          mySerial.print("Kitchen/window F \n");
          delay(20);
          mySerial.print("Kitchen/window F \n");
          delay(20);
        }  
      }
    }
  }
}
    
