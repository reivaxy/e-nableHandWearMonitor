 /* 
 *  =============================================================================================================================================
 *  Project : Hand Monitoring e-Nable France
 *  Author  : Xavier Grosjean & Thomas Broussard
 * 
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description : initialization web page
 * 
 * =============================================================================================================================================
 */
#pragma once

#include "jsonTags.h"
#include "css.h"

char adminPage[] = "\
<html>\
<head><meta charset='utf-8'>\
<meta name='viewport' content='width=device-width, initial-scale=1'>\
<script src='/data?src=admin'></script>\
" CSS "\
</head>\
<body>\
<h1><a href='/'>" MSG_TITLE "</a></h1>\
<h2>" MSG_CONFIG "</h2>\
<div><span class='label'>" MSG_INIT_NAME ": </span><span class='value " JSON_TAG_NAME "'></span></div>\
<div><span class='label'>" MSG_BUILD_DATE ": </span><span class='value'>" __DATE__ " " __TIME__ "</span></div>\
<div><span class='label'>" MSG_CURRENT_TIME ": </span><span class='value " JSON_TAG_DATE "'/></div>\
<div><span class='label'>" MSG_HOME_WIFI ": </span><span class='value " JSON_TAG_SSID "'>" MSG_UNDEFINED "</span><span class='label'>, IP: </span><span class='value " JSON_TAG_SSID_IP "'>" MSG_UNDEFINED "</span></div>\
<div><span class='label'>" MSG_AP_WIFI ": </span><span class='value " JSON_TAG_APSSID "'></span><span class='label'>, IP: </span><span class='value " JSON_TAG_APSSID_IP "'></span></div>\
<div><span class='label'>" MSG_LEVEL ": </span><span class='value " JSON_TAG_LEVEL "'></span></div>\
<div><span class='label'>" MSG_THRESHOLD ": </span><span class='value " JSON_TAG_THRESHOLD "'></span></div>\
<div><span class='label'>" MSG_INIT_REFRESH_INTERVAL ": </span><span class='value " JSON_TAG_REFRESH "'></span></div>\
<form action='/init' method='post'>\  
  <!-- not yet \
  <input name='ntpHost' type='text' placeholder='" MSG_INIT_NTP_HOST "'/><br/>\
  <input name='webSite' type='text' placeholder='" MSG_INIT_WEBSITE "'/><br/>\
  <input name='apiKey' type='text' placeholder='" MSG_INIT_API_KEY "'/><br/>\
  -->\
  <input name='sensorThreshold' type='number' min='0' max='1024' placeholder='" MSG_INIT_SENSOR_THRESHOLD "' style='width: 150px;'/><br/>\
  <input name='refreshInterval' type='number' min='0' max='255' placeholder='" MSG_INIT_REFRESH_INTERVAL "' style='width: 150px;'/><br/>\
  <input type='submit'/>\
</form>\
<hr/>\
<h2>" MSG_MEMORY "</h2>\
<div><span class='label'>" MSG_MEMORY_SIZE ": </span><span class='value " JSON_TAG_MEMORY_SIZE "'></span></div>\
<div><span class='label'>" MSG_MEMORY_USED ": </span><span class='value " JSON_TAG_MEMORY_USED "'></span></div>\
<div><span class='label'>" MSG_MEMORY_MAX_FILES ": </span><span class='value " JSON_TAG_MEMORY_MAX_FILES "'></span></div>\
<hr/>\
<form action='/reset' method='post'>\
  <input type='submit' value='Reset config & files'/>\
</form>\
<form action='/resetConfig' method='post'>\
  <input type='submit' value='Reset config only'/>\
</form>\
<form action='/erase' method='post'>\
  <input type='submit' value='Erase all files'/>\
</form>\
<form action='/createFakeData' method='post'>\
  <input type='submit' value='Create fake data'/>\
</form>\
<form action='/ota' method='post'>\
  <input type='submit' value='Update Firmware'/>\
</form>\
" DATA_SCRIPT "\
</body>\
</html>\
";