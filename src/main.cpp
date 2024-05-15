#include <Arduino.h>
#include <MySQL_Connection.h>

void setup() {
    Serial.begin(9600);
}

void loop() {
    Serial.println("Hello World");
    sleep(1000);
}

// Create mysql Check if table if exist if not then create table name "cs423"
void createTable() {
    MySQL_Connection conn((Client *)&client);
    if (conn.connect(server_addr, 3306, user, password)) {
        Serial.println("Connected to MySQL server");
        delay(1000);
        MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
        cur_mem->execute("SELECT * FROM cs423");
        row_values *row = NULL;
        do {
            row = cur_mem->get_next_row();
            if (row != NULL) {
                Serial.println(row->values[0]);
            }
        } while (row != NULL);
        delete cur_mem;
        conn.close();
    } else {
        Serial.println("Connection failed.");
    }
}