void deviceNameHandler(const char *topic, const char *data);
void publishData();
const char *PUBLISH_EVENT_NAME = "gcp-event";
String deviceName;

void setup() {
  Serial.begin(9600);

  pinMode(D7, OUTPUT);
  digitalWrite(D7, LOW);  
  
  Particle.subscribe("spark/", deviceNameHandler);
  Particle.publish("spark/device/name");
}

void loop(void) {
  float fahrenheit = 72;

  if(fahrenheit > 40 && fahrenheit < 90){
      String temperature = String(fahrenheit);
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