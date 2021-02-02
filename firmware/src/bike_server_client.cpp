#include "Arduino.h"
#include "bike_server_client.h"

StatusOr<String> BikeServerClient::NewSession() {
    StatusOr<String> result;

    http_client.begin(wifi_client, "http://10.0.0.176:8000/api/new_session");
    result.status_code = http_client.GET();

    result.payload = http_client.getString();
    http_client.end();
    return result;
}

int BikeServerClient::AddBatch(const String& session_id, const int num_revolutions, const unsigned long start_time,
                               const unsigned long end_time) {

    char buf[300];
    snprintf(buf, sizeof(buf), "http://10.0.0.176:8000/api/add?session_id=%s&revs=%d&start_time=%lu&end_time=%lu", session_id.c_str(), num_revolutions, start_time, end_time);
    Serial.println(buf);
    http_client.begin(wifi_client, buf);
    int status = http_client.GET();
    http_client.end();
    return status;
}
