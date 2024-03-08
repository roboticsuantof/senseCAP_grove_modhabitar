// #ifndef _SENSOR_LOUDNESS_H
// #define _SENSOR_LOUDNESS_H

// #include "sensorClass.hpp"
// #include "Loudness.h"

// #include <Arduino.h>
// // #include <ArduinoRS485.h>
// // #include <ArduinoModbus.h>

// class sensorLoudness : public sensorClass
// {
// public:
//     sensorLoudness(uint8_t pin = SENSOR_ANALOG_PIN) : sensorClass("Loudness")
//     {
//         loudness = new Loudness(pin);
//             Serial.print("pin: ");
//             Serial.println(pin);
//     };
//     ~sensorLoudness(){};
//     uint16_t init(uint16_t reg, bool i2c_available);
//     bool connected();
//     bool sample();

//     enum
//     {
//         LOUD = 0x00, // unit : 
//         MAX
//     };

// private:
//     Loudness *loudness;
// };

// uint16_t sensorLoudness::init(uint16_t reg, bool i2c_available)
// {
//     uint16_t t_reg = reg;
//     for (uint16_t i = 0; i < sensorLoudness::MAX; i++)
//     {
//         sensorClass::reg_t value;
//         value.addr = t_reg;
//         value.type = sensorClass::regType_t::REG_TYPE_S32_ABCD;
//         value.value.s32 = 0;
//         t_reg += sensorClass::valueLength(value.type);
//         m_valueVector.emplace_back(value);
//     }

//     _connected = i2c_available ? false : true;
//         Serial.print("sensorLoudness::init val_loud: ");
//         Serial.println(t_reg);
//         Serial.print("sensorLoudness::init reg: ");
//         Serial.println(reg);
//         Serial.print("sensorLoudness::init t_reg - reg: ");
//         Serial.println(t_reg - reg);

//     return t_reg - reg;
// }

// bool sensorLoudness::sample()
// {
//     //GROVE_SWITCH_IIC;
//     GROVE_SWITCH_ADC;
//     /*pinMode(GROVE_SWITCH_PIN, OUTPUT);
//     digitalWrite(GROVE_SWITCH_PIN, HIGH);
//     delay(2);
//     pinMode(SENSOR_ANALOG_PIN, INPUT);
//     pinMode(SENSOR_ANALOG_E_PIN, INPUT);
//     delay(10);
//     */
//     //
//     long val_loud = (long)loudness->MeasureLoud();    // int32_t val_loud = loud.measure_();
    
//     m_valueVector[LOUD].value.s32 = val_loud * SCALE;
//         Serial.print("sensorLoudness::sample val_loud: ");
//         Serial.println(val_loud);

//     // m_valueVector[LOUD].value.s32 = val_loud;

//     return true;
// }

// bool sensorLoudness::connected()
// {
//     return _connected;
// }

// #endif