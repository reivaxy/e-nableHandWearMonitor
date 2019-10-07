# e-nableHandWearMonitor

This device is intended to track the time spans during which an E-nable device is worn by its owner.


## Setup

When the device is powered to charge the battery, it exposes a wifi network with default ssid/pwd "HandMonitor/HandMonitor".

Connect a device (phone, computer) to this wifi network, and open a browser on url http://192.168.4.1 to display the setup page:

<img src="https://raw.githubusercontent.com/reivaxy/e-nableHandWearMonitor/master/resources/setupPage.png" width="400px">

You can change the default ssid/pwd, and, optionally, provide your home wifi ssid and pwd. These are stored locally in the device memory, and never sent anywhere.

[wip] Providing your home wifi connection information will allow to automatically setup the date and time in the device, and will allow to upload the data files listing the times the E-nable hand was worn.

[wip] If you don't provide your home wifi connection information you will be able to setup the time manually, and download the data files to your computer or phone.

The setup page displays a "Update firmware" button. When you click on it the device will toggle to OTA (Over the Air) mode, and wait for a new firmware to be uploaded.

You can then use an android app like "ESP8266 Loader" to upload the new binary from your phone connected to the wifi network, using the device default IP 192.168.4.1



 