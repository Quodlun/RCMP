#include "cpp.h"
#include "cpp.cpp"

cpp cpp;

void setup ()
{
  Serial.begin ( 115200 );
}

void loop ()
{
  cpp.cppTest ( "Test" );
}

/*
c:/users/quodl/appdata/local/arduino15/packages/esp32/tools/esp-x32/2302/bin/../lib/gcc/xtensa-esp32-elf/12.2.0/../../../../xtensa-esp32-elf/bin/ld.exe: C:\Users\quodl\AppData\Local\arduino\sketches\EEB7BB95F84CDBE1F4E38C54431B8B9F\sketch\cpp_test.ino.cpp.o: in function `cpp::cppTest(String)':
D:\repos\arduino\rcmp\cpp_test/cpp.cpp:4: multiple definition of `cpp::cppTest(String)'; C:\Users\quodl\AppData\Local\arduino\sketches\EEB7BB95F84CDBE1F4E38C54431B8B9F\sketch\cpp.cpp.o:D:\repos\arduino\rcmp\cpp_test/cpp.cpp:4: first defined here
collect2.exe: error: ld returned 1 exit status
*/