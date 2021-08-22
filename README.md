# ESP8266 Wi-Fi to UART bridge

Wi-Fi bridge to the remote device over TCP

```
   -------         -------          --------
  |       |       |       |        |        |
  | Local |  TCP  |  ESP  |  UART  | Remote |
  | Linux | ----- |  8266 | ------ | Device |
  |       |       |       |        |        |
   -------         -------          --------
```

## How to use

- Flash the firmware to esp8266
- Connect esp8266 rx/tx/gnd pins to the remote device
- Connect to esp8266 wifi access point named esp8266-XXXXXX and and set it up
- Use socat on local linux machine to create a virtual tty that is connected to esp8266

socat command example below, replace ESP-IP

```
socat pty,link=/dev/virtual0 tcp:ESP-IP:2217
```

# TODO

- Configurable port, for now it is hardcoded to `2217` in `config.hpp`
- Configurable baudrate, for now it is hardcoded to `115200` in `config.hpp`
