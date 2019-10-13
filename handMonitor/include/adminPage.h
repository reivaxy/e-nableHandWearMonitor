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
<h1>BEWARE, dangerous functions</h1>\
<form action='/reset' method='post'>\
  <input type='submit' value='Reset'/>\
</form>\
<form action='/createFakeData' method='post'>\
  <input type='submit' value='Fake data'/>\
</form>\
</body>\
</html>\
";