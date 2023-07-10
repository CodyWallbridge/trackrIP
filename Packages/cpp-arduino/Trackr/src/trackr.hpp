#pragma once

#include <WiFi.h>
#include <string>
#include <list>

namespace Trackr {
    class TrackrValue {
        public:
            TrackrValue(int, int, std::string);
            int id;
            int value;
            time_t createdAt;
    };

    class TrackrValuesResponse {
        public:
            TrackrValuesResponse(int, std::list<TrackrValue*>*);
            int totalValues;
            std::list<TrackrValue*>* values;
    };

    extern std::string apiKey;
    extern std::string apiUrl;
    extern time_t lastCallTimestamp;

    void setApiKey(std::string);
    void setApiUrl(std::string);

    Trackr::TrackrValuesResponse* getValues(int, int, int, bool, std::string key = Trackr::apiKey);
    int addSingleValue(int, int, std::string key = Trackr::apiKey);
    int addMultipleValues(int, std::list<int>, std::string key = Trackr::apiKey);
}