# e-nableHandWearMonitor

This device is intended to track the time spans during which an E-nable device is worn by its owner.

## How does it work?

It uses a sensor to detect that the device is worn. Two types of sensors are supported: either a pair of metallic contacts that should be in contact with the skin when the device is worn, or an infrared sensor, that does not need any contact with the skin, which avoids the risk of allergies.

The device has two modes: "recording" and "charging".


In "recording mode" which is as long as the battery lasts, and not plugged into an usb charger, the Esp8266 wakes up at a period that can be set and saved in permanent memory, checks the presence sensor, and compares its state to the previous time it was checked.

If there is a difference, it records the date and time in a file, in the Esp flash memory, saves the new state, and goes back to deep sleep until it automatically wakes up again. This allows to save battery life.

If there is no difference, it just goes back to deep sleep. 


In "charging mode", which is when an usb charger is plugged in, it keeps doing the check but every second and won't sleep. The blue led blinks every 3 seconds to indicate the wifi is activated.

Instead it opens up a wifi network. When the device has not been set up, the wifi network SSID is "HandMonitor", and password it the same.

By connecting your phone to this network, one can open the configuration page at http://192.168.4.1 and set up the device.


## Setup

Here is what the setup page looks like (for now, it may change and this picture may not be up to date):

<img src="https://raw.githubusercontent.com/reivaxy/e-nableHandWearMonitor/master/resources/setupPage.png" width="300px">

You can change the default ssid/pwd, and, optionally, provide your home wifi ssid and pwd. These are stored locally in the device memory, and never sent anywhere.

Providing your home wifi connection information will allow to automatically setup the date and time in the device, and will allow you to upload the data files listing the times the E-nable hand was worn (work in progress)

If you don't provide your home wifi connection information you will be able to setup the time manually (work in progress), and download the data files to your computer or phone.

The setup page also displays an "Update firmware" button. When you click on it the device will toggle to OTA (Over the Air) mode, and wait for a new firmware to be uploaded. The blue led will blink twice per second in this mode.

You can then use an android app like "ESP8266 Loader" to upload the new binary from your phone connected to the wifi network, using the device default IP 192.168.4.1

<img src="https://raw.githubusercontent.com/reivaxy/e-nableHandWearMonitor/master/resources/ESP8266Loader.png" width="300px">

The blue led on the esp will then flash frantically while the firmware is uploading.

<img src="https://raw.githubusercontent.com/reivaxy/e-nableHandWearMonitor/master/resources/firmwareUpdate.png" width="300px">

If you provided your home wifi ssid and password, you can upload using the IP on that network, too. 

After the upload is done (and verified) the esp will reboot, updated, and open the wifi networks depending on the settings provided.


## Data


The "File list" button will display a page listing all the files in the esp memory. One new file is created each month.

Here, I have created fake test data files (dated before 2019), and you can see the current file at the time this is written (Oct. 2019):

<img src="https://raw.githubusercontent.com/reivaxy/e-nableHandWearMonitor/master/resources/fileList.png" width="300px">

Then, clicking on a file, you can display its content, or download it with a long press / download file.

<img src="https://raw.githubusercontent.com/reivaxy/e-nableHandWearMonitor/master/resources/fileContent.png" width="200px">

This file was made with a 10 seconds wake up period, and with a state change each time, to make a biiig file. I had no issue recording 15695 lines, in a 223K file. 

Each entry is composed of the day, then the hour, minute, second, then the state: 0 means device not worn, 1 means device worn.


## "Admin" page

It's actually a page where some "dangerous" features are available: reset the full device (configuration and data files), erase all data files, create test data files...

There is also some information on the SPIFFS memory usage, and more will be added if needed.

<img src="https://raw.githubusercontent.com/reivaxy/e-nableHandWearMonitor/master/resources/adminPage.png" width="300px">


## The 1000 mAh version

The prototype was built using a 1000mAh battery, which provides an autonomy of several days (still need to check how many exactly).

It is not using the definitive PCB, some wires will be removed, some layout improved to ease assembly.


Here are pictures of this device in its 3D printed case with a slide-in lid:


<img src="https://raw.githubusercontent.com/reivaxy/e-nableHandWearMonitor/master/resources/deviceOpen.jpg" width="300px">

<img src="https://raw.githubusercontent.com/reivaxy/e-nableHandWearMonitor/master/resources/device.jpg" width="300px">

<img src="https://raw.githubusercontent.com/reivaxy/e-nableHandWearMonitor/master/resources/backSide.jpg" width="300px">


Solderless connections available on the PCB allow to monitor the serial port for debug messages:

<img src="https://raw.githubusercontent.com/reivaxy/e-nableHandWearMonitor/master/resources/monitoring.jpg" width="300px">

Using a custom tool that clips in the lid rails:

<img src="https://raw.githubusercontent.com/reivaxy/e-nableHandWearMonitor/master/resources/monitoringTool.jpg" width="300px">


The PCB is very thin to minimize weight and volume. This prototype PCB has a faulty IR sensor package layout and incorrect battery connection to charger, fixed for next version.

<img src="https://raw.githubusercontent.com/reivaxy/e-nableHandWearMonitor/master/resources/pcb.jpg" width="300px">


 
