/*
 * EXAMPLE OF ESP32HTTP library
 * Author: migue27au --> https://github.com/migue27au
 * Last Update: 18/02/2021
 */
#include "ESP32HTTPS.h"

char* server = "www.arduino.cc";
String path = "/asciilogo.txt";

char* ssid = "SSID";
char* password = "SSID PASSWORD";

//the rootCA certificate of the destination server
char* rootCA = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDdzCCAl+gAwIBAgIEAgAAuTANBgkqhkiG9w0BAQUFADBaMQswCQYDVQQGEwJJ\n" \
"RTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJlclRydXN0MSIwIAYD\n" \
"VQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTAwMDUxMjE4NDYwMFoX\n" \
"DTI1MDUxMjIzNTkwMFowWjELMAkGA1UEBhMCSUUxEjAQBgNVBAoTCUJhbHRpbW9y\n" \
"ZTETMBEGA1UECxMKQ3liZXJUcnVzdDEiMCAGA1UEAxMZQmFsdGltb3JlIEN5YmVy\n" \
"VHJ1c3QgUm9vdDCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKMEuyKr\n" \
"mD1X6CZymrV51Cni4eiVgLGw41uOKymaZN+hXe2wCQVt2yguzmKiYv60iNoS6zjr\n" \
"IZ3AQSsBUnuId9Mcj8e6uYi1agnnc+gRQKfRzMpijS3ljwumUNKoUMMo6vWrJYeK\n" \
"mpYcqWe4PwzV9/lSEy/CG9VwcPCPwBLKBsua4dnKM3p31vjsufFoREJIE9LAwqSu\n" \
"XmD+tqYF/LTdB1kC1FkYmGP1pWPgkAx9XbIGevOF6uvUA65ehD5f/xXtabz5OTZy\n" \
"dc93Uk3zyZAsuT3lySNTPx8kmCFcB5kpvcY67Oduhjprl3RjM71oGDHweI12v/ye\n" \
"jl0qhqdNkNwnGjkCAwEAAaNFMEMwHQYDVR0OBBYEFOWdWTCCR1jMrPoIVDaGezq1\n" \
"BE3wMBIGA1UdEwEB/wQIMAYBAf8CAQMwDgYDVR0PAQH/BAQDAgEGMA0GCSqGSIb3\n" \
"DQEBBQUAA4IBAQCFDF2O5G9RaEIFoN27TyclhAO992T9Ldcw46QQF+vaKSm2eT92\n" \
"9hkTI7gQCvlYpNRhcL0EYWoSihfVCr3FvDB81ukMJY2GQE/szKN+OMY3EU/t3Wgx\n" \
"jkzSswF07r51XgdIGn9w/xZchMB5hbgF/X++ZRGjD8ACtPhSNzkE1akxehi/oCr0\n" \
"Epn3o0WC4zxe9Z2etciefC7IpJ5OCBRLbf1wbWsaY71k5h+3zvDyny67G7fyUIhz\n" \
"ksLi4xaNmjICq44Y3ekQEe5+NauQrz4wlHrQMz2nZQ/1/I6eYs9HRCwBXbsdtTLS\n" \
"R9I4LtD+gdwyah617jzV/OeBHRnDJELqYzmp\n" \
"-----END CERTIFICATE-----\n";


HTTPS https(server, HTTPS_PORT, rootCA);
//HTTPS https(server, HTTP_PORT, rootCA, true);  //If you want to see the logger output. (Serial Speed is 115200 bauds)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    /*
      0: WL_IDLE_STATUS when Wi-Fi is changing state
      1: WL_NO_SSID_AVAIL if the configured SSID cannot be reached
      3: WL_CONNECTED after connection is established
      4: WL_CONNECT_FAILED if the password is incorrect
      6: WL_DISCONNECTED if the module is not configured in station mode
    */
   
    Serial.print(WiFi.status());
    counter++;
    if(counter>=60){ //after 30 seconds timeout - reset board
      ESP.restart();
    }
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  HTTPSRequest request(HTTP_GET, path);
  //HTTPSRequest request(HTTP_POST, path, "your payload for post request here");
  request.setHeader("User-Agent", "ESP32");

  HTTPSResponse response = https.sendRequest(request);

  int responseCode = response.getResponseCode();
  Serial.print("Responde Code: ");
  Serial.println(responseCode);
  Serial.println("Payload:");
  Serial.println(response.getPayload());
  
  if(responseCode == HTTP_RESPONSE_OK){
    Serial.println("Successfull response");
  } else{
    Serial.println("Wrong response");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
