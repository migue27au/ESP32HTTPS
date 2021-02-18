/*
*	Author: Migue27au --> https://github.com/migue27au
*	Last Update: 18/02/2021
*
*	Library to handle easier the HTTP conecctions between ESP32 and server
*/

#ifndef ESP32HTTPS_H
#define ESP32HTTPS_H

#include "Arduino.h"
#include <string.h>
#include <WiFiClientSecure.h>

#define SERIAL_BAUDS 115200

#define HTTP_POST "POST"
#define HTTP_GET "GET"
#define HTTP_DELETE "DELETE"
#define HTTP_PUT "PUT"
#define HTTP_HEAD "HEAD"
#define HTTP_OPTIONS "OPTIONS"

#define HTTP_RESPONSE_OK 200
#define HTTP_RESPONSE_CREATED 201
#define HTTP_RESPONSE_ACCEPTED 202
#define HTTP_RESPONSE_MULTIPLE_CHOICE 300
#define HTTP_RESPONSE_MOVED_PERMANENTLY 301
#define HTTP_RESPONSE_FOUND 302
#define HTTP_RESPONSE_BAD_REQUEST 400
#define HTTP_RESPONSE_UNAUTHORIZED 401
#define HTTP_RESPONSE_FORBIDDEN 403
#define HTTP_RESPONSE_METHOD_NOT_ALLOWED 405
#define HTTP_RESPONSE_UNSUPPORTED_MEDIA_TYPE 415
#define HTTP_RESPONSE_INTERNAL_SERVER_ERROR 500
#define HTTP_RESPONSE_NOT_IMPLEMENTED 501
#define HTTP_RESPONSE_BAD_GATEWAY 502
#define HTTP_RESPONSE_SERVICE_UNAVAILABLE 503
#define HTTP_RESPONSE_GATEWAY_TIMEOUT 504
#define HTTP_RESPONSE_HTTP_VERSION_NOT_SUPPORTED 505


#define HTTPS_PORT 443
#define HTTP_PORT 80

/*
* Class to handle https requests 
*/
class HTTPSRequest{
public:
	HTTPSRequest(String newHttpMethod = "", String newHttpResource = "", String newPayload = "");
	~HTTPSRequest();

	//return the http method that the request is using
	String getMethod();
	//return the resource uri
	String getResource();

	//set and get payload of the request
	void setPayload(String payload);
	String getPayload();

	//to specify a new header
	void setHeader(String headerName, String headerValue);

	//to remove all headers
	void removeHeaders();

	//return all the headers that the request is using
	String getRequestHeaders();

private:
	String httpMethod;
	String httpResource;
	String httpResquestHeaders;
	String httpPayload;
};

/*
* Class to handle https responses 
*/
class HTTPSResponse{
public:
	HTTPSResponse(unsigned int newHttpResponseCode = 0, String newHeaders = "", String newHttpResponseData = "");
	~HTTPSResponse();

	//return response headers
	String getResponseHeaders();
	//return http response code
	unsigned int getResponseCode();

	//return response payload
	String getPayload();
	

private:
	String httpResponseData;
	unsigned int httpResponseCode;
	String httpResponseHeaders;
};

/*
* Class to handle https conections 
*/
class HTTPS{
public:
	HTTPS(char* newHost, unsigned int newPort, char* newRootCACert, bool logger = false);
	~HTTPS();

	//return the host IP or domain
	char* getHost();

	//return the host IP or domain in string object
	String getStringHost();
	
	//return the server port
	unsigned int getPort();

	//set a specified HTTPSRequest object
	void setRequest(HTTPSRequest req);
	//return the HTTPSRequest object
	HTTPSRequest getRequest();

	//return the HTTPSResponse object
	HTTPSResponse getResponse();

	//return WiFiClientSecure object
	WiFiClientSecure getWifiClientSecure();

	//Send the specified HTTPSREQUEST to host and return the HTTPSResponse object for this request
	HTTPSResponse sendRequest();

	//Set and send a specified HTTPSREQUEST to host and return the HTTPSResponse object for this request
	HTTPSResponse sendRequest(HTTPSRequest request);

	

private:
	bool log;

	WiFiClientSecure client;

	HTTPSRequest httpsRequest;
	HTTPSResponse httpsResponse;

	char* rootCACert;

	char* host;
	unsigned int port;  
};


#endif