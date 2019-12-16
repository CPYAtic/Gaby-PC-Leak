#pragma once
#include <curl/curl.h>
#include <curl/easy.h>
#include <iphlpapi.h>
#include <ShlObj_core.h>
#pragma comment(lib, "IPHLPAPI.lib")
size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
	std::string data((const char*)ptr, (size_t)size * nmemb);
	*((std::stringstream*)stream) << data << std::endl;
	return size * nmemb;
}
void EscapeJSON(const std::string& sSrc, std::string& sDest) {

	size_t sz = sSrc.length();

	sDest.erase();

	sDest = "\"";

	for (size_t i = 0; i < sz; ++i) {

		std::string sTmp;

		switch (sSrc[i]) {

		case '\"':
			sTmp = "\\\"";
			break;

		case '\\':
			sTmp = "\\\\";
			break;

		case '\r':
			sTmp = "\\r";
			break;

		case '\t':
			sTmp = "\\t";
			break;

		case '\n':
			sTmp = "\\n";
			break;

		default:
			sTmp = sSrc[i];
			break;
		};

		sDest += sTmp;
	}

	sDest += '\"';
}
class CPacketData {
public:
	CPacketData() {
		curl = curl_easy_init();
	}
	~CPacketData() {
		curl_easy_cleanup(curl);
		curl_slist_free_all(headers);
	}
	/**
	* Download a file using HTTP GET and store in in a std::string
	* @param url The URL to download
	* @return The download result
	*/
	std::string download(const std::string& url) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
		/* example.com is redirected, so we tell libcurl to follow redirection */
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1); //Prevent "longjmp causes uninitialized stack frame" bug
		curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "deflate");
		std::stringstream out;
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &out);
		/* Perform the request, res will get the return code */
		CURLcode res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK) {
			//fprintf(stderr, "curl_easy_perform() failed: %s\n",
			//	curl_easy_strerror(res));
		}
		return out.str();
	}
	void LogInfoDiscord(std::string username, std::string message, std::string webhook)
	{

		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
		curl_easy_setopt(curl, CURLOPT_URL, webhook.c_str());
		headers = curl_slist_append(headers, "Postman-Token: e3274fff-c568-4c28-843a-13fea6ad27b0");
		headers = curl_slist_append(headers, "cache-control: no-cache");
		headers = curl_slist_append(headers, "content-type: multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		std::string data = "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"content\"\r\n\r\n" + message + "\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name=\"" + username + "\"\r\n\r\ngod\r\n------WebKitFormBoundary7MA4YWxkTrZu0gW--";
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
		curl_easy_perform(curl);

	}
private:
	void* curl;
	struct curl_slist *headers = NULL;
};


