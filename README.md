# CyberPhoneK-LinuxDriver

VoIP Cyberphone K is a nice phone and it's still being sold on the internet as of April 2016. However its latest drivers are for Windows XP and there's nothing for Linux. So I've built upon the original work at http://dormouse.org.uk/sw/cyberphone.php and this version is now quite usable.

This version can now send numberkey presses to Skype (tested in 4.3.0.37 on Ubuntu 15.10)
Needs to run as sudo due to libusb standard setup. There's probably some way to get past that but this works fine for me.

Needs xdotool installed to call numbers in Skype.

The Skype application can also call the ringphone application to ring the handset when there's an incoming call.
