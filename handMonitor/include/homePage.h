 /* 
 *  =============================================================================================================================================
 *  Project : Hand Monitoring e-Nable France
 *  Author  : Xavier Grosjean & Thomas Broussard
 * 
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description : home web page
 * 
 * =============================================================================================================================================
 */
#pragma once

#include "messages.h"
#include "dataScript.h"
#include "jsonTags.h"
#include "css.h"

char homePage[] = "\
<html>\
<head><meta charset='utf-8'>\
<meta name='viewport' content='width=device-width, initial-scale=1'>\
<script src='/data?src=home'></script>\
" CSS "\
</head>\
<body>\
<h1>" MSG_TITLE "</h1>\
<h2 class='value " JSON_TAG_NAME "'></h2>\
<div><span class='label'>" MSG_CURRENT_TIME ": </span><span class='value " JSON_TAG_DATE "'/></div>\
<div><span class='label'>" MSG_HOME_WIFI ": </span><span class='value " JSON_TAG_SSID "'>" MSG_UNDEFINED "</span><span class='label'>, IP: </span><span class='value " JSON_TAG_SSID_IP "'>" MSG_UNDEFINED "</span></div>\
<div><span class='label'>" MSG_AP_WIFI ": </span><span class='value " JSON_TAG_APSSID "'></span><span class='label'>, IP: </span><span class='value " JSON_TAG_APSSID_IP "'></span></div>\
<div><span class='label'>" MSG_WORN ": </span><span class='value " JSON_TAG_WORN "'></span></div>\
<div><span class='label'>" MSG_LEVEL ": </span><span class='value " JSON_TAG_LEVEL "'></span></div>\
<hr/>\
<form action='/listFiles' method='get'>\
  <input type='submit' value='" MSG_FILE_LIST "'/>\
</form>\
<hr/>\
<form action='/init' method='get'>\
  <input type='submit' value='" MSG_INIT_PAGE "'/>\
</form>\
<hr/>\
" DATA_SCRIPT "\
</body>\
</html>\
";