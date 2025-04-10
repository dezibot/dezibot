#include <Dezibot.h>
#include <arduinoFFT.h>
#include <rom/ets_sys.h>
const uint16_t samples = 256; //This value MUST ALWAYS be a power of 2
const int centeredThreshold = 50 ;
//const float signalFrequency = 1000;
const float samplingFrequency = 4000;
float vReal[4][samples];
float vImag[4][samples];
#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

ArduinoFFT<float> FFT = ArduinoFFT<float>(vReal[0], vImag[0], samples, samplingFrequency); /* Create FFT object */

Dezibot dezibot = Dezibot();
void setup() {
  dezibot.begin();
  Serial.begin(115200);
  //dezibot.infraredLight.front.turnOn();
  //dezibot.infraredLight.bottom.turnOn();
}

void loop() {
  portDISABLE_INTERRUPTS();
  for(int i = 0; i < samples; i++){
    vReal[0][i] = dezibot.lightDetection.getValue(IR_FRONT);
    vImag[0][i] = 0.0;
    vReal[1][i] = dezibot.lightDetection.getValue(IR_LEFT);
    vImag[1][i] = 0.0;
    vReal[2][i] = dezibot.lightDetection.getValue(IR_RIGHT);
    vImag[2][i] = 0.0;
    vReal[3][i] = dezibot.lightDetection.getValue(IR_BACK);
    vImag[3][i] = 0.0;
    ets_delay_us(125);
  }
  
  portENABLE_INTERRUPTS();
  //PrintVector(vReal, (samples>>1), 0);
  
  //PrintVector(vReal, (samples>>1), 0);
  float frequency[4];
  float magnitude[4];
  for(int index = 0; index <4; index++){
    FFT.setArrays(vReal[index], vImag[index]);
    FFT.windowing(FFTWindow::Rectangle, FFTDirection::Forward);	/* Weigh data */
    FFT.compute(FFTDirection::Forward); /* Compute FFT */
    FFT.complexToMagnitude(); /* Compute magnitudes */
    FFT.majorPeak(&frequency[index],&magnitude[index]);
    if(abs(frequency[index]-1147)>10){
      magnitude[index] = 0;
    }
    Serial.print(index);
    Serial.print(":");
    Serial.print(frequency[index]);
    Serial.print(",");
    Serial.print(index+4);
    Serial.print(":");
    Serial.print(magnitude[index]);
    if(index < 3){
      Serial.print(",");
    }
    Serial.println();
  }

//================================================
float leftValue = magnitude[1];
float rightValue = magnitude[2]; 
switch(brightest(magnitude)){
    case IR_FRONT:
      //correct Stearing to be centered
      if( abs(leftValue-rightValue)
          < centeredThreshold){
            dezibot.motion.move();
          }else{
            if (leftValue > rightValue){
              dezibot.motion.rotateAntiClockwise();
            } else{
              dezibot.motion.rotateClockwise();
            }
          }
      dezibot.multiColorLight.setTopLeds(BLUE);
      break;
    case IR_LEFT:
      dezibot.motion.rotateAntiClockwise();
      dezibot.multiColorLight.setTopLeds(RED);
      break;
    case IR_RIGHT:
      dezibot.motion.rotateClockwise();
      dezibot.multiColorLight.setTopLeds(GREEN);
      break;
    case IR_BACK:
      if(leftValue > rightValue){
        dezibot.motion.rotateAntiClockwise();
      } else {
        dezibot.motion.rotateClockwise();
      }
      dezibot.multiColorLight.setTopLeds(YELLOW);
      break;
  }






}

photoTransistors brightest(float *magnitudes){
  int pos;
  float maxMagnitude = 0; 
  for(int index = 0; index <4; index++){
    if (magnitudes[index] > maxMagnitude){
      pos = index;
      maxMagnitude = magnitudes[index];
    }
  }
  switch (pos) {
    case 0:
      return IR_FRONT;
    case 1: 
      return IR_LEFT;
    case 2: 
      return IR_RIGHT;
    case 3:
      return IR_BACK;
  }
}

void PrintVector(float *vData, uint16_t bufferSize, uint8_t scaleType)
{
  for (uint16_t i = 0; i < bufferSize; i++)
  {
    float abscissa;
    /* Print abscissa value */
    switch (scaleType)
    {
      case SCL_INDEX:
        abscissa = (i * 1.0);
	break;
      case SCL_TIME:
        abscissa = ((i * 1.0) / samplingFrequency);
	break;
      case SCL_FREQUENCY:
        abscissa = ((i * 1.0 * samplingFrequency) / samples);
	break;
    }
    Serial.print(abscissa, 6);
    if(scaleType==SCL_FREQUENCY)
      Serial.print("Hz");
    Serial.print(" ");
    Serial.println(vData[i], 4);
  }
  Serial.println();
}
