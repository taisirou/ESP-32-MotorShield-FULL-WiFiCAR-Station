/* WiFi-Control-Car */

#include <WiFi.h>
const char* ssid     = "Goripin0";      //アクセスポイントのSSID
const char* password = "megumi11";     //アクセスポイントのパスワード
const char html[] =
  "<!DOCTYPE html><html lang='ja'><head><meta charset='UTF-8'>\
<style>input {margin:8px;width:80px;}\
div {font-size:16pt;color:red;text-align:center;width:400px;border:groove 40px orange;}</style>\
<title>WiFi_Car Controller</title></head>\
<body><div><p>Tank Controller</p>\
<form method='get'>\
<input type='submit' name='le' value='左' />\
<input type='submit' name='fo' value='前' />\
<input type='submit' name='ri' value='右' /><br>\
<input type='submit' name='st' value='停止' /><br>\
<input type='submit' name='bl' value='後左' />\
<input type='submit' name='ba' value='後ろ' />\
<input type='submit' name='br' value='後右' /><br><br>\
</form></div></body></html>";

void stop() {
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);
  ledcWrite(0, 0);
  digitalWrite(25, LOW);
  digitalWrite(26, LOW);
  ledcWrite(1, 0);
}

WiFiServer server(80);

void setup()
{
  Serial.begin(115200);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);
  ledcSetup(0, 490, 8);
  ledcSetup(1, 490, 8);
  ledcAttachPin(14, 0);
  ledcAttachPin(27, 1);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();

}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.print(html);
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        if (currentLine.endsWith("GET /?fo")) {
          stop();
          digitalWrite(12, LOW);
          digitalWrite(13, HIGH);
          ledcWrite(0, 255);
          digitalWrite(25, LOW);
          digitalWrite(26, HIGH);
          digitalWrite(27, HIGH);
          ledcWrite(1, 255);
        }
        if (currentLine.endsWith("GET /?le")) {
          stop();
          digitalWrite(12, LOW);
          digitalWrite(13, HIGH);
          ledcWrite(0, 255);
          digitalWrite(25, LOW);
          digitalWrite(26, HIGH);
          digitalWrite(27, HIGH);
          ledcWrite(1, 200);
        }
        if (currentLine.endsWith("GET /?ri")) {
          stop();
          digitalWrite(12, HIGH);
          digitalWrite(13, LOW);
          ledcWrite(0, 200);
          digitalWrite(25, LOW);
          digitalWrite(26, HIGH);
          ledcWrite(1, 255);
        }
        if (currentLine.endsWith("GET /?ba")) {
          stop();
          digitalWrite(12, HIGH);
          digitalWrite(13, LOW);
          ledcWrite(0, 255);
          digitalWrite(25, HIGH);
          digitalWrite(26, LOW);
          ledcWrite(1, 255);
        }
        if (currentLine.endsWith("GET /?bl")) {
          stop();
          digitalWrite(12, HIGH);
          digitalWrite(13, LOW);
          ledcWrite(0, 255);
          digitalWrite(25, HIGH);
          digitalWrite(26, LOW);
          ledcWrite(1, 200);
        }
        if (currentLine.endsWith("GET /?br")) {
          stop();
          digitalWrite(12, HIGH);
          digitalWrite(13, LOW);
          ledcWrite(0, 200);
          digitalWrite(25, HIGH);
          digitalWrite(26, LOW);
          ledcWrite(1, 255);
        }
        if (currentLine.endsWith("GET /?st")) {
          stop();
          digitalWrite(12, LOW);
          digitalWrite(13, LOW);
          ledcWrite(0, 0);
          digitalWrite(25, LOW);
          digitalWrite(26, LOW);
          ledcWrite(1, 0);
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
