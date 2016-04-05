/*
 * cyberphone.c
 * Version 0.1
 *
 * Copyright (c) 2008 Richard C Smith (richard at dormouse dot org dot uk)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 *
 * A simple demonstration of reading from and writing to the control 
 * interface of the PDT Cyberphone K
 *
 * Build (as a regular user) with something like:
 * gcc -o cyberphone -Wall -lusb cyberphone.c
 *
 * Depending on how libusb and udev are configured on your machine, you may 
 * need to be root to run this program.
 *
 * For more information, see http://www.dormouse.org.uk/sw/cyberphone.php
 */

#include <stdio.h>
#include <stdlib.h>
#include <usb.h>

#define IDVENDOR	0x0d8d /* Promotion & Display Technology, Ltd */
#define IDPRODUCT	0x0652 /* Cyberphone K */

#ifndef USB_CTRL_GET_TIMEOUT
#define USB_CTRL_GET_TIMEOUT    5000
#endif
#ifndef USB_CTRL_SET_TIMEOUT
#define USB_CTRL_SET_TIMEOUT    5000
#endif

void read_control_reg(usb_dev_handle *devh) {
	char buf[1];
	int ret;
	if ((ret = usb_control_msg(devh, USB_ENDPOINT_IN | USB_TYPE_VENDOR,
			5, 0, 0xa201, buf, 1, USB_CTRL_GET_TIMEOUT)) != 1) {
		printf("Control read returned error: %d\n", ret);
		return;
	}
//	printf("Control read: 0x%02x\n", *buf & 0xff);
}

void read_status_reg(usb_dev_handle *devh) {
	char buf[2];
	int ret;
	if ((ret = usb_control_msg(devh, USB_ENDPOINT_IN | USB_TYPE_VENDOR,
			 5, 0, 0xa202, buf, 2, USB_CTRL_GET_TIMEOUT)) != 2) {
		printf("Status read returned error: %d\n", ret);
		return;
	}

	if (*buf == 0x03) {
//	    printf("Status read: 0x%02x 0x%02x\n", *buf & 0xff, *(buf+1) & 0xff);
	    if (*(buf+1) == 0x51) system("xdotool key 1");
	    if (*(buf+1) == 0x41) system("xdotool key 2");
	    if (*(buf+1) == 0x31) system("xdotool key 3");
	    if (*(buf+1) == 0x21) system("xdotool key 4");
	    if (*(buf+1) == 0x11) system("xdotool key 5");
	    if (*(buf+1) == 0x52) system("xdotool key 6");
	    if (*(buf+1) == 0x42) system("xdotool key 7");
	    if (*(buf+1) == 0x32) system("xdotool key 8");
	    if (*(buf+1) == 0x22) system("xdotool key 9");
	    if (*(buf+1) == 0x53) system("xdotool key asterisk");
	    if (*(buf+1) == 0x43) system("xdotool key 0");
	    if (*(buf+1) == 0x33) system("xdotool key numbersign");
	    if (*(buf+1) == 0x54) system("xdotool key KP_Enter");
	    if (*(buf+1) == 0x55) system("xdotool key Escape");
	    usleep(200000);
	}
}

void clear_status_reg(usb_dev_handle *devh) {
        char buf[2];
        int ret;
        if ((ret = usb_control_msg(devh, USB_ENDPOINT_IN | USB_TYPE_VENDOR,
                         5, 0, 0xa202, buf, 2, USB_CTRL_GET_TIMEOUT)) != 2) {
                printf("Status read returned error: %d\n", ret);
                return;
        }
}


void ring_bell(usb_dev_handle *devh) {
	char buf[1];
	int ret;
	*buf = 0xb5; /* A kind of magic */
	if ((ret = usb_control_msg(devh, USB_ENDPOINT_OUT | USB_TYPE_VENDOR,
			4, 0, 0xa201, buf, 1, USB_CTRL_SET_TIMEOUT)) != 1) {
		printf("Control write returned error: %d\n", ret);
		return;
	}
	printf("Lift handset to stop phone ringing.\n");
}

int main(void) {
	struct usb_bus *busses, *bus;

	usb_init();
	usb_find_busses();
	usb_find_devices();
	busses = usb_get_busses();
	for (bus = busses; bus; bus = bus->next) {
		struct usb_device *dev;
		usb_dev_handle *devh;

		for (dev = bus->devices; dev; dev = dev->next) {
			while (dev->descriptor.idVendor == IDVENDOR
			    && dev->descriptor.idProduct == IDPRODUCT) {
				/* Device found */
				devh = usb_open(dev);
				if (devh == NULL)
					exit(EXIT_FAILURE);
				read_control_reg(devh);
				read_status_reg(devh);
				for (int i=0; i<99; i++) {
                		    clear_status_reg(devh);
            			}
				//ring_bell(devh);
				usb_close(devh);
				
			}
		}
	}
	
	exit(EXIT_SUCCESS);
}

/* EOF */
