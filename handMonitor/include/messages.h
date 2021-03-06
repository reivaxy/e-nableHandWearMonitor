/* 
 *  =============================================================================================================================================
 *  Project : Hand Monitoring e-Nable France
 *  Author  : Xavier Grosjean & Thomas Broussard
 * 
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description : define which message file should be used. Allows to not modify cpp file to switch to another language 
 * 
 * =============================================================================================================================================
 */

#pragma once

// This file only defines which message file should be used

#define __HM__FR

#ifdef __HM__FR
   #include "messages_fr.h"
#else
   #include "messages_en.h"
#endif