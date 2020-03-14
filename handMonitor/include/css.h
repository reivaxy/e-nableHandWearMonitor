 /* 
 *  =============================================================================================================================================
 *  Project : Hand Monitoring e-Nable France
 *  Author  : Xavier Grosjean & Thomas Broussard
 * 
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description : css for web pages
 * 
 * =============================================================================================================================================
 */
#pragma once

#define CSS "\
<style>\
   body {\
      font-family: sans-serif;\
      font-size:15px;\
   }\
   span.value {\
      color: blue;\
   }\
   h1 a {\
      color: black;\
      text-decoration: none;\
   }\
   body.hideManualTime #manualTime {\
      display: none;\
   }\
   body.hideTimeOffset #timeOffset {\
      display: none;\
   }\
   input.value {\
      position:absolute;\
      right:5px;\
      width: 150px;\
   }\
   div.value {\
      position: relative;\
      max-width: 450px;\
      padding:5px 0px;\
   }\
</style>\
"
