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
<html title='Hand Monitor Admin'>\
<head><meta charset='utf-8'>\
<meta name='viewport' content='width=device-width, initial-scale=1'>\
</head>\
<body>\
<h1>Hand Monitor Admin</h1>\
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