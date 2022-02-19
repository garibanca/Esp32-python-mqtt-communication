// This example uses an ESP32 Development Board
// to connect to shiftr.io.
//
// You can check on your device after a successful
// connection here: https://www.shiftr.io/try.
//
// by Joël Gähwiler
// https://github.com/256dpi/arduino-mqtt

#include <WiFiClientSecure.h>
#include <MQTT.h>
#include "kamera.h"
//#include <base64.h>
//extern "C" {
//#include "crypto/base64.h"
//}
#include "mbedtls/base64.h"


const char ssid[] = "Wifi";
const char pass[] = "Wifisifre";




WiFiClientSecure net;
MQTTClient client(10000);

unsigned long lastMillis = 0;

void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("arduino", "isim", "mqttsifre")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/hello");
  client.subscribe("istek");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  if (topic == "istek")
  {
    camera_capture();
  }

  // Note: Do not use the client in the callback to publish, subscribe or
  // unsubscribe as it may cause deadlocks when other things arrive while
  // sending and receiving acknowledgments. Instead, change a global variable,
  // or push to a queue and handle it in the loop after calling `client.loop()`.
}

void setup() {
  net.setInsecure();
  Serial.begin(115200);
  WiFi.begin(ssid, pass);

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported
  // by Arduino. You need to set the IP address directly.
  //
  // MQTT brokers usually use port 8883 for secure connections.
  client.begin("694e712811a34dd8802e5445ec37cd96.s2.eu.hivemq.cloud", 8883, net);
  client.onMessage(messageReceived);
  camera_init();
  connect();
}

void loop() {
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }

  // publish a message roughly every second.
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    client.publish("my/test/topic", "world");
    sado=Serial.read();
    
    if((sado == 'c' || sado == 'b') || sado == 'a'){
  
  
    camera_capture();
  
    }

  }
  client.loop();
}

esp_err_t camera_capture(){
    //acquire a frame
    Serial.println("  \n deniyoz bisiyler \n");
    fb = NULL;

    size_t _jpg_buf_len = 0;
    uint8_t * _jpg_buf = NULL;


    
    size_t fb_len = 0;
    Serial.println("  \n deniyoz bisiyler \n");

    
    fb = esp_camera_fb_get();
    esp_err_t res = ESP_OK;

    if (!fb) {
        Serial.println("  \n Camera Capture Failed \n");
        return ESP_FAIL;
    }
    //replace this with your own function
    //process_image(fb->width, fb->height, fb->format, fb->buf, fb->len)
  

  Serial.println("  \n bura \n");
  Serial.println(fb->len);
  Serial.println("  \n bura \n");
  
  bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
  Serial.println(_jpg_buf_len);
  if(!jpeg_converted){
    Serial.println("JPEG compression failed");
    res = ESP_FAIL;
  }

//  Serial.println(_jpg_buf_len);
//  Serial.println("  \n oraa \n");
//  String imgDataB64 = base64::encode(fb->buf, fb->len);
  
//  String devlet ="dsfnfdlkmsdflsdkmfekwrmslkvxcvxcvxcvxcvxcvxcvxvxgggggggggggggggggggggggggggggggggffmsdlfmsdlfmksldkgmdlfgdfmlkmdlkmdslfmsodfmsdl dlkgmfsmdlmfsdlfmsldfmsldfmsldfmlsd";
//  size_t nekadar = imgDataB64.length();
//    size_t nekadar = devlet.length();
//    Serial.println(nekadar);
//    char* bufum= (char*) malloc(nekadar);
//  imgDataB64.toCharArray(bufum,nekadar+1);
//    devlet.toCharArray(bufum,nekadar+1);
//  Serial.println("  \n şura \n");
    

Serial.println("PSRAM found: " + String(psramFound()));
//
//  int image_buf_size = 200 * 150;                                                  
//  uint8_t *image = (uint8_t *)ps_calloc(image_buf_size, sizeof(char));
//  
//  size_t length=fb->len;
//  
//  size_t olen;
//  
//  Serial.print("length is");
//  
//  Serial.println(length);
//  int err1= mbedtls_base64_encode(image, image_buf_size, &olen, fb->buf, length);
//  
//  Serial.println(err1);
//  
//  String img((const __FlashStringHelper*) image);
//  Serial.println("\n");
//  Serial.println(img);
//  Serial.println("\n");
//  Serial.println(img.c_str());
//  Serial.println("\n");
//  //msg1=img;

  //Serial.println(sizeof(*bufum));
  // Now we can publish stuff!
  Serial.print(F("\nSending val "));
//  Serial.print(x);
  Serial.print(F(" to test feed..."));

  if (!client.publish("kamera",(char*)_jpg_buf,_jpg_buf_len)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
    client.publish("my/test/topic", "gonderildi");
  
  if (sado == 'b') {
  int k=0;
  for (int j = 0; j < fb->height; j++){
    for (int i = 0; i < fb->width; i++) {
      unsigned short p = fb->buf[k];
      k++;
      Serial.print(" ,0x");
      if (p < 0x1000) {
        Serial.print('0');
      }

      if (p < 0x0100) {
        Serial.print('0');
      }

      if (p < 0x0010) {
        Serial.print('0');
      }
      
      Serial.print(p, HEX);
    }
    Serial.print('\n');
  }
  Serial.print("\n   "  + String(k)  +"  \n   ");
  Serial.print("\n   "  + String(fb->width)  +"  \n   ");
  Serial.print("\n   "  + String(fb->height)  +"  \n   ");
  
  }
 if (sado == 'c') {
    unsigned short p = fb->buf[fb->len/2];
    Serial.print(" ,0x");
    if (p < 0x1000) {
      Serial.print('0');
    }

    if (p < 0x0100) {
      Serial.print('0');
    }

    if (p < 0x0010) {
      Serial.print('0');
    }
    Serial.print(p, HEX);

}
  //free(bufum);
  free(_jpg_buf);
  _jpg_buf = NULL;
  sado == 'd';
    
  //return the frame buffer back to the driver for reuse
  esp_camera_fb_return(fb);
  return ESP_OK;
}
