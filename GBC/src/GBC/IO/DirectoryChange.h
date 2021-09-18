#pragma once

#include <cstdint>
#include <string>
#include <filesystem>
#include <thread>
#include <functional>

namespace gbc
{
	using NotificationType = uint8_t;
	enum NotificationType_
	{
		NotificationType_FileNameChanged      = 1 << 0, // Renaming, creating, or deleting a file.
		NotificationType_DirectoryNameChanged = 1 << 1, // Renaming, creating, or deleting a directory.
		NotificationType_AttributesChanged    = 1 << 2, // Changing the attributes of a file or directory.
		NotificationType_SizeChanged          = 1 << 3, // Changing the size of a file or directory.
		NotificationType_LastWriteChanged     = 1 << 4, // Writing to a file or directory
		NotificationType_SecurityChanged      = 1 << 5, // Changing the security of a file or directory.

		NotificationType_Any = NotificationType_FileNameChanged | NotificationType_DirectoryNameChanged
							 | NotificationType_AttributesChanged | NotificationType_SizeChanged
							 | NotificationType_LastWriteChanged | NotificationType_SecurityChanged
	};

	// Parameter: if everything went smoothly and a notification was received
	// Return value: if you want to continue waiting for notifications
	using DirectoryChangeNotificationFunc = std::function<bool(bool)>;

	struct DirectoryChangeNotifier
	{
		DirectoryChangeNotifier() noexcept = default;
		DirectoryChangeNotifier(DirectoryChangeNotifier&& notification) noexcept;
		DirectoryChangeNotifier& operator=(DirectoryChangeNotifier&& notification) noexcept;
		~DirectoryChangeNotifier();

		void Remove();
	private:
		friend class DirectoryChange;
		DirectoryChangeNotifier(std::thread&& thread);
		std::thread thread;
		void* closeHandle = nullptr;
	};

	class DirectoryChange
	{
	private:
		DirectoryChange() = delete;
		DirectoryChange(const DirectoryChange&) = delete;
		DirectoryChange(DirectoryChange&&) = delete;
		DirectoryChange& operator=(const DirectoryChange&) = delete;
		DirectoryChange& operator=(DirectoryChange&&) = delete;
		~DirectoryChange() = delete;
	public:
		static DirectoryChangeNotifier CreateNotifier(const DirectoryChangeNotificationFunc& notificationFunc, const std::filesystem::path& directoryPath, NotificationType notificationType, bool checkSubdirectories = false);
	};
}
