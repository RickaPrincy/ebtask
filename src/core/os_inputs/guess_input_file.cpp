#include <libudev.h>

#include <cstring>
#include <stdexcept>

#include "os_inputs.hpp"
#include "udev_wrapper.hpp"

std::string ebtask::guess_input_file()
{
	ebtask::UdevResourceWrapper<udev> udev(udev_new(), udev_unref);
	std::string result = "";

	ebtask::UdevResourceWrapper<udev_enumerate> enumerate(
		udev_enumerate_new(udev), udev_enumerate_unref);
	udev_enumerate_add_match_subsystem(enumerate, "input");
	udev_enumerate_add_match_property(enumerate, "ID_INPUT_KEYBOARD", "1");
	udev_enumerate_scan_devices(enumerate);

	struct udev_list_entry *devices = udev_enumerate_get_list_entry(enumerate);
	struct udev_list_entry *entry;

	udev_list_entry_foreach(entry, devices)
	{
		const char *path = udev_list_entry_get_name(entry);
		ebtask::UdevResourceWrapper<udev_device> dev(
			udev_device_new_from_syspath(udev, path), udev_device_unref);

		const char *devnode = udev_device_get_devnode(dev);

		if (devnode && strstr(devnode, "event"))
			result = devnode;
	}

	if (result.empty())
		throw std::runtime_error("No input file found.");

	return result;
}
