/* Edge Impulse Arduino examples
 * Copyright (c) 2021 EdgeImpulse Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 Adapted from
 Edge Impulse Arduino examples,
 https://www.okdo.com/getting-started/get-started-with-arduino-nano-33-ble/#h-3-ble-mobile-app-toc,
 and examples from from Arduino BLE library
*/

#include <boxing-moves_inferencing.h>
#include <Arduino_LSM9DS1.h>
#include <ArduinoBLE.h>

String greeting = "Hello";

BLEService greetingService("180C");  // User defined service

BLEStringCharacteristic greetingCharacteristic("2A56",  // standard 16-bit characteristic UUID
    BLERead| BLENotify, 13); // remote clients will only be able to read this



#define CONVERT_G_TO_MS2    9.80665f
#define MAX_ACCEPTED_RANGE  2.0f        
static bool debug_nn = false; 
int countCross=0;
int countHook=0;
int countJab=0;
int countReady=0;
int countUppercut=0;

void setup()
{
    
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LEDR, OUTPUT);
    pinMode(LEDG, OUTPUT);
    pinMode(LEDB, OUTPUT);
      
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println("Edge Impulse Inferencing Demo");

    if (!IMU.begin()) {
        ei_printf("Failed to initialize IMU!\r\n");
    }
    else {
        ei_printf("IMU initialized\r\n");
    }

    if (EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME != 3) {
        ei_printf("ERR: EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME should be equal to 3 (the 3 sensor axes)\n");
        return;
  }

    if (!BLE.begin()) {   // initialize BLE
      Serial.println("starting BLE failed!");
    while (1);
  }
  BLE.setLocalName("Nano33BLE");  // Set name for connection
  BLE.setAdvertisedService(greetingService); // Advertise service
  greetingService.addCharacteristic(greetingCharacteristic); // Add characteristic to service
  BLE.addService(greetingService); // Add service
  greetingCharacteristic.writeValue(greeting); // Set greeting string

  BLE.advertise();  // Start advertising
  Serial.print("Peripheral device MAC: ");
  Serial.println(BLE.address());
  Serial.println("Waiting for connections...");    
  
}

void ei_printf(const char *format, ...) {
   static char print_buf[1024] = { 0 };

   va_list args;
   va_start(args, format);
   int r = vsnprintf(print_buf, sizeof(print_buf), format, args);
   va_end(args);

   if (r > 0) {
       Serial.write(print_buf);
   }
}


float ei_get_sign(float number) {
    return (number >= 0.0) ? 1.0 : -1.0;
}

void loop()
{   
    

    BLEDevice central = BLE.central();  // Wait for a BLE central to connect
  
    ei_printf("\nStarting inferencing in 1 seconds...\n");

    delay(1000);

    ei_printf("Sampling...\n");

    float buffer[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE] = { 0 };

    for (size_t ix = 0; ix < EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE; ix += 3) {
        uint64_t next_tick = micros() + (EI_CLASSIFIER_INTERVAL_MS * 1000);

        IMU.readAcceleration(buffer[ix], buffer[ix + 1], buffer[ix + 2]);

        for (int i = 0; i < 3; i++) {
            if (fabs(buffer[ix + i]) > MAX_ACCEPTED_RANGE) {
                buffer[ix + i] = ei_get_sign(buffer[ix + i]) * MAX_ACCEPTED_RANGE;
            }
        }

        buffer[ix + 0] *= CONVERT_G_TO_MS2;
        buffer[ix + 1] *= CONVERT_G_TO_MS2;
        buffer[ix + 2] *= CONVERT_G_TO_MS2;

        delayMicroseconds(next_tick - micros());
    }

    signal_t signal;
    int err = numpy::signal_from_buffer(buffer, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
    if (err != 0) {
        ei_printf("Failed to create signal from buffer (%d)\n", err);
        return;
    }

    ei_impulse_result_t result = { 0 };

    err = run_classifier(&signal, &result, debug_nn);
    if (err != EI_IMPULSE_OK) {
        ei_printf("ERR: Failed to run classifier (%d)\n", err);
        return;
    }

    ei_printf("Predictions ");
    ei_printf("(DSP: %d ms., Classification: %d ms., Anomaly: %d ms.)",
        result.timing.dsp, result.timing.classification, result.timing.anomaly);
    ei_printf(": \n");
    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        ei_printf("    %s: %.5f\n", result.classification[ix].label, result.classification[ix].value);
    }
#if EI_CLASSIFIER_HAS_ANOMALY == 1
    ei_printf("    anomaly score: %.3f\n", result.anomaly);
#endif

    if (result.classification[0].value >= 0.6){
      countCross++;
          digitalWrite(LED_BUILTIN, HIGH);
          digitalWrite(LEDR, HIGH);
          digitalWrite(LEDG, HIGH);
          digitalWrite(LEDB, HIGH);
      greeting = "Cross";         
      } else if (result.classification[1].value >= 0.6){
      countHook++;
          digitalWrite(LEDR, LOW);
          digitalWrite(LED_BUILTIN, LOW);
          digitalWrite(LEDG, HIGH);
          digitalWrite(LEDB, HIGH);  
      greeting = "Hook";      
      } else if (result.classification[2].value >= 0.6){
      countJab++;
          digitalWrite(LEDG, LOW);
          digitalWrite(LED_BUILTIN, LOW);
          digitalWrite(LEDR, HIGH);
          digitalWrite(LEDB, HIGH);      
      greeting = "Jab";  
      } else if (result.classification[3].value >= 0.6){
      countReady++;
          digitalWrite(LEDG, LOW);
          digitalWrite(LED_BUILTIN, LOW);
          digitalWrite(LEDR, LOW);
          digitalWrite(LEDB, LOW);       
      greeting = "Ready";  
      } else if (result.classification[4].value >= 0.6){
      countUppercut++;
          digitalWrite(LEDB, LOW);
          digitalWrite(LED_BUILTIN, LOW);
          digitalWrite(LEDR, HIGH);
          digitalWrite(LEDG, HIGH);      
      greeting = "Uppercut";  
      };
      Serial.println("\r\n");
      Serial.print("Cross count: ");
      Serial.println(countCross);
      Serial.print("Hook count: ");
      Serial.println(countHook);
      Serial.print("Jab count: ");
      Serial.println(countJab);
      Serial.print("Ready count: ");
      Serial.println(countReady);
      Serial.print("Uppercut count: ");
      Serial.println(countUppercut);
      Serial.println("\r\n");
      Serial.print("total number of punches: ");
      Serial.println(countCross+countHook+countJab+countUppercut);      
      
      greetingCharacteristic.writeValue(greeting); // Set greeting strin
}

#if !defined(EI_CLASSIFIER_SENSOR) || EI_CLASSIFIER_SENSOR != EI_CLASSIFIER_SENSOR_ACCELEROMETER
#error "Invalid model for current sensor"
#endif
