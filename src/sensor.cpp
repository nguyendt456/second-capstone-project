#include <main.h>

void ReadSensors(float& temperature , float& humidity, float& dirt_humidity) {
    dirt_humidity = float(analogRead(MOI_SENSOR)) / 3150.0 * 100;

    DHT.read();
    temperature = float(DHT.getTemperature());
    humidity = float(DHT.getHumidity());
}