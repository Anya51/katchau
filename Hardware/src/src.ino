#include <EmonLib.h>
#include <MySQL_Cursor.h>
#include <MySQL_Connection.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Dns.h>

#define ct5 1

EnergyMonitor sct;
double KWH = 0.0;
double KWH_total = 0.0;
float volt, c_value;
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

char hostname[] = "us-cdbr-iron-east-02.cleardb.net"; // change to your server's hostname/URL
char user[] = "baf1fa41c54e25";               // MySQL user login username
char password[] = "f9f37d2b";         // MySQL user login password

IPAddress ip(10, 27, 195, 195);
IPAddress gateway(10, 27, 0, 3);
IPAddress subnet(255, 255, 0, 0);
IPAddress server_ip(52, 204, 50, 0);
EthernetServer server(80);
EthernetClient client;
MySQL_Connection conn((Client*) &client);
DNSClient dns_client;

//char INSERT_DATA[] = "INSERT INTO test_arduino.hello_sensor (message, sensor_num, value) VALUES ('%s',%d,%s)";
char INSERT_SQL[] = "INSERT INTO heroku_afe57191d90821d.potency (value) VALUES (%s)";
char query [128];
char S_cvalue[10];
void setup() {
  pinMode(ct5, INPUT);

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println("Ethernet WebServer Example");

  Ethernet.begin(mac);

  dns_client.begin(Ethernet.dnsServerIP());
  dns_client.getHostByName(hostname, server_ip);
  Serial.println(server_ip);
  Serial.println("Connecting...");

  volt = ((5.0 / 1023.0) * analogRead(ct5)) - 5.0;
  if (volt < 0.0)
    volt *= -1.0;
  c_value = volt / 0.04;
  if (conn.connect(server_ip, 3306, user, password)) {
    // Initiate the query class instance
    MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
    // Save
    dtostrf(c_value, 1, 1, S_cvalue);
    sprintf(query, INSERT_SQL, S_cvalue);
    // Execute the query
    cur_mem->execute(query);
    delete cur_mem;
    Serial.println("Data recorded.");
  }
  else
    Serial.println("Connection failed.");
  conn.close();
}
void loop() {
  volt = ((5.0 / 1023.0) * analogRead(ct5)) - 5.0;
  if (volt < 0.0)
    volt *= -1.0;
  c_value = volt / 0.04;
  Serial.print("Volt: ");
  Serial.print(volt, 2);
  Serial.print("| Corrente: ");
  Serial.print(c_value, 2);
  Serial.print("A\n");
  delay(741);
}
