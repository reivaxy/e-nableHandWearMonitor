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

char initPage[] = "\
<html>\
<head><meta charset='utf-8'>\
<meta name='viewport' content='width=device-width, initial-scale=1'>\
</head>\
<body>\
<h1>" MSG_TITLE "</h1>\
<pre>\
<script src='/info'></script>\
" MSG_BUILD_DATE ": " __DATE__ " " __TIME__ "\
</pre>\
<br/>\
<h2>" MSG_DATA "</h2>\
<form action='/listFiles' method='get'>\
  <input type='submit' value='" MSG_FILE_LIST "'/>\
</form>\
<hr/>\
<h2>" MSG_CONFIG "</h2>\
<form action='/initSave' method='post'>\
  <input name='apSsid' type='text' placeholder='" MSG_INIT_AP_SSID "'/><br/>\
  <input name='apPwd' type='password' placeholder='" MSG_INIT_AP_PWD "'/><br/>\
  <input name='homeSsid' type='text' placeholder='" MSG_INIT_HOME_SSID "'/><br/>\
  <input name='homePwd' type='password' placeholder='" MSG_INIT_HOME_PWD "'/><br/>\
  <input type='submit'/>\
</form>\
<hr/>\
<h2>" MSG_SYSTEM "</h2>\
<form action='/ota' method='post'>\
  <input type='submit' value='" MSG_START_OTA "'/>\
</form>\
</body>\
</html>\
";