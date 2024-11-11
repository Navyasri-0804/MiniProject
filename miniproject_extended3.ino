
#include <DHT.h>  
#include <ESP8266WiFi.h>
#define DHTPIN D3        
#define GAS    D4  
#define NRML   D5  
#define FULL   D6
#define NRML2  D7
#define FULL2  D8

DHT dht(DHTPIN, DHT11);

String apiKey = "RT8DMEBRETQKMNYN";     //  Enter your Write API key from ThingSpeak
const char *ssid =  "Komali";     // replace with your wifi ssid and wpa2 key
const char *pass =  "12345678";
const char* server = "api.thingspeak.com";
WiFiClient client;

int flag1=0;
int flag2=0;
int flag3=0;
int flag5=0;
int flag6=0;
int flag7=0;
int flag8=0;
int flag9=0;

float t=0;
float h=0;
int i=0;
void setup()
{
       Serial.begin(9600);
       pinMode(GAS, INPUT);
       pinMode(FULL, INPUT);        
       pinMode(NRML, INPUT);  
       pinMode(FULL2, INPUT);        
       pinMode(NRML2, INPUT); 

       Serial.println("Connecting to ");
       Serial.println(ssid);
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED)
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
        dht.begin();
}
 
void loop()
{
  for(i=0;i<50;i++)
  {
  if(digitalRead(GAS)==LOW)
  {
  flag1=1;
  Serial.println("GAS");  
  }
  else
  {
  flag1=0;
  Serial.println("NO GAS");    
  }


  
if((digitalRead(FULL2)==LOW)&&(digitalRead(NRML2)==LOW))
     {
        flag5=1;
        flag6=0;
        Serial.println("fill2");  
        //goto xit;  
     }
     
  if((digitalRead(FULL)==LOW)&&(digitalRead(NRML)==LOW))
  {
        flag2=1;
        flag3=0;
        Serial.println("fill");  
        goto xit;  
  }

    if((digitalRead(FULL2)==HIGH)&&(digitalRead(NRML2)==HIGH))
    {
      flag5=0;
      flag6=1; 
      Serial.println("no flow2");  
      //goto xit;  
    }
      
    if((digitalRead(FULL)==HIGH)&&(digitalRead(NRML)==HIGH))
   {
      flag2=0;
      flag3=1;
      Serial.println("no flow");  
      goto xit;  
    } 
    if((digitalRead(NRML)==LOW) && (digitalRead(NRML2)==LOW))
    {  
      flag9=1;
      Serial.println("normal flow");  
      goto xit;
    }

  flag2=0;
  flag3=0; 
  flag5=0;
  flag6=0;
  
     
xit:  

      if((flag2==HIGH) && (flag5==HIGH))
      {  
      flag7=1;
      Serial.println("Overflow");  
      }
      if((flag5==HIGH) && (flag3==HIGH))
      {  
      flag8=1;
      Serial.println("Blocked b/w a and b");  
      }

      
       h = dht.readHumidity();
       t = dht.readTemperature();
     
                             Serial.print("Temperature: ");
                             Serial.println(t);
                             Serial.print("Humidity: ");
                             Serial.println(h);  

  delay(100);
 }
                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                           
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(h);
                             postStr +="&field3=";
                             postStr += String(flag1);  
                             postStr +="&field4=";
                             postStr += String(flag7);   
                             postStr +="&field5=";
                             postStr += String(flag3);
                             postStr +="&field6=";
                             postStr += String(flag8); 
                             postStr +="&field7=";
                             postStr += String(flag9); 

                               
                            /* postStr +="&field6=";
                             postStr += String(flag5);
                             postStr +="&field7=";
                             postStr += String(flag6);                              
                             postStr +="&field8=";
                             postStr += String(flag2);
                             postStr +="&field9=";
                             postStr += String(flag3); */

                                                                                                            
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 

                        }
          client.stop();
          Serial.println("Waiting...");
          delay(2000);
 
}
