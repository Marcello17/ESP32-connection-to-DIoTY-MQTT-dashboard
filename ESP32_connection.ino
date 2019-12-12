

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>


const char* ssid = "i4 Group";
const char* pass = "iami4group123!"; /* double check the pasword */
const char* brokerUser = "marcello.jardim54@gmail.com";
const char* brokerPass = "84b955b3";
const char* broker = "mqtt.dioty.co";
const char* outTopic = "/marcello.jardim54@gmail.com/out"; /* double check */
const char* inTopic = "/marcello.jardim54@gmail.com/in";
boolean boot = true;


WiFiClient espClient;
PubSubClient client(espClient);
long currentTime, lastTime;
int count = 0;
char messages[50];

const int ledPin = 2;

boolean setupWifi() /* check if it needs to have a capital F */
{
  int retries = 0;
  delay(100);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while(WiFi.status() != WL_CONNECTED && retries <10)
  {
    delay(100);
    Serial.print("-");
    retries++;
    
  }
  Serial.print("\nConnected to");
  Serial.println(ssid);
  Serial.println(WiFi.localIP());
  return true;
  
}

void reconnect()
{
  while(!client.connected())
  {
    Serial.print("\nConnecting to");
    Serial.println(broker);
    if(client.connect("koikoikoi", brokerUser, brokerPass))
    {
      Serial.print("\nConnected to");
      Serial.println(broker);
      client.subscribe(inTopic);
    }
    else
    {
      Serial.println("\nTrying to connect again");
      delay(5000);
    }
    
    
  }
}


void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Received messages; ");
  Serial.println(topic);
  for(int i=0; i<length; i++)
  {
    Serial.print((char) payload[i]);
  }
  Serial.println();
}

void setup() 
{
  // put your setup code here, to run once:
Serial.begin(115200);



 pinMode(ledPin, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
if (boot == true)
{
  if (setupWifi())
  {
     client.setServer(broker, 1883);
     client.setCallback(callback);
  }
  
  boot = false;
}
if (!client.connected())
{
  reconnect();
}
client.loop();
digitalWrite(ledPin, HIGH); // turn the LED on (HIGH is the voltage level)
delay(1000);                // wait for a second
digitalWrite(ledPin, LOW);
delay(1000);

currentTime = millis();
if(currentTime - lastTime > 2000)
{
  count++;
  snprintf(messages, 75, "%ld", count);
  Serial.print("Sending messages: ");
  Serial.println(messages);
  client.publish(outTopic, messages);
  lastTime = millis();
}


}
