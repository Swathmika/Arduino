/********************************
 * Libraries included
 *******************************/

//#include <ESP8266WiFi.h>
#include <DHT.h>

#include <WiFi.h>
#include <PubSubClient.h>
#define DHTPIN 5
DHT dht(DHTPIN, DHT11);
xQueueHandle Q1 = 0;
xQueueHandle Q2 = 0;  

/********************************
 * Constants and objects
 *******************************/
char const * SSID_NAME = "Swathmika"; // Put here your SSID name
char const * SSID_PASS = "Swathi@123"; // Put here your password

char* TOKEN = "BBFF-wvlzDNiEmNxtuDK38SBcHh2Y646snE"; // Put here your TOKEN

char* DEVICE_LABEL = "esp32"; // Your Device label

/* Put here your variable's labels*/
char const * VARIABLE_LABEL_1 = "temperature";
char const * VARIABLE_LABEL_2 = "humidity";

/* HTTP Settings */
char const * HTTPSERVER = "things.ubidots.com";
const int HTTPPORT = 80;
char const * USER_AGENT = "ESP32";
char const * VERSION = "1.0";

WiFiClient clientUbi;

/********************************
 * Auxiliar Functions
 *******************************/

void SendToUbidots(char* payload) {

  int contentLength = strlen(payload);

  /* Connecting the client */
  clientUbi.connect(HTTPSERVER, HTTPPORT);

  if (clientUbi.connected()) {
    /* Builds the request POST - Please reference this link to know all the request's structures https://ubidots.com/docs/api/ */
    clientUbi.print(F("POST /api/v1.6/devices/"));
    clientUbi.print(DEVICE_LABEL);
    clientUbi.print(F(" HTTP/1.1\r\n"));
    clientUbi.print(F("Host: "));
    clientUbi.print(HTTPSERVER);
    clientUbi.print(F("\r\n"));
    clientUbi.print(F("User-Agent: "));
    clientUbi.print(USER_AGENT);
    clientUbi.print(F("/"));
    clientUbi.print(VERSION);
    clientUbi.print(F("\r\n"));
    clientUbi.print(F("X-Auth-Token: "));
    clientUbi.print(TOKEN);
    clientUbi.print(F("\r\n"));
    clientUbi.print(F("Connection: close\r\n"));
    clientUbi.print(F("Content-Type: application/json\r\n"));
    clientUbi.print(F("Content-Length: "));
    clientUbi.print(contentLength);
    clientUbi.print(F("\r\n\r\n"));
    clientUbi.print(payload);
    clientUbi.print(F("\r\n"));

    Serial.println(F("Making request to Ubidots:\n"));
    Serial.print("POST /api/v1.6/devices/");
    Serial.print(DEVICE_LABEL);
    Serial.print(" HTTP/1.1\r\n");
    Serial.print("Host: ");
    Serial.print(HTTPSERVER);
    Serial.print("\r\n");
    Serial.print("User-Agent: ");
    Serial.print(USER_AGENT);
    Serial.print("/");
    Serial.print(VERSION);
    Serial.print("\r\n");
    Serial.print("X-Auth-Token: ");
    Serial.print(TOKEN);
    Serial.print("\r\n");
    Serial.print("Connection: close\r\n");
    Serial.print("Content-Type: application/json\r\n");
    Serial.print("Content-Length: ");
    Serial.print(contentLength);
    Serial.print("\r\n\r\n");
    Serial.print(payload);
    Serial.print("\r\n");
  } else {
    Serial.println("Connection Failed ubidots - Try Again");
  }

  /* Reach timeout when the server is unavailable */
  int timeout = 0;
  while (!clientUbi.available() && timeout < 5000) {
    timeout++;
    delay(1);
    if (timeout >= 5000) {
      Serial.println(F("Error, max timeout reached"));
      break;
    }
  }

  /* Reads the response from the server */
  Serial.println(F("\nUbidots' Server response:\n"));
  while (clientUbi.available()) {
    char c = clientUbi.read();
    Serial.print(c); // Uncomment this line to visualize the response on the Serial Monitor
  }

  /* Disconnecting the client */
  clientUbi.stop();
}

/********************************
 * Main Functions
 *******************************/

void setup() {
  Serial.begin(115200);
  /* Connects to AP */
  WiFi.begin(SSID_NAME, SSID_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    }

  WiFi.setAutoReconnect(true);
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());

  if (clientUbi.connect(HTTPSERVER, HTTPPORT)) {
    Serial.println("connected to Ubidots cloud");
  } else {
    Serial.println("could not connect to Ubidots cloud");
  }
dht.begin();
   Q1 = xQueueCreate(3,sizeof(float));
   Q2 = xQueueCreate(3,sizeof(float));
   if(Q1 == NULL || Q2 == NULL){
   Serial.println("Error creating the queue");
  }
  xTaskCreate(taskOne,"TaskOne", 10000, NULL, 1,NULL);
  xTaskCreate(taskTwo,"TaskTwo",10000,NULL, 1, NULL);
  xTaskCreate(taskThree,"TaskThree",10000,NULL, 1, NULL);
}

//void loop() {
//
//  // Space to store values to send
//  char payload[200];
//  char str_val_1[30];
//  char str_val_2[30];
//
//  /*---- Simulates the values of the sensors -----*/
//  float sensor_value_1 = random(0, 1000)*1.0;
//  float sensor_value_2 = random(0, 1000)*1.0;
//
//  /*---- Transforms the values of the sensors to char type -----*/
//
//  /* 4 is mininum width, 2 is precision; float value is copied onto str_val*/
//  dtostrf(sensor_value_1, 4, 2, str_val_1);
//  dtostrf(sensor_value_2, 4, 2, str_val_2);
//
//  // Important: Avoid to send a very long char as it is very memory space costly, send small char arrays
//  sprintf(payload, "{\"");
//  sprintf(payload, "%s%s\":%s", payload, VARIABLE_LABEL_1, str_val_1);
//  sprintf(payload, "%s,\"%s\":%s", payload, VARIABLE_LABEL_2, str_val_2);
//  sprintf(payload, "%s}", payload);
//
//  /* Calls the Ubidots Function POST */
//  SendToUbidots(payload);
//
//  delay(5000);
//
//}

void loop() {
//delay(1000);
}
void taskOne(void *p){
     
    for(;;){
      float humidity = dht.readHumidity();
      float temperature = dht.readTemperature();
       
         xQueueSend(Q1, &humidity, 0);
        xQueueSend(Q1, &temperature, 0);
       vTaskDelay(2000);
     }
}

void taskTwo(void *p){
  float received_temperature;
  float received_humidity;
 
    for(;;){
      xQueueReceive(Q1,&received_temperature,portMAX_DELAY );
      xQueueReceive(Q1,&received_humidity,portMAX_DELAY );
      printf("Received in task 2 value: Temperature: %f \n", received_temperature);
      printf("Received in task 2 value: Humidity: %f \n", received_humidity);
      xQueueSend(Q2, &received_temperature, 0);
      xQueueSend(Q2, &received_humidity, 0);
      // vTaskDelay(1000);
     }
}

void taskThree(void *p){
  float temperature;
  float humidity;  
    for(;;){
       xQueueReceive(Q2,&temperature,portMAX_DELAY );
       printf(" Temp %i\n", temperature);
       xQueueReceive(Q2,&humidity,portMAX_DELAY );
       printf(" Temp %i\n", humidity);
       //vTaskDelay(1000);      
  char payload[200];
  char str_val_1[30];
  char str_val_2[30];

   if (isnan(temperature) || isnan(humidity)) {
   Serial.println("Failed to read from DHT sensor");

}
  dtostrf(temperature, 4, 2, str_val_1);
  dtostrf(humidity, 4, 2, str_val_2);

  sprintf(payload, "{\"");
  sprintf(payload, "%s%s\":%s", payload, VARIABLE_LABEL_1, str_val_1);
  sprintf(payload, "%s,\"%s\":%s", payload, VARIABLE_LABEL_2, str_val_2);
  sprintf(payload, "%s}", payload);
  /* Calls the Ubidots Function POST */
  SendToUbidots(payload);

     }
}
