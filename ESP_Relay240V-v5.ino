

/////////////////////
/////

//


////////////////////

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// wifi and ESP and MQTT


// WiFi parameters
const char* ssid = "Xiaomi_AS";
const char* password = "AS172014";
const char* mqtt_server = "192.168.31.232";  //Enter the MQTT server details
const char* mqtt_username = "pi";
const char* mqtt_password = "muemc761";
// The client id identifies the ESP8266 device. Think of it a bit like a hostname (Or just a name, like Greg).
const char* clientID = "ESP01";
const char* topic="RelayV4";
#define RELAY_PIN 0//


// crée un instance du server
WiFiClient espClient;
PubSubClient client(espClient);

void setup(void)
{  pinMode(RELAY_PIN,OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);

  digitalWrite(RELAY_PIN,LOW);
  Wificonnect();
  blink_led(5);
  client.setServer(mqtt_server, 1883);       //Port number through which the MQTT server is sending & receiving data
  client.setCallback(callback);
  delay(300);
}

void Wificonnect()
{
  delay(10);
  WiFi.begin(ssid, password);                 //Connect to WiFi Network
  while (WiFi.status() != WL_CONNECTED) {     //Trying to connect to the Network
    delay(500);
  }
  //blink_led();
}

void MQTTreconnect()
{
while (!client.connected())
  {
    if (client.connect("espClient",mqtt_username,mqtt_password))
    {
      client.subscribe(topic);            //Subscribes to topic
    }
    else
    {
      delay(5000);
    }
  }
}

void loop() {
  if(WiFi.status() != WL_CONNECTED)
    {    Wificonnect(); }
  if (!client.connected())
    {    MQTTreconnect();  }

  client.loop();
  delay(10);
}


void callback(char* topic, byte* payload, unsigned int length)
{

  String st = topic;
  int relay_status =0;
  if(payload[0]=='0') relay_status=0;
  else if(payload[0]=='1') relay_status=1;
  //Serial.println(LED_status);

  if(st.equalsIgnoreCase(topic))
  { blink_led(3);
    if(relay_status==0)  digitalWrite(RELAY_PIN, LOW);
     if(relay_status==1)  digitalWrite(RELAY_PIN, HIGH);
  }
  //blink_led();
}

void blink_led(int nb){
 int n;
for(n=0;n<nb;n++){
  digitalWrite(LED_BUILTIN, LOW);    // Turn the LED on (Note that LOW is the voltage level
                                    // but actually the LED is on; this is because
                                    // it is acive low on the ESP-01)
  delay(100);                      // Wait for a second
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(100);
  }
}
