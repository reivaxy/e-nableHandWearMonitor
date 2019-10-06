/* 
 *  =============================================================================================================================================
 *  Project : Hand Monitoring e-Nable France
 *  Author  : Reivaxy & Thomas Broussard
 * 
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description : 
 * 
 * =============================================================================================================================================
 */

#ifndef __MAPPING_H__
#define __MAPPING_H__

/* 
* ====================================================================================
*                                  I/O Mapping
* ====================================================================================
*/

/** RTC Clock */
#define PIN_RTC_SCL 4
#define PIN_RTC_SDA 5
#define PIN_RTC_VCC 16

/** Sensor */
#define PIN_SENSOR  A0

/** Charger detection */
#define PIN_POWER_DETECT 12


/* 
* ====================================================================================
*                                  Debugging
* ====================================================================================
*/

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