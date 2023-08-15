#include "trackr.hpp"
#include <unistd.h>
#include <HTTPClient.h>
#include <iostream>
#include <list>
#include "nlohmann/json.hpp"

size_t write_data(char *contents, size_t size, size_t nmemb, void *userp);

std::string Trackr::apiKey = "";
std::string Trackr::apiUrl = "http://wryneck.cs.umanitoba.ca";
time_t Trackr::lastCallTimestamp = 0;

//--------------------------------------------------
// TrackrValue class
//--------------------------------------------------

Trackr::TrackrValue::TrackrValue(int id, int value, std::string timestring) {
    this->id = id;
    this->value = value;
    
    struct tm tm;
    strptime(timestring.c_str(), "%Y-%m-%dT%H:%M:%S", &tm);
    this->createdAt = mktime(&tm);
}

//--------------------------------------------------
// TrackrValuesResponse class
//--------------------------------------------------

Trackr::TrackrValuesResponse::TrackrValuesResponse(int totalValues, std::list<TrackrValue*>* values) {
    this->totalValues = totalValues;
    this->values = values;
}

//--------------------------------------------------
// Namespace functions
//--------------------------------------------------

void Trackr::setApiKey(std::string key) {
    Trackr::apiKey = key;
}

void Trackr::setApiUrl(std::string url) {
    Trackr::apiUrl = url;
}

Trackr::TrackrValuesResponse* Trackr::getValues(int fieldId, int offset, int limit, bool descending, std::string key) {
    HTTPClient http;

    // wait until current time is at least 1s after last call
    time_t now = time(NULL);
    if (now < Trackr::lastCallTimestamp + 1) {
        sleep(Trackr::lastCallTimestamp + 1 - now);
    }
    
    std::string opts = "?fieldId=" + std::to_string(fieldId) + "&offset=" + std::to_string(offset) + "&limit=" + std::to_string(limit) + "&order=" + (descending ? "desc" : "asc") + "&apiKey=" + key;
    
    http.begin((Trackr::apiUrl + "/api/values" + opts).c_str());
    http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
    int http_code = http.GET();
    
    if (http_code == 200) {
        nlohmann::json j = nlohmann::json::parse(http.getString());
        std::list<TrackrValue*>* values = new std::list<TrackrValue*>();
        for (nlohmann::json::iterator it = j["values"].begin(); it != j["values"].end(); ++it) {
            values->push_back(new TrackrValue((*it)["id"], stoi((*it)["value"].get_ref<std::string&>()), (*it)["createdAt"]));
        }
        TrackrValuesResponse* response = new TrackrValuesResponse(j["totalValues"], values);
        return response;
    }

    Trackr::lastCallTimestamp = time(NULL);

    return nullptr;
}

int Trackr::addSingleValue(int fieldId, int value, std::string key) {
    HTTPClient http;

    // wait until current time is at least 1s after last call
    time_t now = time(NULL);
    if (now < Trackr::lastCallTimestamp + 1) {
        sleep(Trackr::lastCallTimestamp + 1 - now);
    }
    
    std::string opts = "fieldId=" + std::to_string(fieldId) + "&value=" + std::to_string(value) + "&apiKey=" + key;
    
    http.begin((Trackr::apiUrl + "/api/values/").c_str());
    http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int http_code = http.POST(opts.c_str());

    Trackr::lastCallTimestamp = time(NULL);

    return http_code;
}


int Trackr::addMultipleValues(int fieldId, std::list<int> values, std::string key) {
    int rc;

    for (int i : values) {
        rc = Trackr::addSingleValue(fieldId, i, key);
        if (rc < 200 || rc > 299) {
            return rc;
        }
    }

    return rc;
}