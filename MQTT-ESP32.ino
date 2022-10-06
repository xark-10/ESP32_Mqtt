/*
 *  This sketch sends a message to a TCP server
 *
 */

#include "WiFi.h"
#include <PubSubClient.h>



const char * ssid = " E408A1";
const char * password = "73427342";

// const char *mqtt_host[] = "cd1e0119.us-east-1.emqx.cloud" ;
// const char *clientid[] = "ESP32" ;
// const char *username[] = "ESP32" ;
// const char *passwd[] = "esp32" ;
// const char topicname[] = "ledlclient" ;
// const char topicsub[] = "ledlserver" ;
// const int *mqtt_port = 8084 ;

// MQTT Broker
const char *mqtt_broker = "cd1e0119.us-east-1.emqx.cloud";
const char *topic = "esp32/test";
const char topicname[] = "ledlclient" ;
const char topicsub[] = "ledlserver" ;
const char *mqtt_username = "ESP32";
const char *mqtt_password = "esp32";
const int mqtt_port = 15253;  

WiFiClient espClient;
PubSubClient client(espClient);
void setup()
{
    Serial.begin(115200);
    delay(10);


    // set led pin
  //digitalWrite (LED_BUILTIN ,HIGH);
  pinMode (LED_BUILTIN , OUTPUT); // Initiaoutputlize the LED_BUILTIN pin as an

    // We start by connecting to a WiFi network
    WiFi.mode(WIFI_STA);
    WiFi.begin("E408A1", "73427342");

    Serial.println();
    Serial.println();
    Serial.print("Waiting for WiFi... ");

    while(WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    delay(500);


client.setServer(mqtt_broker, mqtt_port);
client.setCallback(callback);
while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
        Serial.println("Public emqx mqtt broker connected");
    } else {
        Serial.print("failed with state ");
        Serial.print(client.state());
        delay(2000);
    }
}
    

    
client.subscribe(topic);
// client.publish(topic, "power"); 

}


/*void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
    Serial.println("-----------------------");
}*/

void respond(){
  //client.subscribe(topic);
  client.publish(topic, "pong");
}

void callback( char *topic , byte *payload , unsigned int length ) {
    Serial.println ( " Message arrived in topic : " ) ;
    Serial.println (topic);
    Serial.print ( " Message : " ) ;
    String message ;
    char *message2 ;
    for ( int i = 0 ; i < length ; i ++ ) {
       message = message + (char)payload[i] ; // convert * byte to string
   }
    Serial.print (message) ;
    if (message == "ping" ) {
      Serial.println (".......................");
      respond();
   }
    else if ( message == "power" ) {
      digitalWrite(LED_BUILTIN , !digitalRead(LED_BUILTIN));
     // the led tells the client the state of the led .
     // low is led on
     if ( digitalRead (LED_BUILTIN)== HIGH) {
        message2 = " led1 : 1 " ;
     }
      else {
        message2 = "led1:0" ;
      }
    //  client.publish ( topic , message2 ) ;
    }
  } // LED on


void loop()
{
  
 client.loop();
 delay(1000);
}
