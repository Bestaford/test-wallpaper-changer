#include <stdlib.h>
#include <stdio.h>
#include "curl/curl.h"

#define BUFSIZE 1024
#define FILENAME "tempwallpaper.jpg"
#define URL "https://cdn.britannica.com/39/7139-050-A88818BB/Himalayan-chocolate-point.jpg"

int main() {
	//global init
	curl_global_init(CURL_GLOBAL_DEFAULT);

	//retrieve temp path
	char path_buffer[BUFSIZE];
	GetTempPath(BUFSIZE - sizeof(FILENAME), path_buffer);
	strcat(path_buffer, FILENAME);

	//open file for writing image
	FILE* fp = fopen(path_buffer, "wb");
	if (!fp) {
		fprintf(stderr, "Failed to open file %s\n", path_buffer);
		exit(EXIT_FAILURE);
	}
	
	//initialize cURL
	CURL* curl = curl_easy_init();
	if (!curl) {
		fprintf(stderr, "curl_easy_init() failed\n");
		exit(EXIT_FAILURE);
	}

	//set URL and file stream
	curl_easy_setopt(curl, CURLOPT_URL, URL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

	//perform request for image downloading
	printf("Saving image from %s to %s\n", URL, path_buffer);
	CURLcode result = curl_easy_perform(curl);

	//check request result
	if (result != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
		exit(EXIT_FAILURE);
	}

	//cleanup
	curl_easy_cleanup(curl);
	curl_global_cleanup();
	fclose(fp);

	//set downloaded image as wallpaper
	SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, path_buffer, SPIF_SENDCHANGE);

	printf("Done!\n");

	return EXIT_SUCCESS;
}