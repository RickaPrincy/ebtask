#include "os.hpp"

#include <array>
#include <memory>
#include <stdexcept>

std::string ebtask::execute_command(const std::string &command)
{
	const char *cmd = command.c_str();
	// There is no bufferflow here XD
	std::array<char, 128> buffer;
	std::string result;
	std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);

	if (!pipe)
		throw std::runtime_error("Error when try to open pipe");

	while (!feof(pipe.get()))
		if (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr)
			result += buffer.data();
	return result;
}
