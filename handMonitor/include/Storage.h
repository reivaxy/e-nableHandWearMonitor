/* 
 *  =============================================================================================================================================
 *  Project : Hand Monitoring e-Nable France
 *  Author  : Xavier Grosjean & Thomas Broussard
 * 
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description : Hand monitor
 * 
 * =============================================================================================================================================
 */

#pragma once

#include "FS.h"

#define LAST_RTC_ADDR 124  // address of last 4 bytes of 512: (512/4)-4 

class Storage {
public:
   static void recordStateChange(int previousState);
   static void listFiles(char *list, int size);
};

