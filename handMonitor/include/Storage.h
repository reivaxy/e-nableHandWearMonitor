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
#define DEFAULT_FILE_CONTENT_ALLOC 3000

typedef std::list<char*> FileList;
typedef std::list<char*> LineList;

class Storage {
public:
   static void recordStateChange(int previousState);
   static void listFiles(FileList *fileList);
   static void readFile(const char *fileName, LineList *lineList);
   static void createFakeData();
};

