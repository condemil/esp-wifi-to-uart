#pragma once

#include <ArduinoJson.h>
#include <LittleFS.h>

#include "logger.hpp"

namespace config {
static const uint8_t IO_BUTTON = 0;

static const char *NAME = "uart-bridge";

static char HOSTNAME[15];
static const uint16_t SERVER_PORT = 2217;
static const uint32_t UART_BAUDRATE = 115200;

struct Config {
    char syslog_host[40];
    uint16_t syslog_port;
    char ota_token[40];
    bool provisioned;
};

Config conf;
static const char *_filename = "/config.json";

void setup() {
    sprintf(HOSTNAME, "esp8266-%06x", ESP.getChipId());

    if (!LittleFS.begin()) {
        logger::errorln(F("config: failed to mount FS"));
        return;
    }

    File file = LittleFS.open(_filename, "r");

    if (!file) {
        logger::errorln(F("config: failed to open file to read"));
        return;
    }

    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, file);

    if (error) {
        logger::errorln(F("config: failed to deserialize"));
        file.close();
        return;
    }

    conf.provisioned = doc["provisioned"];
    strlcpy(conf.syslog_host, doc["syslog_host"], sizeof(conf.syslog_host));
    conf.syslog_port = doc["syslog_port"];
    strlcpy(conf.ota_token, doc["ota_token"], sizeof(conf.ota_token));

    file.close();
}

void persist() {
    File file = LittleFS.open(_filename, "w");

    if (!file) {
        logger::errorln(F("config: failed to open file to write"));
        return;
    }

    StaticJsonDocument<512> doc;

    doc["provisioned"] = conf.provisioned;
    doc["syslog_host"] = conf.syslog_host;
    doc["syslog_port"] = conf.syslog_port;
    doc["ota_token"] = conf.ota_token;

    if (serializeJson(doc, file) == 0) {
        logger::errorln(F("config: failed to write file"));
    }

    file.close();
    logger::debugln(F("config: persisted"));
}

void truncate() {
    conf.provisioned = false;
    conf.syslog_host[0] = (char)0;
    conf.syslog_port = 514;
    conf.ota_token[0] = (char)0;
    persist();
}
} // namespace config
