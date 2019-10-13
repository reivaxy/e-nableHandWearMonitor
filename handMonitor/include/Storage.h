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
#include <list>

#define LAST_RTC_ADDR 124  // address of last 4 bytes of 512: (512/4)-4 
#define DEFAULT_LIST_ALLOC 300

typedef std::list<char[15]> FileList;

class Storage {
public:
   static void recordStateChange(int previousState);
   static void listFiles(char **list);
   static void createFakeData();
};

