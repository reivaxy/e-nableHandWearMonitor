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

#include "messages.h"
#include "jsonTags.h"
#include "css.h"

char initPage[] = "\
<html>\
<head><meta charset='utf-8'>\
<meta name='viewport' content='width=device-width, initial-scale=1'>\
<script src='/data?src=init'></script>\
" CSS "\
</head>\
<body>\
<h1><a href='/'>" MSG_TITLE "</a></h1>\
<h2 class='value " JSON_TAG_NAME "'></h2>\
<div><span class='label'>" MSG_BUILD_DATE ": </span><span class='value'>" __DATE__ " " __TIME__ "</span></div>\
<div><span class='label'>" MSG_CURRENT_TIME ": </span><span class='value " JSON_TAG_DATE "'>" MSG_UNDEFINED "</span></div>\
<div><span class='label'>" MSG_HOME_WIFI ": </span><span class='value " JSON_TAG_SSID "'>" MSG_UNDEFINED "</span><span class='label'>, IP: </span><span class='value " JSON_TAG_SSID_IP "'>" MSG_UNDEFINED "</span></div>\
<div><span class='label'>" MSG_AP_WIFI ": </span><span class='value " JSON_TAG_APSSID "'></span><span class='label'>, IP: </span><span class='value " JSON_TAG_APSSID_IP "'></span></div>\
<div><span class='label'>" MSG_LEVEL ": </span><span class='value " JSON_TAG_LEVEL "'></span></div>\
<div><span class='label'>" MSG_THRESHOLD ": </span><span class='value " JSON_TAG_THRESHOLD "'></span></div>\
<div><span class='label'>" MSG_INIT_REFRESH_INTERVAL ": </span><span class='value " JSON_TAG_REFRESH "'></span></div>\
<div><span class='label'>" MSG_PAUSE ": </span><span class='value " JSON_TAG_START_PAUSE "'></span> - <span class='value " JSON_TAG_END_PAUSE "'></span></div>\
<div><span class='label'>" MSG_IS_PAUSED ": </span><span class='value " JSON_TAG_IS_PAUSED "'></span></div>\
<hr/>\
<h2>" MSG_DATA "</h2>\
<form action='/listFiles' method='get'>\
  <input type='submit' value='" MSG_FILE_LIST "'/>\
</form>\
<hr/>\
<h2>" MSG_CONFIG "</h2>\
<form action='/init' method='post'>\
  <div class='value'><span class='label'>" MSG_INIT_NAME "</span><input class='value' name='name' type='text'/></div>\
  <div class='value'><span class='label'>" MSG_INIT_AP_SSID "</span><input class='value' name='apSsid' type='text'/></div>\
  <div class='value'><span class='label'>" MSG_INIT_AP_PWD "</span><input class='value' name='apPwd' type='password'/></div>\
  <div class='value'><span class='label'>" MSG_INIT_HOME_SSID "</span><input class='value " JSON_TAG_SSID "' name='homeSsid' type='text'/></div>\
  <div class='value'><span class='label'>" MSG_INIT_HOME_PWD "</span><input class='value' name='homePwd' type='password'/></div>\
\  
  <div  class='value' id='timeOffset'><span>" MSG_TIME_OFFSET "</span><br/>\
  <span class='label'>" MSG_INIT_TIME_OFFSET "</span><input class='value' name='timeOffset' type='text'/></div>\
\  
  <div class='value' id='manualTime'><span>" MSG_MANUAL_TIME_SETTING "</span><br/>\
  <span class='label'>" MSG_CURRENT_TIME "</span><input class='value' name='manualDateTime' type='datetime-local'/></div>\
\
  <div class='value'><span class='label'>" MSG_INIT_START_PAUSE "</span><input class='value' name='startPause' type='time'/></div>\
  <div class='value'><span class='label'>" MSG_INIT_END_PAUSE "</span><input class='value' name='endPause' type='time'/></div>\
  <div class='value'><span class='label'>" MSG_INIT_PAUSE_REFRESH_INTERVAL "</span><input class='value' name='intervalPause' type='number'/></div>\
\
  <input type='submit'/>\
</form>\
<hr/>\
<h2>" MSG_SYSTEM "</h2>\
<form action='/ota' method='post'>\
  <input type='submit' value='" MSG_START_OTA "'/>\
</form>\
" DATA_SCRIPT "\
</body>\
</html>\
";