# Second capstone project HCMUT :rat:

## Introduction

Collecting and publishing real-time sensor data using MQTT message broker. Controlling device by interacting with message broker topic.

## Feature

* ![alt text][supported] &nbsp; Publishing temperature and humidity data from DHT22, Dirt huminity from analog sensor to MQTT topic.
* ![alt text][supported] &nbsp; Real-time controlling pump motor using message broker design.

[supported]: https://img.shields.io/badge/-Supported-brightgreen
[on_develop]: https://img.shields.io/badge/-On%20developing-yellow

## Prerequisite

**Microprocess board**

* Yolobit kit (ESP32)

**Library:**

* `WiFi.h`&nbsp;&nbsp;**default**
* `DHT.h`&nbsp;&nbsp;**Rob Tillaart**
* `PubSubClient`&nbsp; **default**

**Develop environment:**

* PlatformIO