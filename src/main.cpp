#include <main.h>

const char* ssid = "NNguyen";
const char* password = "20903137";
const char* mqtt_server = "makerzone.net";

const char* profile;

int default_humidirt = 10;

unsigned long lastMsg = 0;
unsigned long last_flag = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
int flag = 0;
bool check = 0;

WiFiClient espClient;
PubSubClient client(espClient);
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if (topic[0] == 'h') {
    default_humidirt = atoi((char*) payload);
    Serial.print("Changing humi to: ");
    Serial.println(default_humidirt);
  }
  switch (payload[0]) {
    case 'l':
    Serial.println("low profile detected");
    flag = 1;
    break;

    case 'm':
    Serial.println("medium profile detected");
    flag = 2;
    break;

    case 'h':
    Serial.println("high profile detected");
    flag = 3;
    break;

    case 'a':
    Serial.println("auto profile detected");
    default_humidirt = 10;
    break;
  }
  
}
void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  DHT.begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), "popos", "mqttserver")) {
      Serial.println("connected");
      client.subscribe("profile");
      client.subscribe("humi_dirt");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long flag_now = millis();
  switch (flag) {
    case 1:
    if (check == 0) {
      last_flag = flag_now;
      check = 1;
      Serial.println("Start !");
      digitalWrite(2, 1);
      digitalWrite(4, 0);
    }

    if (flag_now - last_flag > 1000) {
      Serial.println("Stop !");
      last_flag = flag_now;
      flag = 0;
      check = 0;
      digitalWrite(2, 0);
      digitalWrite(4, 0);
    }
    break;

    case 2:
    if (check == 0) {
      last_flag = flag_now;
      check = 1;
      Serial.println("Start !");
      digitalWrite(2, 1);
      digitalWrite(4, 0);
    }

    if (flag_now - last_flag > 2000) {
      Serial.println("Stop !");
      last_flag = flag_now;
      flag = 0;
      check = 0;
      digitalWrite(2, 0);
      digitalWrite(4, 0);
    }
    break;

    case 3:
    if (check == 0) {
      last_flag = flag_now;
      check = 1;
      Serial.println("Start !");
      digitalWrite(2, 1);
      digitalWrite(4, 0);
    }

    if (flag_now - last_flag > 4000) {
      Serial.println("Stop !");
      last_flag = flag_now;
      flag = 0;
      check = 0;
      digitalWrite(2, 0);
      digitalWrite(4, 0);
    }
    break;
  }
  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    float temp, humi, humi_dirt;
    ReadSensors(temp, humi, humi_dirt);
    if (humi_dirt < default_humidirt && flag == 0) {
      digitalWrite(2, 1);
      digitalWrite(4, 0);
    } 
    if (humi_dirt >= default_humidirt && flag == 0) {
      digitalWrite(2, 0);
      digitalWrite(4, 0);
    }
    snprintf(msg, MSG_BUFFER_SIZE, "%0.2f %0.2f %0.2f", temp, humi, humi_dirt);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("historicaldata", msg);
  }
  
}