#ifndef _SENSOR_ENVIRONMENTAL_H
#define _SENSOR_ENVIRONMENTAL_H

#include <Arduino.h>

#include "sensorClass.hpp"
#include <SensirionI2CSen5x.h>
#include <Wire.h>

// The used commands use up to 48 bytes. On some Arduino's the default buffer
// space is not large enough
// #define MAXBUF_REQUIREMENT 48

// #if (defined(I2C_BUFFER_LENGTH) &&                 
//      (I2C_BUFFER_LENGTH >= MAXBUF_REQUIREMENT)) || 
//     (defined(BUFFER_LENGTH) && BUFFER_LENGTH >= MAXBUF_REQUIREMENT)
// #define USE_PRODUCT_INFO
// #endif

// SensirionI2CSen5x sen5x;

#define SENSOR_ENVIRONMENTAL_I2C_ADDR 0x69

class sensorEnvironmental : public sensorClass
{
public:
    sensorEnvironmental() : sensorClass("All-In-One-Environmental"){};
    ~sensorEnvironmental(){};

    uint16_t init(uint16_t reg, bool i2c_available);
    bool connected();
    bool sample();

    enum
    {
        ENV_T = 0,
        ENV_RH = 1,
        ENV_VOC = 2, 
        ENV_PM_1 = 3, // unit : PPM
        ENV_PM_2_5 = 4, // unit : PPM
        ENV_PM_10 = 5, // unit : PPM
        MAX
    };

private:
    SensirionI2CSen5x env;
};

uint16_t sensorEnvironmental::init(uint16_t reg, bool i2c_available)
{
    uint16_t t_reg = reg;

    for (uint16_t i = 0; i < sensorEnvironmental::MAX; i++)
    {
        sensorClass::reg_t value;
        value.addr = t_reg;
        value.type = sensorClass::regType_t::REG_TYPE_S32_ABCD;
        value.value.s32 = 0;
        m_valueVector.emplace_back(value);
        t_reg += sensorClass::valueLength(value.type);
        Serial.print("Environmental t_reg++: ");
        Serial.println(t_reg);
    }

    if (!i2c_available)
    {
        _connected = false;
        return t_reg - reg;
    }
    GROVE_SWITCH_IIC;
    Wire.begin();
    Wire.beginTransmission(SENSOR_ENVIRONMENTAL_I2C_ADDR);
    if (Wire.endTransmission() != 0)
    {
        _connected = false;
        return t_reg - reg;
    }

    GROVE_SWITCH_IIC;
    env.begin(Wire);
    // env.begin(Wire, 0x10); // use the hardware I2C
    // env.changeGMXXXAddr();

    _connected = true;

    return t_reg - reg;
}

bool sensorEnvironmental::sample()
{
    GROVE_SWITCH_IIC;
    int16_t error = env.startMeasurement();
     
    if (error) {
        printf("Error executing sen5x_start_measurement(): %i\n", error);
    }

    float mass_concentration_pm1p0;
    float mass_concentration_pm2p5;
    float mass_concentration_pm4p0;
    float mass_concentration_pm10p0;
    float ambient_humidity;
    float ambient_temperature;
    float voc_index;
    float nox_index;

    env.readMeasuredValues(mass_concentration_pm1p0, mass_concentration_pm2p5, mass_concentration_pm4p0, mass_concentration_pm10p0,
                           ambient_humidity, ambient_temperature, voc_index, nox_index);

    int32_t val_PM1p0 = mass_concentration_pm1p0;
    int32_t val_PM2p5 = mass_concentration_pm2p5;
    int32_t val_PM4p0 = mass_concentration_pm4p0;
    int32_t val_PM10p0 = mass_concentration_pm10p0;
    int32_t val_VOC = voc_index;
    int32_t val_RH = ambient_humidity;
    int32_t val_T = ambient_temperature;

    m_valueVector[ENV_T].value.s32 = val_T * SCALE;
    m_valueVector[ENV_RH].value.s32 = val_RH * SCALE;
    m_valueVector[ENV_VOC].value.s32 = val_VOC * SCALE;
    m_valueVector[ENV_PM_1].value.s32 = val_PM1p0 * SCALE;
    m_valueVector[ENV_PM_2_5].value.s32 = val_PM2p5 * SCALE;
    m_valueVector[ENV_PM_10].value.s32 = val_PM10p0 * SCALE;

    return true;
}

bool sensorEnvironmental::connected()
{
    return _connected;
}

#endif