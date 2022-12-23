#include <WiFi.h>
#include <PubSubClient.h>

/* wifi config */
const char *ssid = "MI9";
const char *password = "12345678";

/* mqtt config */
const char *clientId = "oeibax";
const char *username = "admin";
const char *pwd = "123456";
const char *mqttServer = "101.33.214.83";

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++)
    {
        Serial.print((char)payload[i]);
    }
    Serial.println();
}

void reconnect() {
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect(clientId, username, pwd)) {
            Serial.println("connected");

            client.setCallback(callback);
            client.subscribe("esp32/test");
            client.publish("esp32/test", "esp32 device working now...");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}


void setup() {
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqttServer, 1883);
}

void loop() {
    if (!client.connected())
    {
        reconnect();
    }
    client.loop();
}
