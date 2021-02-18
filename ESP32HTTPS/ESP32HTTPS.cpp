#include "ESP32HTTPS.h"


int utilCharToInt(char c){
  switch(c){
    case '0':
      return 0;
    case'1':
      return 1;
    case'2':
      return 2;
    case'3':
      return 3;
    case'4':
      return 4;
    case'5':
      return 5;
    case'6':
      return 6;
    case'7':
      return 7;
    case'8':
      return 8;
    case'9':
      return 9;
    default:
      return -1;
  }
}

//-------------------------------------------------------------------------
HTTPSRequest::HTTPSRequest(String newHttpMethod, String newHttpResource, String newPayload){
	httpMethod = newHttpMethod;
	httpResource = newHttpResource;
	httpPayload = newPayload;
	httpResquestHeaders = "";
}

HTTPSRequest::~HTTPSRequest() {
}

void HTTPSRequest::setPayload(String newPayload){
	httpPayload = newPayload;
}

String HTTPSRequest::getPayload(){
	return httpPayload;
}

String HTTPSRequest::getMethod(){
	return httpMethod;
}

String HTTPSRequest::getResource(){
	return httpResource;
}

String HTTPSRequest::getRequestHeaders(){
	return httpResquestHeaders;
}

//Se pueden poner varias cabeceras, ya que se concatenan.
void HTTPSRequest::setHeader(String headerName, String headerValue){
	httpResquestHeaders += headerName + ": " + headerValue + "\r\n";
}

void HTTPSRequest::removeHeaders(){
	httpResquestHeaders = "";
}

//-------------------------------------------------------------------------
HTTPSResponse::HTTPSResponse(unsigned int newHttpResponseCode, String newHeaders, String newHttpResponseData){
	httpResponseData = newHttpResponseData;
  	httpResponseCode = newHttpResponseCode;
  	httpResponseHeaders = newHeaders;
}

HTTPSResponse::~HTTPSResponse() {
}

String HTTPSResponse::getPayload(){
	return httpResponseData;
}

String HTTPSResponse::getResponseHeaders(){
	return httpResponseHeaders;
}

unsigned int HTTPSResponse::getResponseCode(){
	return httpResponseCode;
}

//-------------------------------------------------------------------------
HTTPS::HTTPS(char* newHost, unsigned int newPort, char* newRootCACert, bool logger){
	host = newHost;
	port = newPort;
	rootCACert = newRootCACert;

	client.setCACert(rootCACert);

	log = logger;
	if(log){
		Serial.begin(SERIAL_BAUDS);
		Serial.println("HTTPS.h: HTTPS.h log mode enabled");

		Serial.print("HTTPS.h: host -> ");
		Serial.println(host);
		Serial.print("HTTPS.h: port -> ");
		Serial.println(port);
		Serial.print("HTTPS.h: rootCaCert -> ");
		Serial.println(rootCACert);
	}
}

HTTPS::~HTTPS() {
}

char* HTTPS::getHost(){
	return host;
}

String HTTPS::getStringHost(){
	return String(host);
}

unsigned int HTTPS::getPort(){
	return port;
}

void HTTPS::setRequest(HTTPSRequest req){
	httpsRequest = req;
}

HTTPSRequest HTTPS::getRequest(){
	return httpsRequest;
}

HTTPSResponse HTTPS::getResponse(){
	return httpsResponse;
}

WiFiClientSecure HTTPS::getWifiClientSecure(){
	return client;
}

HTTPSResponse HTTPS::sendRequest(){

	HTTPSResponse res;

	if(client.connect(host, port)){
		if(log){
			Serial.println("HTTPS.h: Connected to server");
		}

		String data = httpsRequest.getMethod() + " " + httpsRequest.getResource() + " HTTP/1.1\r\n";
		data += "Host: " + getStringHost() + ":" + (String)port + "\r\n";
		data += "Content-Length: " + (String)httpsRequest.getPayload().length() + "\r\n";
		data += httpsRequest.getRequestHeaders();
		data += "\r\n";
		data += httpsRequest.getPayload();

		if(log){
			Serial.println("HTTPS.h: Request -> ");
			Serial.println(data);
		}

		client.print(data);

		
		String firstLine = client.readStringUntil('\n');
		String responseHeaders = "";
		String payload = "";

		unsigned int code;


		for(int i = 0; i < firstLine.length(); i++){
			if(firstLine[i] == ' '){
				code = 100*utilCharToInt(firstLine[i+1]);
				code += 10*utilCharToInt(firstLine[i+2]);
				code += utilCharToInt(firstLine[i+3]);

				break;
			}
		}

		while (client.connected()){
			String line = client.readStringUntil('\n');

			if(line == "\r"){
		    	while (client.available()) {
		    		char c = client.read();
			    	payload += String(c);
			    }
				break;
			} else {
				responseHeaders += line + "\n";
			}
		}

		if(log){
			Serial.print("HTTPS.h: ResponseCode -> ");
			Serial.println(code);
			Serial.print("HTTPS.h: ResponseHeaders -> ");
			Serial.println(responseHeaders);
			Serial.print("HTTPS.h: ResponsePayload -> ");
			Serial.println(payload);
		}

		res = HTTPSResponse(code, responseHeaders, payload);
		httpsResponse = res;
	} else if(log){
		Serial.println("HTTPS.h: Cannot connect to server.");
	}

	client.stop();

	return res;
}


HTTPSResponse HTTPS::sendRequest(HTTPSRequest request){
	HTTPSResponse res;

	if(client.connect(host, port)){
		if(log){
			Serial.println("HTTPS.h: Connected to server");
		}
		String data = request.getMethod() + " " + request.getResource() + " HTTP/1.1\r\n";
		data += "Host: " + getStringHost() + ":" + (String)port + "\r\n";
		data += "Content-Length: " + (String)request.getPayload().length() + "\r\n";
		data += request.getRequestHeaders();
		data += "\r\n";
		data += request.getPayload();

		if(log){
			Serial.println("HTTPS.h: Request -> ");
			Serial.println(data);
		}

		client.print(data);

		
		String firstLine = client.readStringUntil('\n');
		String responseHeaders = "";
		String payload = "";

		unsigned int code;


		for(int i = 0; i < firstLine.length(); i++){
			if(firstLine[i] == ' '){
				code = 100*utilCharToInt(firstLine[i+1]);
				code += 10*utilCharToInt(firstLine[i+2]);
				code += utilCharToInt(firstLine[i+3]);

				break;
			}
		}

		while (client.connected()){
			String line = client.readStringUntil('\n');

			if(line == "\r"){
		    	while (client.available()) {
		    		char c = client.read();
			    	payload += String(c);
			    }
				break;
			} else {
				responseHeaders += line + "\n";
			}
		}

		if(log){
			Serial.print("HTTPS.h: ResponseCode -> ");
			Serial.println(code);
			Serial.print("HTTPS.h: ResponseHeaders -> ");
			Serial.println(responseHeaders);
			Serial.print("HTTPS.h: ResponsePayload -> ");
			Serial.println(payload);
		}

		res = HTTPSResponse(code, responseHeaders, payload);
		httpsResponse = res;
	} else if(log){
		Serial.println("HTTPS.h: Cannot connect to server.");
	}

	client.stop();

	return res;	
}