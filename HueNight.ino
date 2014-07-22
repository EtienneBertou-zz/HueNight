/*
** Include files
** Ethernet Shield required
*/

#include <Ethernet.h>
#include <SPI.h>

/*
** Network setup
*/
// Older shields that do not have a sticker take a random mac byte
byte mac[]    = {  0x90, 0xA2, 0xDA, 0x0F, 0x24, 0xA8 };
// IP of your Hue bridge in the network. Can be found in your router or at www.meethue.com/api/nupnp.
// I will add a refined class for bridge discovery later.
byte server[] = { 192, 168, 1, 164 };
// Initialize Ethernet client
EthernetClient client;

/*
** Authentication
*/
// The username who has access to the API (see http://developers.meethue.com/gettingstarted.html to create a user)
// This process also has to be automated. Letting the bridge generate a username is advised.
const String username = "newdeveloper";

/*
** Read an HTTP response
*/
void readResponse() {
  while (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  client.stop();
  Serial.println();
  Serial.println("End of response..");
}

/*
** Test request -- could be used to find out how many lights we have
*/

void sendTestRequest() {
  if (client.connect(server, 80)) {
    Serial.println("Connected to server on port 80..");
    String req = "GET /api/";
    req += username;
    req += "/lights HTTP/1.1";
    client.println(req);
    delay(500);
    readResponse();
    client.stop();
  } else {
    Serial.println("Connection failed..");
  }
}

/*
** Send a command to light 1.
*/

void sendToLight(String body) {
  String req;
  Serial.println("Sending command to api/newdeveloper/lights/1");
  if (client.connect(server, 80)) {
    Serial.println("Connected to resource..");
    req = "";
    req = "PUT /api/";
    req += username;
    req += "/lights/";
    req += "1";
    req += "/state HTTP/1.1";
    client.println(req);
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(body.length());
    client.println();
    client.println(body);
    // Allow client to receive command and respond
    delay(500);
    readResponse();
    client.stop();
  } else {
    Serial.println("Cannot connect to server..");
  }

}

/*
** Setup
*/
void setup() {
  Serial.begin(9600);
  Serial.println("Setting up Network...");
  Ethernet.begin(mac);
  //Wait for the network to initialize
  delay(1000);
  Serial.println("Sending test request..");
  sendTestRequest();
  //sendToLight("{\"on\":false}");
}

/*
** Loop
*/
void loop() {
  String req = "";
  // Breathe in
  req = "{\"on\": true,\"hue\":10000,\"sat\":100,\"bri\":100,\"transitiontime\":20}";
  // Print the command for debugging
  Serial.println(req);
  sendToLight(req);

  // Breathe out
  req = "{\"on\": true,\"hue\":8000,\"sat\":140,\"bri\":1,\"transitiontime\":20}";
  // Print the command for debugging
  Serial.println(req);
  sendToLight(req);

  delay(2500);
}
