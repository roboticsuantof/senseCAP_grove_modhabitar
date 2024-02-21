#ifndef _SENSOR_TEMPERATURE_H
#define _SENSOR_TEMPERATURE_H

#include "sensorClass.hpp"
#include "Seeed_MCP9808.h"
#include <Wire.h>

// #define MSL 102009 // Mean Sea Level in Pa

class sensorTemperature : public sensorClass
{
public:
    sensorTemperature() : sensorClass("Temperature"){
        // Serial.println("Initialazing  sensorTemperature class");
    };
    ~sensorTemperature(){};

    uint16_t init(uint16_t reg, bool i2c_available);
    bool connected();
    bool sample();

    enum
    {
        TEMPERATURE,
        MAX
    };

private:
    MCP9808 temperature; // IIC
};

uint16_t sensorTemperature::init(uint16_t reg, bool i2c_available)
{
    uint16_t t_reg = reg;

    for (uint16_t i = 0; i < sensorTemperature::MAX; i++)
    {
        sensorClass::reg_t value;
        value.addr = t_reg;
        value.type = sensorClass::regType_t::REG_TYPE_S32_ABCD;
        value.value.s32 = 0;
        m_valueVector.emplace_back(value);
        t_reg += sensorClass::valueLength(value.type);
    }

    if (!i2c_available)
    {
        _connected = false;
        return t_reg - reg;
    }
    GROVE_SWITCH_IIC;
    Wire.begin();
    Wire.beginTransmission(DEFAULT_IIC_ADDR);
    if (Wire.endTransmission() != 0)
    {
        _connected = false;
        return t_reg - reg;
    }
    // _connected = temperature.init();
    _connected = true;
    
// Serial.print("sensorTemperature::init   ,   _connected =");
// Serial.println(_connected);

    return t_reg - reg;
}

bool sensorTemperature::sample()
{
    GROVE_SWITCH_IIC;
    float temp_ = 0;
    temperature.get_temp(&temp_);

    m_valueVector[TEMPERATURE].value.s32 = temp_ * SCALE;

    return true;
}

bool sensorTemperature::connected()
{
    return _connected;
}

#endif