#include <WiFi.h>
#include <WiFiClient.h>
#include <Arduino.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

#include <time.h> // Add this line

const char* ssid = "";
const char* password = "";
const char* mysql_server = "localhost";  // MySQL server IP address
const char* mysql_user = "root";
const char* mysql_password = "";
const char* mysql_database = "cs423";

IPAddress serveraddr(192, 168, 1, 2);  // MySQL server IP address

WiFiClient client; // Declare the client variable

MySQL_Connection conn((Client *)&client);

// Define the GPIO pin connected to the PIR motion sensor.
const int PIR_PIN = 2;

// Define the GPIO pin connected to the LED.
const int LED_PIN = 4;

void setup() {
    Serial.begin(9600);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    // Set the PIR motion sensor pin as an input.
    pinMode(PIR_PIN, INPUT);

    // Set the LED pin as an output.
    pinMode(LED_PIN, OUTPUT);

    createTable();

    // get time from os
    time_t currentTime = time(NULL); // Add this line
    String time = String(currentTime); // Add this line
    insertData(time);
}

void loop() {
    // Read the value of the PIR motion sensor pin.
    int motion = digitalRead(PIR_PIN);

    // If motion is detected, turn on the LED.
    if (motion == HIGH) {
        digitalWrite(LED_PIN, HIGH);
    }
    // If there is no motion, turn off the LED.
    else {
        digitalWrite(LED_PIN, LOW);
    }

    sleep(1000);
}

void createTable() {
    Serial.println("Creating table...");
    if (conn.connect(mysql_server, mysql_user, mysql_password)) {
        Serial.println("Connected to MySQL server");
        delay(1000);
        MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
        cur_mem->execute("CREATE TABLE IF NOT EXISTS cs423 (time VARCHAR(50))");
        delete cur_mem;
        conn.close();
    } else {
        Serial.println("Connection failed.");
        conn.close();
    }
}

void insertData(String time) {
    Serial.println("Inserting data...");
    if (conn.connect(mysql_server, mysql_user, mysql_password)) {
        Serial.println("Connected to MySQL server");
        delay(1000);
        MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
        cur_mem->execute("INSERT INTO cs423 (time) VALUES ('" + time + "')");
        delete cur_mem;
        conn.close();
    } else {
        Serial.println("Connection failed.");
        conn.close();
    }
}
