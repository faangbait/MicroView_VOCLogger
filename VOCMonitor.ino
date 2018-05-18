/******************************************************************************
  This code uses snippets from SparkFun. Their license is below.

  All code not covered under SparkFun's license is also released under MIT License.
  *****************************************************************************

  Marshall Taylor @ SparkFun Electronics
  Nathan Seidle @ SparkFun Electronics

  April 4, 2017

  This code is released under the [MIT License](http://opensource.org/licenses/MIT).

  Please review the LICENSE.md file included with this example. If you have any questions
  or concerns with licensing, please contact techsupport@sparkfun.com.

  Distributed as-is; no warranty is given.
******************************************************************************/

#include "SparkFunCCS811.h"
#include <MicroView.h>

#define CCS811_ADDR 0x5B //Default I2C Address
//#define CCS811_ADDR 0x5A //Alternate I2C Address

CCS811 mySensor(CCS811_ADDR);

void setup()
{
  Serial.begin(115200);
  uView.begin();
  uView.clear(PAGE);

  CCS811Core::status returnCode = mySensor.begin();
  if (returnCode != CCS811Core::SENSOR_SUCCESS)
  {
  Serial.print("beginCore exited with: ");
  switch ( returnCode )
  {
  case CCS811Core::SENSOR_SUCCESS:
    uView.println("SUCCESS");
    break;
  case CCS811Core::SENSOR_ID_ERROR:
    uView.println("ID_ERROR");
    break;
  case CCS811Core::SENSOR_I2C_ERROR:
    uView.println("I2C_ERROR");
    break;
  case CCS811Core::SENSOR_INTERNAL_ERROR:
    uView.println("INTERNAL_ERROR");
    break;
  case CCS811Core::SENSOR_GENERIC_ERROR:
    uView.println("GENERIC_ERROR");
    break;
  default:
    uView.println("UNSPEC_ERROR");
  }
    while (1); //Hang if there was a problem.
  }
  mySensor.begin();
  uView.println("20 min");
  uView.println("Burn in!");
  uView.display();
  delay(5000);
}

int loopcount=0;
int vocs;
int lastMillis;

void loop()
{
  uView.clear();
  loopcount++;
  //Check to see if data is ready with .dataAvailable()
  if (mySensor.dataAvailable())
  {
    //If so, have the sensor read and calculate the results.
    //Get them later
    mySensor.readAlgorithmResults();
    vocs = mySensor.getTVOC();
    uView.print("Mins:");
    uView.println(loopcount);
    uView.print("VOCs:");
    uView.println(vocs);
    uView.display();

    if (millis()>lastMillis+60000)
    {
      Serial.print(loopcount);
      Serial.print(",");
      Serial.println(vocs);
      lastMillis=millis();
    }
  }

  delay(100); //Don't spam the I2C bus
}

