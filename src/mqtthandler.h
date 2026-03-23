

void mqtt_setup() {
  client.setServer(mqtt_host, mqtt_port);

}


void reconnect() {

  while (!client.connected()) {

    Serial.print("Connecting to MQTT...");

    String clientId = "ESPClient-";
    clientId += String(random(0xffff), HEX);

    // Create LWT payload (offline message)
    StaticJsonDocument<128> willDoc;
    willDoc["device_id"] = device_id;
    willDoc["status"] = "offline";

    char willBuffer[128];
    serializeJson(willDoc, willBuffer);

    bool connected = client.connect(
      clientId.c_str(),
      mqtt_user,
      mqtt_password,
      topic_status.c_str(),   // LWT topic
      1,                      // QoS
      true,                   // retained
      willBuffer              // LWT message
    );

    if (connected) {

      Serial.println("connected!");

      // Publish ONLINE status (retained)
      publishStatus("online");

    } else {

      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds...");
      delay(5000);
    }
  }
}







void publishState() {
  JsonDocument doc;
 
  doc["state"]      = lightOn ? "ON" : "OFF";
  doc["brightness"] = brightness;
 
  // Nested color object
  JsonObject color = doc["color"].to<JsonObject>();
  color["r"] = ledR;
  color["g"] = ledG;
  color["b"] = ledB;
 
  // Array of active modes
  JsonArray modes = doc["modes"].to<JsonArray>();
  modes.add("rainbow");
  modes.add("sparkle");



 
  // Nested device object
  JsonObject device = doc["device"].to<JsonObject>();
  device["id"]       = "esp32-living-room";
  device["ip"]       = WiFi.localIP().toString();
  device["firmware"] = "1.2.3";
 
  size_t len = measureJson(doc);
  if (mqtt.beginPublish("home/lights/state", len, true)) {
    serializeJson(doc, mqtt);
    mqtt.endPublish();
  }
}



//ESP send status update to MQTT broker in JSON format. Todo: Implement this to send actual status updates based on device state.

{
  "ver": 1,
  "dvc": {
    "sn": chipID, //Serial number based on MAC address
    "fn": devicefnName, //Friendly name based on chipID
    "sv": fwversion, //Firmware version
    "cn": true, //Device is connected to MqTT broker. Implemented as LWT message in MQTT connection setup, will be set to false if device goes offline.
  },

  "wf": {  //Kanskje flytt inn i dvc objektet senere?
    "sid": WiFi.SSID(), //Connected WiFi SSID
    "ip": WiFi.localIP().toString(), //Device IP address
    "wfr": WiFi.RSSI(), //WiFi signal strength
  },

  "sta": {
    "md": devicecurrentmode, //Light state ON/OFF
  }

  "pwr": {
    "chr": charging, //Light state ON/OFF
    "bat": battery, //Battery level in percentage. Todo: Implement battery level reading and reporting.
  }
}

//ESP receive command from MQTT broker to change light state. Todo: Implement this to actually change light state based on received commands.

...