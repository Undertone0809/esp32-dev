#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>

/* timer */
hw_timer_t *tim1 = NULL;
int tim1_IRQ_count = 0;
int enable_tim1_work = 0;

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

/* mpu6050 config */
const int MPU_ADDR = 0x68;
int16_t accelerometer_x, accelerometer_y, accelerometer_z; // variables for accelerometer raw data
int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data
int16_t temperature; // variables for temperature data
char tmp_str[7]; // temporary variable used in convert function


void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("[mqtt] Message arrived [");
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
        Serial.print("[mqtt] Attempting MQTT connection...");
        if (client.connect(clientId, username, pwd)) {
            Serial.println("connected");

            client.setCallback(callback);
            client.subscribe("/esp32");
            client.publish("/esp32", "esp32 device working now...");
        } else {
            Serial.print("[mqtt] failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("[wifi] Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
    sprintf(tmp_str, "%6d", i);
    return tmp_str;
}

void mpuInit() {
    Wire.begin();
    Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
    Wire.write(0x6B); // PWR_MGMT_1 register
    Wire.write(0); // set to zero (wakes up the MPU-6050)
    Wire.endTransmission(true);
}

void isAlarm() {
    if((temperature/340.00+36.53) > 23) {
        double temp = temperature / 340.00 + 36.53;
        char msg[50];
        sprintf(msg, "温度报警: 当前温度%.2f，温度过高", temp);
        client.publish("/esp32", msg);
    }
}

void readMpu6050() {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
    Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
    Wire.requestFrom(MPU_ADDR, 7*2, true); // request a total of 7*2=14 registers
    
    // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
    accelerometer_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
    accelerometer_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
    accelerometer_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
    temperature = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
    gyro_x = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
    gyro_y = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
    gyro_z = Wire.read()<<8 | Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
    
    isAlarm();

    // print out data
    Serial.print("[mpu6050] aX = "); Serial.print(convert_int16_to_str(accelerometer_x));
    Serial.print(" | aY = "); Serial.print(convert_int16_to_str(accelerometer_y));
    Serial.print(" | aZ = "); Serial.print(convert_int16_to_str(accelerometer_z));
    // the following equation was taken from the documentation [MPU-6000/MPU-6050 Register Map and Description, p.30]
    Serial.print(" | tmp = "); Serial.print(temperature/340.00+36.53);
    Serial.print(" | gX = "); Serial.print(convert_int16_to_str(gyro_x));
    Serial.print(" | gY = "); Serial.print(convert_int16_to_str(gyro_y));
    Serial.print(" | gZ = "); Serial.print(convert_int16_to_str(gyro_z));
    Serial.println();
    
}

void tim1Interrupt() {
    tim1_IRQ_count ++;
    if (tim1_IRQ_count > 10) {
        enable_tim1_work = 1;
        tim1_IRQ_count = 0;
    }
    
}

void setup() {
    Serial.begin(115200);
    Serial.println("[info] start work");
    
    mpuInit();
    setup_wifi();
    client.setServer(mqttServer, 1883);

    tim1 = timerBegin(0, 80, true);
    timerAttachInterrupt(tim1, tim1Interrupt, true);
    timerAlarmWrite(tim1, 100000ul, true);
    timerAlarmEnable(tim1);
}

void loop() {
    if (enable_tim1_work) {
        readMpu6050();
        enable_tim1_work = 0;
    }
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}