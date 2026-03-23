




// MqTT credentials and configuration
const char* MQTT_HOST  = "mqtt.yourdomain.com"; // Ip-address or domain of your MQTT broker/
const int   MQTT_PORT  = 8883;   // MQTT broker port (default 1883 for unencrypted, 8883 for TLS)
const char* MQTT_USER  = "myuser"; // MQTT username
const char* MQTT_PASS  = "mypassword"; // MQTT topic to publish light state
const char* CLIENT_ID  = "ExoPingClient"; // Unique client ID for MQTT connection
