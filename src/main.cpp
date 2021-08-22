#include <Arduino.h>

#include "bridge.hpp"
#include "button.hpp"
#include "config.hpp"
#include "logger.hpp"
#include "ota.hpp"
#include "wifi.hpp"

Button button;

void onButtonClick();

void setup() {
    pinMode(1, OUTPUT); // GPIO 1 is used as TX
    logger::setup();
    logger::debugln(F("\nmain: started"));

    config::setup();
    wifi::setup();

    if (strlen(config::conf.syslog_host) != 0) {
        logger::setupSyslog(config::conf.syslog_host, config::conf.syslog_port, config::HOSTNAME, config::NAME);
    }

    ota::setup();
    button.setup(ButtonType::pullup, config::IO_BUTTON, onButtonClick);
    bridge::setup();

    logger::debugln(F("main: setup is over"));
}

void loop() {
    wifi::handle();
    ota::handle();
    button.handle();
    bridge::handle();
}

void onButtonClick() {
    config::truncate();
    ESP.restart();
}
