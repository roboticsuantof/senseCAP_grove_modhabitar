#ifndef _SENSOR_TEMPERATURE_H
#define _SENSOR_TEMPERATURE_H

#include "sensorClass.hpp"
#include "Seeed_MCP9808.h"
#include <Wire.h>
#include <Arduino.h>


class sensorTemperature : public sensorClass
{
public:
    sensorTemperature() : sensorClass("Seeed_MCP9808"){
            Serial.println("Building SensorTemperature");
    };

    ~sensorTemperature(){};
    uint16_t init(uint16_t reg, bool i2c_available);
    virtual bool connected();
    virtual bool sample();
    uint16_t addr;

    enum
    {
        TEMPERTURE_CM = 0x00,
        MAX
    };

private:
    MCP9808 temperature;
};

uint16_t sensorTemperature::init(uint16_t reg, bool i2c_available)
{
    uint16_t t_reg = reg;
    addr = t_reg;

    temperature.set_upper_limit(SET_UPPER_LIMIT_ADDR,0x01e0);
    delay(10);
    //Set upper limit is 32°C
    temperature.set_critical_limit(SET_CRITICAL_LIMIT_ADDR,0x0200);
    delay(10);
    //Enable the alert bit.The alert bit outputs low when the temperature value beyond limit.Otherwise stays high.
    temperature.set_config(SET_CONFIG_ADDR,0x0008);

    for (uint16_t i = 0; i < sensorTemperature::MAX; i++)
    {
        sensorClass::reg_t value;
        value.addr = t_reg;
        value.type = sensorClass::regType_t::REG_TYPE_S32_ABCD;
        value.value.s32 = 0;
        t_reg += sensorClass::valueLength(value.type);
        m_valueVector.emplace_back(value);
    }

    if (!i2c_available)
    {
        _connected = false;
        return t_reg - reg;
    }
    GROVE_SWITCH_IIC;
    Wire.begin();
    Wire.beginTransmission(SENSOR_MULTIGAS_I2C_ADDR);
    if (Wire.endTransmission() != 0)
    {
        _connected = false;
        return t_reg - reg;
    }

    GROVE_SWITCH_IIC;
    // temperature.IIC_begin(Wire, SENSOR_MULTIGAS_I2C_ADDR);
    temperature.IIC_begin();
    // temperature.begin(Wire, 0x10); // use the hardware I2C
    // temperature.changeGMXXXAddr();

    _connected = true;

    Serial.print("In method:  sensorTemperature::init()");

    return t_reg - reg;
}

bool sensorTemperature::sample()
{
    GROVE_SWITCH_ADC;

    float temp_ = 0;
    // int value = (int)temperature->read_temp_reg(addr,);
    temperature.get_temp(&temp_);
    int value = int(temp_);
    m_valueVector[TEMPERTURE_CM].value.s32 = value * SCALE;


    Serial.print("temperature temp_ is: ");
    Serial.println(temp_);
    Serial.print("temperature value is: ");
    Serial.println(value);
    delay(1000);

    return true;
}

bool sensorTemperature::connected()
{
    return _connected;
}

#endif