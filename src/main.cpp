#include <WiFi.h>
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

MySQL_Connection conn((Client *)&client);

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
    createTable();

    // get time from os
    time_t currentTime = time(NULL); // Add this line
    String time = String(currentTime); // Add this line
    insertData(time);
}

void loop() {
    Serial.println("Hello World");
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
