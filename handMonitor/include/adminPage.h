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

char adminPage[] = "\
<html>\
<head><meta charset='utf-8'>\
<meta name='viewport' content='width=device-width, initial-scale=1'>\
</head>\
<body>\
<h1>Hand Monitor Admin</h1>\
<form action='/initSave' method='post'>\  
  <!-- not yet \
  <input name='ntpHost' type='text' placeholder='" MSG_INIT_NTP_HOST "'/><br/>\
  <input name='webSite' type='text' placeholder='" MSG_INIT_WEBSITE "'/><br/>\
  <input name='apiKey' type='text' placeholder='" MSG_INIT_API_KEY "'/><br/>\
  -->\
  <input name='sensorThreshold' type='number' min='0' max='1024' placeholder='" MSG_INIT_SENSOR_THRESHOLD "' size='20'/><br/>\
  <input name='refreshInterval' type='number' min='0' max='300' placeholder='" MSG_INIT_REFRESH_INTERVAL "' size='20'/><br/>\
  <input type='submit'/>\
</form>\
<form action='/reset' method='post'>\
  <input type='submit' value='Reset config & files'/>\
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
<pre>\
Build date: " __DATE__ " " __TIME__ "\n\
SPIFFS:\n\
<script src='/adminInfo'></script>\
<pre>\
</body>\
</html>\
";