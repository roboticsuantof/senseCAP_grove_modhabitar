#ifndef _SENSOR_AC_H
#define _SENSOR_AC_H

#include "sensorClass.hpp"

#define AC_ADC_PIN A2
#define ADC_BITS 12
#define ADC_COUNTS (1<<ADC_BITS)

class sensorWaterFlow : public sensorClass
{
  public:
  sensorWaterFlow(): sensorClass("Sensor-Flow"){};
  ~sensorWaterFlow(){};

  uint16_t init(uint16_t reg, bool i2c_available);
  bool connected();
  bool sample();

  enum
  {
    AC = 0,
    MAX
  };
  private:
    double voltCal = 523.56;
    double phaseCal = 1.7;
    unsigned int cycles = 20;
    unsigned int timeout = 2000;
    int SupplyVoltage = 3300;
    int sampleV;
    double lastFilteredV,filteredV;
    double offsetV = ADC_COUNTS >> 1;
    double phaseShiftedV;
    double sqV,sumV;
    int startV;
    boolean lastVCross,checkVCross;
};

uint16_t sensorWaterFlow::init(uint16_t reg, bool i2c_available){
  uint16_t t_reg = reg; 

  for (uint16_t i = 0; i < sensorWaterFlow::MAX; i++)
    {
        sensorClass::reg_t value;
        value.addr = t_reg;
        value.type = sensorClass::regType_t::REG_TYPE_S32_ABCD;
        value.value.s32 = 0;
        t_reg += sensorClass::valueLength(value.type);
        m_valueVector.emplace_back(value);
    }

    _connected = i2c_available ? false : true;
    //_connected = true;
    return t_reg - reg;
}

bool sensorWaterFlow::sample()
{
    
  GROVE_SWITCH_ADC;
  pinMode(AC_ADC_PIN, INPUT);
  
  unsigned int crossCount = 0;
  unsigned int numberOfSamples = 0;
  
  unsigned long start = millis();
  
  while(1){
    int startV = analogRead(AC_ADC_PIN);
    if((startV<(ADC_COUNTS*0.51)) && (startV>(ADC_COUNTS*0.49)))
        break;
    if((millis()-start)>timeout)    
        break;
  }
  
  start = millis();
  
  while((crossCount<cycles) && ((millis()-start)<timeout))
  {
    numberOfSamples++;
    lastFilteredV = filteredV;
    
    sampleV = analogRead(AC_ADC_PIN);
    offsetV = offsetV + ((sampleV - offsetV)/ADC_COUNTS);
    filteredV = sampleV - offsetV;
    
    sqV = filteredV * filteredV;
    sumV += sqV;
    
    phaseShiftedV = lastFilteredV + phaseCal * (filteredV - lastFilteredV);
    
    lastVCross = checkVCross;
    if(sampleV>startV)
        checkVCross = true;
    else 
        checkVCross = false;
    if(numberOfSamples == 1)
        lastVCross = checkVCross;
    if(lastVCross !=checkVCross)
        crossCount++;
  }
  
  double V_RATIO = voltCal * ((SupplyVoltage/1000.0)/(ADC_COUNTS));
  float value = V_RATIO * sqrt(sumV/numberOfSamples);
  m_valueVector[AC].value.s32 = value * SCALE;
  //Serial.println(value);
  sumV = 0; 
  
  return true;
}

bool sensorWaterFlow::connected()
{
  return _connected;
}

#endif