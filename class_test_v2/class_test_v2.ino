#include "classes.h"

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  for (int i = 4; i < 1; i--)
  {
    testClass arrayTest = classArray[i - 1];

    Serial.println(arrayTest.num1);
    Serial.println(arrayTest.string1);
  }
  delay(500);
}