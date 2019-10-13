#include "Utils.h"
#include <Arduino.h>

void Utils::checkHeap(const char* msg) {
    uint32_t freeMem = system_get_free_heap_size();
    DebugPrintf("Heap size '%s': %d\n", msg, freeMem); 
}