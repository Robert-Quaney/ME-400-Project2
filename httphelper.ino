#include "httphelper.h"
//
//  Get list of audio and associated picture files
//
bool GetBaseFiles(HTTPClient &https, String picFiles[],String audioFiles[],int rowcount){
    
    String payload = https.getString();
    //Serial.println(payload);
    https.end();

    int last = payload.lastIndexOf(':');
    int idx = 0;
    int lastidx = 0;
    String filename;

    int i = 0;
    // Loop through the songs and pictures.  Play & display them.
    while (lastidx < last) 
    {
        idx = payload.indexOf(':', lastidx);
        filename = payload.substring(lastidx, payload.indexOf(".", lastidx));
        String url;
        url = filename + ".pbm";
        picFiles[i] = url;

        url = filename + ".u8";
        audioFiles[i] = url;

        lastidx = idx + 1;
        i = i+1;
    }
    return true;
}
//
// Create wifi connection using passed SSID and password
//
bool httphelper::connect(String SSID, String password, int timeout){
    int count=0;
    //.mode(WIFI_STA);
    WiFi.begin(SSID, password);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        count++;
        delay(1000);
        if (count > timeout){
            Serial.println("");
            Serial.println("Connection Failed!");
            return false;
        }
    }
    Serial.println(WiFi.localIP());
    return true;
}
//
// Create wifi connection using default SSID and password
//
bool httphelper::connect(int timeout){
    int count=0;
    //WiFi.mode(WIFI_STA);
    WiFi.begin("KSU Guest", "");
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        count++;
        delay(1000);
        if (count > timeout){
            Serial.println("");
            Serial.println("Connection Failed!");
            return false;
        }
    }
    Serial.println(WiFi.localIP());
    return true;
}
//
//  Disconnect WIFI
//
void httphelper::disconnect(){
    WiFi.disconnect();
}

bool httphelper::getURL(HTTPClient &http, String url, int timeout){
    
    // Check for wifi connection
    if ((WiFi.status() == WL_CONNECTED))
    {
        Serial.print("[HTTP] BEGIN STARTED...\n");
        String base = "https://intranet.mne.ksu.edu/static/ME400/";
        WiFiClientSecure *client = new WiFiClientSecure;

        if (client) {
            // configure traged server and url
            http.begin(url); // It's a 160x128 raw pbm, portable bitmap, type P4 -- https://en.wikipedia.org/wiki/Netpbm

            Serial.print("[HTTP] GET IN PROGRESS...\n");
            // Set the timeout
            http.setTimeout(timeout);
            // start connection and send HTTP header
            int httpCode = http.GET();

            // httpCode will be negative on error
            if (httpCode > 0)
            {
                // HTTP header has been send and Server response header has been handled
                Serial.printf("[HTTP] GET... code: %d\n", httpCode);

                // file found at server
                if (httpCode == HTTP_CODE_OK)
                {
                    return true;
                }
            }
            else
            {
                Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
            }
        }
    }
    return false;
}
