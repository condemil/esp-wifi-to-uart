#pragma once

#include <DNSServer.h>
#include <Syslog.h>
#include <WiFiManager.h>

#include "config.hpp"
#include "elapsedMillis.hpp"
#include "logger.hpp"

namespace wifi {
WiFiManagerParameter _custom_syslog_host("syslog_host", "SYSLOG Server", "", 40);
WiFiManagerParameter _custom_syslog_port("syslog_port", "SYSLOG Port", "514", 8);
WiFiManagerParameter _custom_ota_token("ota_token", "OTA Token", "", 40);

elapsedMillis _reconnect_time_elapsed;
const unsigned int RECONNECT_DELAY = 5000;

void _saveConfigCallback() {
    logger::debugln(F("wifi: connection established, set network mode to provisioned"));
    config::conf.provisioned = true;
    strcpy(config::conf.syslog_host, _custom_syslog_host.getValue());
    config::conf.syslog_port = atol(_custom_syslog_port.getValue());
    strcpy(config::conf.ota_token, _custom_ota_token.getValue());
    config::persist();
}

void _connect() {
    WiFi.begin();

    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        logger::errorln(F("wifi: connection failed"));
    }

    logger::debugf("wifi: connected, ip address: %s\n", WiFi.localIP().toString().c_str());
}

void setup() {
    if (!config::conf.provisioned) {
        logger::debugln(F("wifi: creating AP for wifi provisioning, connect to http://192.168.4.1"));
        WiFiManager wifiManager;

        wifiManager.setDebugOutput(false);
        wifiManager.setSaveConfigCallback(_saveConfigCallback);

        wifiManager.addParameter(&_custom_syslog_host);
        wifiManager.addParameter(&_custom_syslog_port);
        wifiManager.addParameter(&_custom_ota_token);

        wifiManager.startConfigPortal(config::HOSTNAME);
        return;
    }

    WiFi.mode(WIFI_STA);
    _connect();
}

void handle() {
    if (WiFi.status() == WL_CONNECTED)
        return;

    if (_reconnect_time_elapsed >= RECONNECT_DELAY) {
        _reconnect_time_elapsed = 0; // reset timer
        logger::errorln(F("wifi: connection lost, reconnecting"));
        _connect();
    }
}
} // namespace wifi
