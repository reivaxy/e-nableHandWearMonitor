#include "Utils.h"
#include <Arduino.h>

void Utils::checkHeap(const char* msg) {
    uint32_t freeMem = system_get_free_heap_size();
    DebugPrintf("Heap size '%s': %d\n", msg, freeMem); 
}

int Utils::toInt(const char* str, int length) {
   int result = 0;
   for(int i = 0; i < length; i++) {
      result += pow(10, (length - 1 - i)) * (str[i] - 48);
   }
   DebugPrintf("Converted first %d chars from '%s' to %d\n", length, str, result);
   return result;
}