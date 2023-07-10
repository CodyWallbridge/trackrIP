#include "trackr.hpp"
#include <iostream>
#include <unistd.h>
#include "WiFi.h"

#define CONNECT_RETRIES 20

char ssid[] = "SHAW-608330";
char pass[] = "251169015919";

char apiKey[] = "jA5SQ9953e6dRUC4FBCGNTLakUgMIjPexJHXZkp2kWc4MC7Kk90IAEqWd3HQieYA";
int fieldId = 3;

int status = WL_IDLE_STATUS;

void setup() {
  int retries = 0;
  
  Serial.begin(115200);
  Serial.print("SSID: ");
  Serial.println(ssid);

  Serial.print("Connecting to the wireless network...");

  WiFi.mode(WIFI_STA);
  status = WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED && retries < CONNECT_RETRIES)
  {
      delay(500);
      Serial.print(".");
      retries++;
  }

  Serial.println("!");
    
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Couldn't get a wifi connection");
    while (true);
  } else {
    Serial.println("Connected to wifi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    example();
  }
}

void loop() {
  
}

void example() {  
    // Add single value
    long rc = Trackr::addSingleValue(3, 100, apiKey);

    Serial.println("HTTP response code: " + rc);
    
    // You can also add a batch of values (warning: slow! 1s per value due to API rate limiting)
    std::list<int> values = { 100, 200, 300, 400, 500 };
    rc = Trackr::addMultipleValues(3, values, apiKey);

    Serial.print("HTTP response code: ");
    Serial.println(rc);

    // If you dont want to specify the API key in every API call, you can set it globally:
    Trackr::setApiKey(apiKey);

    // Now you can omit the API key in the API calls:
    rc = Trackr::addSingleValue(3, 100);

    Serial.print("HTTP response code: ");
    Serial.println(rc);
    
    // We can also fetch values from the API:
    Trackr::TrackrValuesResponse* response = Trackr::getValues(3, 0, 10, true);

    Serial.print("Total values: ");
    Serial.println(response->totalValues);
    
    for (Trackr::TrackrValue* value : *response->values) {
      Serial.print("{ id: ");
      Serial.print(value->id);
      Serial.print(", value: ");
      Serial.print(value->value);
      Serial.print(", createdAt: ");
      Serial.print(value->createdAt);
      Serial.println(" }");
    }
}
