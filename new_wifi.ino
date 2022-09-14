#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#define MQ_sensor A0 //Sensor is connected to A0

const char* ssid = "bssm_free";
const char* password = "bssm_free"; 
const char* mqtt_server = "10.150.150.2";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* message, unsigned int length){
  Serial.print("Message arrived on topic:");
  Serial.print(topic);
  Serial.print(".Message:");
  String messageTemp;
  
  for(int i=0;i<length;i++){
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
}

void setup_wifi(){
  delay(10);

  Serial.println();
  Serial.print("Connecting");
  Serial.println(ssid);
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address:");
  Serial.println(WiFi.localIP());
}


void reconnect(){
  while(!client.connected()){
    Serial.print("Attempting MQTT connection");
    if(client.connect("ESP8266Client")){
      Serial.println("connected");
    }else{
      Serial.print("failed,rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server,1883);
  client.setCallback(callback);
}

void loop() {
  // put your main code here, to run repeatedly:
  float msg;

  msg = analogRead(MQ_sensor)-125;
  
  if(!client.connected()){
    reconnect();
  }
  if(!client.loop())
    client.connect("ESP8266Client");

  static char measure[7];
  dtostrf(msg,6,2,measure);
  
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish("Sensor", measure);
  delay(1500);

}
