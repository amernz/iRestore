#ifndef USB_HPP_
#define USB_HPP_

/*
 * Usb.hpp
 *
 *  Created on: 29 Mar 2022
 *      Author: Dr238
 */

#include <iostream>
#include <libusb-1.0/libusb.h>
#include <unistd.h>
#include <cstdlib>

#define RED     "\033[31m"
#define RESET   "\033[0m"

typedef struct libusb_handler_s libusb_handler_t;

struct libusb_handler_s {
	libusb_device_handle *dev;
	libusb_device *device;
	char serialnumber[2048];
};

void release_device(libusb_handler_t *usb_handler);
libusb_handler_t *reconnect_device(libusb_handler_t *usb_handler, uint16_t vid, uint16_t pid, int timer);


libusb_handler_t *acquire_device(uint16_t vid, uint16_t pid)
{
	libusb_handler_t *usb_handler = nullptr;
	struct libusb_device_descriptor desc;
	int r = -1;

	libusb_init(nullptr);

	usb_handler = (libusb_handler_t*) malloc(sizeof(libusb_handler_t));
	if (usb_handler == nullptr) {
		std::cerr << "Memory was not allocated. Exiting..." << '\n';
		return NULL;
	}

	usb_handler->dev = nullptr;

	while (!usb_handler->dev) {
		usb_handler->dev = libusb_open_device_with_vid_pid(nullptr, vid, pid);
		sleep(1);
	}

	if (!usb_handler->dev) {
		std::cerr << RED << "No device found in DFU Mode!" << RESET << '\n';
		return nullptr;
	}

	usb_handler->device = libusb_get_device(usb_handler->dev);

	r = libusb_claim_interface(usb_handler->dev, 0);
	if (r < 0) {
		std::cerr << "[!] Did not claim interface " << r << '\n';
		return nullptr;
	}

	r = libusb_get_device_descriptor(usb_handler->device, &desc);
	if (r < 0) {
		std::cerr << "[!] Failed to get device descriptor " << r << '\n';
		return nullptr;
	}

	r = libusb_get_string_descriptor_ascii(usb_handler->dev, desc.iSerialNumber, (unsigned char *)usb_handler->serialnumber, sizeof(usb_handler->serialnumber));
	if (r < 0) {
		std::cerr << "[!] Did not get string_descriptor_ascii " << r << '\n';
		return nullptr;
	}

	return usb_handler;
}

void release_device(libusb_handler_t *usb_handler)
{
	libusb_release_interface(usb_handler->dev, 0);
	libusb_close(usb_handler->dev);

	free(usb_handler);
}


libusb_handler_t *reconnect_device(libusb_handler_t *usb_handler, uint16_t vid, uint16_t pid, int timer)
{
	release_device(usb_handler);
	sleep(timer);
	return acquire_device(vid, pid);
}

#endif /* USB_HPP_ */
