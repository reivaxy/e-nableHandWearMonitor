/* 
 *  =============================================================================================================================================
 *  Project : Hand Monitoring e-Nable France
 *  Author  : Xavier Grosjean & Thomas Broussard
 * 
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description : 
 * 
 * =============================================================================================================================================
 */

/* 
* ====================================================================================
*                                  Debugging
* ====================================================================================
*/
#ifndef __DEBUG_H__
#define __DEBUG_H__

// remove '//' to enable debug mode
#define __DEBUG

#ifdef __DEBUG
   #define DebugPrintln(x)  Serial.println(x)
   #define DebugPrint(x)    Serial.print(x)
   #define DebugPrintf(...) Serial.printf(__VA_ARGS__)
#else
   #define DebugPrintln(x)
   #define DebugPrint(x)  
   #define DebugPrintf(...)  
#endif

#ifdef __DEBUG
   #define DebugBegin()  Serial.begin(9600)
#else
   #define DebugBegin()
#endif


#endif