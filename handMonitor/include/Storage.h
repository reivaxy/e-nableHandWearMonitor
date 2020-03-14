/* 
 *  =============================================================================================================================================
 *  Project : Hand Monitoring e-Nable France
 *  Author  : Xavier Grosjean & Thomas Broussard
 * 
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description : Hand monitor class for esp file storage 
 * 
 * =============================================================================================================================================
 */

#pragma once

#include "FS.h"
#include "RTClock.h"
#include <list>

#define DEFAULT_FILE_CONTENT_ALLOC 3000

typedef std::list<char*> FileList;
typedef std::list<char*> LineList;

class Storage {
public:
   static void recordStateChange(int newState, int level, RTClock *clock);
   static void listFiles(FileList *fileList);
   static void getFile(const char *fileName, File *f);
   static void createFakeData();
   static void deleteFiles(const char* fileName);
};
 
