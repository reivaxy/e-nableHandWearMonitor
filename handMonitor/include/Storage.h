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
   static void recordStateChange(int previousState, int level);
   static void listFiles(FileList *fileList);
   static void getFile(const char *fileName, File *f);
   static void createFakeData();
};

