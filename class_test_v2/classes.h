#ifndef CLASSES_H
#define CLASSES_H

struct testClass
{
public:
  int num1;
  String string1;

  testClass(int num1, String string1) : num1(num1), string1(string1) {}
};

testClass classArray[] =
{
  testClass(1, "Test 1"),
  testClass(2, "Test 2"),
  testClass(3, "Test 3"),
  testClass(4, "Test 4")
};

#endif