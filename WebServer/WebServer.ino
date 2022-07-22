#include <WiFi.h>
#include <WiFiManager.h>
#include <SoftwareSerial.h>


WiFiServer server(80);
SoftwareSerial xbee(13,12);

String header;
String BB = "Not Available";
String YB = "Not Available";
String RECORD = "Not Available";
unsigned long currentTime = millis();
unsigned long previousTime = 0; 
const long timeoutTime = 2000;

void setup()
{
  WiFi.mode(WIFI_STA);
  Serial.begin(115200);
  xbee.begin(9600);
  WiFiManager wm;
  wm.resetSettings();
  bool res = wm.autoConnect("SMART_STOCK_DISPLAY","smartstockdisplay"); 
  
  if(!res)
  {
    Serial.println("Failed to connect");
  } 
  else
  {
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    server.begin();
  }
}

void loop()
{
  WiFiClient client = server.available();  

  if(client)
  {                           
    currentTime = millis();
    previousTime = currentTime;         
    String currentLine = "";                
    while (client.connected() && currentTime - previousTime <= timeoutTime)
    { 
      currentTime = millis();
      if (client.available())
      {             
        char c = client.read();                   
        header += c;
        if (c == '\n')
        {                   
          if (currentLine.length() == 0) 
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if (header.indexOf("GET /BB/available") >= 0)
            {
              BB = "Available";
              xbee.write('0');
              Serial.println("Blue Books Available");
              delay(1000);
            }
            else if (header.indexOf("GET /BB/notAvailable") >= 0)
            {
              BB = "Not Available";
              xbee.write('1');
              Serial.println("Blue Books Not Available");
              delay(1000);
            }
            else if (header.indexOf("GET /YB/available") >= 0)
            {
              YB = "Available";
              xbee.write('2');
              Serial.println("Yellow Books Available");
              delay(1000);
            }
            else if (header.indexOf("GET /YB/notAvailable") >= 0)
            {
              YB = "Not Available";
              xbee.write('3');
              Serial.println("Yellow Books Not Available");
              delay(1000);
            }
            else if (header.indexOf("GET /RECORD/available") >= 0)
            {
              RECORD = "Available";
              xbee.write('4');
              Serial.println("Records Available");
              delay(1000);
            }
            else if (header.indexOf("GET /RECORD/notAvailable") >= 0)
            {
              RECORD = "Not Available";
              xbee.write('5');
              Serial.println("Records Not Available");
              delay(1000);
            }


            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 20px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            
            client.println("<body><h1>SMART STOCK DISPLAY</h1>");
            
            client.println("<p>Blue Books Status -  " +  BB + "</p>");
            
            if (BB=="Not Available")
            {
              client.println("<p><a href=\"/BB/available\"><button class=\"button\">Available</button></a></p>");
            }
            else
            {
              client.println("<p><a href=\"/BB/notAvailable\"><button class=\"button button2\">Not Available</button></a></p>");
            } 
            
            client.println("<p>Yellow Books Status -  " +  YB + "</p>");
            
            if (YB=="Not Available") {
            client.println("<p><a href=\"/YB/available\"><button class=\"button\">Available</button></a></p>");
            } else {
            client.println("<p><a href=\"/YB/notAvailable\"><button class=\"button button2\">Not Available</button></a></p>");
            } 
            
            client.println("<p>Records Status -  " +  RECORD + "</p>");
            
            if (RECORD=="Not Available")
            {
              client.println("<p><a href=\"/RECORD/available\"><button class=\"button\">Available</button></a></p>");
            }
            else
            {
              client.println("<p><a href=\"/RECORD/notAvailable\"><button class=\"button button2\">Not Available</button></a></p>");
            } 
            
            client.println("</body></html>");
            client.println();
            break;
          }
          else
          { 
            currentLine = "";
          }
        }
        else if (c != '\r')
        { 
          currentLine += c;     
        }
      }
    }
            
    header = "";
    client.stop();
    Serial.println("");
  }
}
