#include "FileSystem.h"
#include "config.h"
#include <cstdint>
#include <string>

#ifdef _WIN32
#include <Windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#endif

namespace eng
{
	std::filesystem::path FileSystem::GetExecutableFolder() const
	{
#ifdef _WIN32
		wchar_t buf[MAX_PATH];
		GetModuleFileNameW(NULL, buf, MAX_PATH);
		return std::filesystem::path(buf).remove_filename();
#elif defined(__APPLE__)
		uint32_t size = 0;
		_NSGetExecutablePath(nullptr, &size);
		std::string tmp(size, '\0');
		_NSGetExecutablePath(tmp.data(), &size);
		return std::filesystem::weakly_canonical(std::filesystem::path(tmp)).remove_filename();
#elif defined(__linux__)
		return std::filesystem::weakly_canonical(std::filesystem::read_symlink("/proc/self/exe")).remove_filename();
#else
		return std::filesystem::current_path();
#endif
	}

	std::filesystem::path FileSystem::GetAssetsFolder() const
	{
		auto executableAssetsPath = GetExecutableFolder() / "assets";
		if (std::filesystem::exists(executableAssetsPath))
		{
			return std::filesystem::weakly_canonical(executableAssetsPath);
		}

#ifdef ASSETS_ROOT
		auto sourceAssetsPath = std::filesystem::path(ASSETS_ROOT);
		if (std::filesystem::exists(sourceAssetsPath))
		{
			return std::filesystem::weakly_canonical(sourceAssetsPath);
		}
#endif
		return executableAssetsPath;
	}
}
