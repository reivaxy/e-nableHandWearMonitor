 /* 
 *  =============================================================================================================================================
 *  Project : Hand Monitoring e-Nable France
 *  Author  : Reivaxy & Thomas Broussard
 * 
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description : initialization web page
 * 
 * =============================================================================================================================================
 */
#pragma once

#include "messages.h"

char initPage[] = "\
<html title='Hand Monitor'>\
<head>\
<style>\
</style>\
</head>\
<body>\
<h1>" MSG_INIT_WELCOME "</h1>\
<form action='/initSave' method='post'>\
  <input name='apSsid' type='text' placeholder='" MSG_INIT_AP_SSID "'/><br/>\
  <input name='apPwd' type='text' placeholder='" MSG_INIT_AP_PWD "'/><br/>\
  <input name='homeSsid' type='text' placeholder='" MSG_INIT_HOME_SSID "'/><br/>\
  <input name='homePwd' type='text' placeholder='" MSG_INIT_HOME_PWD "'/><br/>\
  <!-- not yet <input name='ntpHost' type='text' placeholder='" MSG_INIT_NTP_HOST "'/><br/>\
  <input name='webSite' type='text' placeholder='" MSG_INIT_WEBSITE "'/><br/>\
  <input name='apiKey' type='text' placeholder='" MSG_INIT_API_KEY "'/><br/> -->\
  <input type='submit'/>\
</form>\
</body>\
</html>\
";