#include <DHT.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define DHTPIN 4
#define DHTTYPE DHT11

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

//Wifi credential
const char *ssid="Galaxy A710316";
const char *password = "jwsg3561";

#define TOKEN "S4D5wU6MqToZCtwVzQv9"
const char *mqtt_server = "demo.thingsboard.io";

WiFiClient wifiClient;
PubSubClient client(wifiClient);
int status = WL_IDLE_STATUS;


void setup() {
  Serial.begin(115200);
  Serial.println(F("DHTxx test!"));
  dht.begin();
  Serial.println("connect to : ");
  Serial.println(ssid);
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("... ");
  }
  Serial.println("\n");
  Serial.println("IP Address : ");
  Serial.println(WiFi.localIP());
  Serial.println("\n");
  Serial.println("connect to : ");
  Serial.println(ssid);
  client.setServer(mqtt_server,1883);
}

void loop() {
 
  // Wait a few seconds between measurements.
  if(!client.connected()) reconnect();
 

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  f = f + random(30);
  t = t + random(30);
  h = h + random(30);
  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
 
  char attributes[1000];
 
  client.publish("v1/devices/me/telemetry",attributes);
  client.publish("v1/devices/me/attributes",attributes);

  delay(5000);

 
 
}

void reconnect()
{
  while(!client.connected())
  {
    status = WiFi.status();
    if(status != WL_CONNECTED)
    {
      WiFi.begin(ssid,password);
      while(WiFi.status() != WL_CONNECTED)
      {
        delay(500);
        Serial.println(". ");
      }
      Serial.println("Connected to AP");
    }
    Serial.println("Connecting to thingsboard node ..."    );
    if(client.connect("188e7e20-8c3d-11ef-abca-65f6fb556bf7",TOKEN,""))
    {
      Serial.println(("Done"));
    }
    else
    {
      Serial.println("FAILED, retrying in 5 seconds");
      delay(5000);
    }
  }
}

