#pragma once

#include <libudev.h>

#include <functional>
#include <stdexcept>

namespace ebtask
{
	template<typename T>
	class UdevResourceWrapper
	{
	private:
		T *mudev_resource = nullptr;
		std::function<void(T *resource)> mdeleter;

	public:
		UdevResourceWrapper(T *udev_resource, std::function<void(T *resource)> deleter)
		{
			mudev_resource = udev_resource;
			mdeleter = deleter;

			if (!mudev_resource)
				throw std::runtime_error("Udev Resource is not valid");
		}

		operator T *() const
		{
			return mudev_resource;
		}

		virtual ~UdevResourceWrapper()
		{
			mdeleter(mudev_resource);
		}
	};
}  // namespace ebtask
