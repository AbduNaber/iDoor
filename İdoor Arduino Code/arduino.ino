#include <MFRC522.h>              //MFRC522 library
#include <SPI.h>            //NFC library
#include <RFID.h>           //NFC library
#include <Servo.h>          //Servo library
#include <SoftwareSerial.h> // pin communacation library
#include "id.h"             // stores NFC card id 


void datasent(int value,int arr[4]);   
void printscreen();
void espbegin(String wifiName,String wifiPassword);
void rfidRead(int *a);
void distanceRead(int *a,int *b);

#define trigPin 4           // Distance Sensor trigger port
#define echoPin 2           // Distance Sensor Echo port

MFRC522 rfid(10, 9);         //NFC port 

int sure, mesafe;           // variable for calculation of distance

Servo myservo;              //Servo 

//definitions required not to post twice
int aa=0;                    
int bb=0;
int *doorStatus = &aa;
int *temp = &bb ;
int kapi = 0;               

String wifiName = "XXXXXXX";                 //Wifi name
String wifiPassword = "XXXXXXXX";           // Wifi password

int rx = 7;                                               // ESP8266 RX pin
int tx = 6;                                               // ESP8266 TX pin
String ip = "184.106.153.149";                                //Thingspeak ip adress
SoftwareSerial esp(rx,tx);                             //Serial communacation pin

void setup() { // starting module

  Serial.begin(9600);
  pinMode(3, OUTPUT); //  red lamb pin
  pinMode(8, OUTPUT); //  green lamb pin

  SPI.begin();            
  rfid.PCD_Init(); // RC522 module
  pinMode(trigPin, OUTPUT);     //distance sensor trigger pin
  pinMode(echoPin, INPUT);      //distance sensor echo pin
  myservo.attach(5);            //Servo pin
  espbegin(wifiName,wifiPassword); // starts ESP8266 wifi shield (for more info look at espbegin function)
  Serial.println("started");
  myservo.write(0); // sets servo to o angle
  delay(500);
}

void loop() {
  rfidRead(doorStatus); //reads RFID cards
  distanceRead(doorStatus,temp); // calculate distance of door
}

void espbegin(String wifiName,String wifiPassword){
  esp.begin(115200);                                          // initiates serial communication with ESP8266.
  esp.println("AT");                                          // controls the module with the AT command
  Serial.println("AT is sent");
  while (!esp.find("OK")) {                                   // Waits until module is ready
    esp.println("AT");
    Serial.println("ESP8266 was not found.");
  } 
  Serial.println("OK");
  esp.println("AT+CWMODE=1");                                 //Sets the ESP8266 module as a client.
  while (!esp.find("OK")) {                                   //Waiting until the setting is done
    Serial.println("SETTINGs are being made");
    esp.println("AT+CWMODE=1");
  }
  
  Serial.println("esp was set as CLIENT");
  Serial.println("Connecting wifi network...");
  esp.println("AT+CWJAP=\"" + wifiName + "\",\"" + wifiPassword + "\""); //Connecting to the WIFI network
  while (!esp.find("OK"));                                    //Waiting until it connects to the network.
  Serial.println("connected to WIFI"); 
  delay(1000);
}

void distanceRead(int *a,int *b){ // Reads distance with Ultrasonic ranging module HC - SR04

  digitalWrite(trigPin, LOW);        
  delay(10);
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);
  sure = pulseIn(echoPin, HIGH);
  mesafe = (sure / 2) * 0.0343;
  Serial.print(mesafe);               
  Serial.println(" cm far away");
  delay(500);

  if (mesafe <= 18 ) {
    digitalWrite(8,HIGH);
    if((*a == *b)){
      Serial.println("Door is open");
    datasent(1,anon);
    *a=1;
    }                   
  }
  
  if(mesafe >18) {
    digitalWrite(8,LOW);
    if(*a != *b){
       Serial.println("door is close");
    kapi=0;
    Serial.println(*a,*b);
    datasent(0,anon);
    *a=0;
    }
  }
}

void rfidRead(int *a){ // scans NFC card with RC522 RFID NFC Module
  if(rfid.PICC_IsNewCardPresent()) {
    if (rfid.PICC_ReadCardSerial()) {
    if (rfid.uid.uidByte[0] == ID[0] && rfid.uid.uidByte[1] == ID[1] && rfid.uid.uidByte[2] == ID[2] && rfid.uid.uidByte[3] == ID[3]) {
    Serial.println("Door is opened");
    printscreen();
    myservo.write(180); 
    digitalWrite(8,HIGH);
    delay(5000);
    myservo.write(0); 
    digitalWrite(8,LOW);
    datasent(0,ID);
    
    *a=0;

    Serial.println("closed information is send to server");
  }
    else  if (rfid.uid.uidByte[0] == ID3[0] && rfid.uid.uidByte[1] == ID3[1] && rfid.uid.uidByte[2] == ID3[2] && rfid.uid.uidByte[3] == ID3[3]) {
    Serial.println("door was opened");
    printscreen();
    myservo.write(180); 
    digitalWrite(8,HIGH);
    delay(5000);
    myservo.write(0); 
    digitalWrite(8,LOW);
    datasent(0,ID3);
    
    *a=0;

    Serial.println("servera kapalı bilgisi gönderildi");
  }
  else  if (rfid.uid.uidByte[0] == ID4[0] && rfid.uid.uidByte[1] == ID4[1] && rfid.uid.uidByte[2] == ID4[2] && rfid.uid.uidByte[3] == ID4[3]) {
    Serial.println("Kapi acildi");
    printscreen();
    myservo.write(180); /
    digitalWrite(8,HIGH);
    delay(5000);
    myservo.write(0); 
    digitalWrite(8,LOW);
    datasent(0,ID4);
    
    *a=0;

    Serial.println("closed information was sent to server");
  }
  else  if (rfid.uid.uidByte[0] == ID5[0] && rfid.uid.uidByte[1] == ID5[1] && rfid.uid.uidByte[2] == ID5[2] && rfid.uid.uidByte[3] == ID5[3]) {
    Serial.println("Kapi acildi");
    printscreen();
    myservo.write(180); 
    digitalWrite(8,HIGH);
    delay(5000);
    myservo.write(0); 
    digitalWrite(8,LOW);
    datasent(0,ID5);
    
    *a=0;

    Serial.println("closed information was sent to server");
  }
  else if (rfid.uid.uidByte[0] == ID2[0] && rfid.uid.uidByte[1] == ID2[1] && rfid.uid.uidByte[2] == ID2[2] && rfid.uid.uidByte[3] == ID2[3]){
    Serial.println("Kapi acildi");
    printscreen();
    myservo.write(180);    
    *a=1;
    digitalWrite(8,HIGH);
    datasent(1,ID2);
    Serial.println("opened information was sent to server");
    
  }
   else { //this block is executed when somebody trie unauthorized access.
    Serial.println("INVALID KART PLEASE CONTACT WITH YOUR AUTHOTUORY");
    printscreen();
    digitalWrite(3,LOW);
    digitalWrite(3,HIGH);
    delay(300);
    digitalWrite(3,LOW);
    
  }
  }
  }
  rfid.PICC_HaltA();
}

void datasent(int value,int arr[4]){
  // array to str for sending data to server
  char id[12];
  int n = 0;
  for (int i = 0; i < 4; i++) {
      n += sprintf(&id[n], "%d", arr[i]);
    }
   // 
   
    
  Serial.println("data is sending");
 
  esp.println("AT+CIPSTART=\"TCP\",\"" + ip + "\",80");       
  if (esp.find("Error")) {                                    
    Serial.println("AT+CIPSTART Error");
  }
    String data = "GET https://api.thingspeak.com/update?api_key=${YOUR_API_KEY}"; 
    data += "&field1=";                       
    data += String(value);                    
    data += "&field2=";                        
    data += String(id);                     
    data += "\r\n";
    Serial.println(data) ;
    esp.print("AT+CIPSEND=");                                   
    esp.println(data.length() + 2);
    delay(1000);
    esp.print(data);                                          
    Serial.println("Data is sent.");
    delay(1000);
    Serial.println("CONNECTION IS CLOSED");
    esp.println("AT+CIPCLOSE");

}

void printscreen() { // prints scanned card id
  Serial.print("card ID Number: ");
  for (int sayac = 0; sayac < 4; sayac++) {
    Serial.print(rfid.uid.uidByte[sayac]);
    Serial.print(" ");
  }
  Serial.println("");
}

