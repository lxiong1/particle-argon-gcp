/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/home/luexiong/projects/particle-argon-gcp/src/particle-argon-gcp.ino"
// google cloud code modified
void setup();
void loop(void);
void publishData(float f);
#line 2 "/home/luexiong/projects/particle-argon-gcp/src/particle-argon-gcp.ino"
void deviceNameHandler(const char *topic, const char *data);
void publishData();
const char *PUBLISH_EVENT_NAME = "gcp-event";
String deviceName;

void setup() {
  Serial.begin(9600);
  // Set up 'power' pins, comment out if not used!
  pinMode(D7, OUTPUT);
  digitalWrite(D7, LOW);  
  
  Particle.subscribe("spark/", deviceNameHandler);
  Particle.publish("spark/device/name");
}

// up to here, it is the same as the address acanner
// we need a few more variables for this example

void loop(void) {
  float fahrenheit = 72;

  if(fahrenheit > 40 && fahrenheit < 90){   // unit is in my basement, expecting reasonable temperatures
      // now that we have the readings, we can publish them to the cloud
      String temperature = String(fahrenheit); // store temp in "temperature" string      
      digitalWrite(D7,HIGH);
      publishData(fahrenheit);
      delay(100);
      digitalWrite(D7,LOW);      
  }

  delay(15000);
}

void publishData(float f) {

	char buf[256];
	snprintf(buf, sizeof(buf), "{\"temperature\":%.3f,\"device\":\"%s\"}", f, deviceName.c_str());
	Serial.printlnf("publishing %s", buf);
	Particle.publish(PUBLISH_EVENT_NAME, buf, PRIVATE);
}

void deviceNameHandler(const char *topic, const char *data) {
	deviceName = data;
}