//dependencies
#include<WiFi.h>
#include"DHT.h"
#include <Adafruit_MPU6050.h>
#include <Wire.h>


//connecting to wifi..
const char* ssid = "realme_6";
const char* password = "11111111";
 
//host and port of the ip address where data is to be sent 
const uint16_t port = 4002;
const char * host = "143.110.182.214";
 
//starting adafruit library for mpu6050
Adafruit_MPU6050 mpu;

//definings pins for data extraction
#define DHTPIN 4
#define vib_pin 15
#define DHTTYPE DHT11  

//starting dht library
DHT dht(DHTPIN,DHTTYPE);



void setup() {
  WiFi.begin(ssid,password);

   while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(100);
   }
   Serial.print("CONNECTED TO ");
   Serial.println(WiFi.localIP());

   
  pinMode(vib_pin,INPUT);//pin to read vibration sensor data
  
  Serial.begin(115200);//starting the serial monitor

  mpu.begin();//starting mpu for  reading
  
  dht.begin();//starting dht for reading
}

void loop() {

  //defining WiFi client
   WiFiClient client;
   client.connect(host, port);
 
//reading dht sensor temperature data
  float t = dht.readTemperature();
  Serial.print(" Temperature: ");
  Serial.print(t);
  Serial.println("°C ");

 //reading vibration sensor data 
  long vibration = vib_init();
  Serial.println(vibration);

  //mpu sensor
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  
//Accelerometer reading
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  
//gyroscope reading
  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  String DATA = String(t)+" "+String(vibration)+" "+String(a.acceleration.x)+" "+String(a.acceleration.y)+" "+String(a.acceleration.z)+" "+String(g.gyro.x)+" "+String(g.gyro.y)+" "+String(g.gyro.z);

  //sending data to cloud
  client.print(DATA);

   client.stop();
   
  Serial.println("____________________________________________________________________________");
  //printing d\sent data into serial monitor 
  Serial.println(DATA);
  Serial.println("____________________________________________________________________________");

  
  delay(1000);
}

long vib_init(){
  long vibration = pulseIn(vib_pin,HIGH); 
  return vibration;
}
