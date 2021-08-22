#pragma once

#include <Arduino.h>
#include <WiFiClient.h>
#include <WiFiServer.h>

#include "config.hpp"

#define min(a,b) ((a)<(b)?(a):(b))

namespace bridge {
WiFiServer _tcp_server(config::SERVER_PORT);
WiFiClient _tcp_client;

uint16_t _bytes_available;
uint16_t _bytes_read;
uint8_t _buf[1024];

void setup() {
    _tcp_server.begin();
    _tcp_server.setNoDelay(true);
    Serial.begin(config::UART_BAUDRATE);

    while(Serial.available()) {
      Serial.read(); // empty buffer
    }
}

void handle() {
    if (_tcp_server.hasClient()) { // new connection from client
        if (_tcp_client) {
            _tcp_server.stop(); // disconnect existing client
        }

        logger::debugln(F("bridge: new tcp client"));
        _tcp_client = _tcp_server.available();
    }

    if (!_tcp_client) {
        return;
    }

    if (!_tcp_client.connected()) {
        return;
    }

    // send data UART -> TCP
    while ((_bytes_available = Serial.available()) > 0) {
        _bytes_read = Serial.readBytes(_buf, min(sizeof(_buf), _bytes_available));

        if (_bytes_read == 0) {
            break;
        }

        _tcp_client.write((uint8_t*)_buf, _bytes_read);
    }

    // send data TCP -> UART
    while ((_bytes_available = _tcp_client.available()) > 0) {
        _bytes_read = _tcp_client.readBytes(_buf, min(sizeof(_buf), _bytes_available));

         if (_bytes_read == 0) {
             break;
         }

         Serial.write(_buf, _bytes_read);
         Serial.flush();
     }
}
} // namespace bridge
