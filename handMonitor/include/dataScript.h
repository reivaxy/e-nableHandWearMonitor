 /* 
 *  =============================================================================================================================================
 *  Project : Hand Monitoring e-Nable France
 *  Author  : Xavier Grosjean & Thomas Broussard
 * 
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description : script to dispatch json data to targetted dom elements
 *  Browses the fields of a pre loaded json object stored in global jsonData variable, 
 *  and set their values into dom elements having the json key for class
 * 
 * =============================================================================================================================================
 */
#pragma once

#define DATA_SCRIPT "\
<script>\
if (jsonData) {\
   for(var key in jsonData) {\
      var element = document.querySelector('.' + key);\
      if (element) {\
         var value = jsonData[key];\
         element.textContent = value;\
      }\
   }\
}\
</script>\
"
