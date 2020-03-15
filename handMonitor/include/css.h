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
      background-color:#bfe3fd;\
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
   div.logo {\
      position: absolute;\
      top:3px;\
      right:3px;\
      width:80px;\
      height:72px;\
      background-image:url(/logo.png)\
   }\
   a.erase, a.download {\
      text-decoration:none;\
   }\
</style>\
<link href='custom.css' rel='stylesheet' type='text/css' />\
"
