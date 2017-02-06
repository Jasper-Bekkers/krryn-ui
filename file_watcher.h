#pragma once
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <experimental/filesystem>

namespace fs {
class file_watcher {
public:
	static void start();
	static void stop();

	static void watch_file(const std::string &a_Filename, std::function<void(const std::string& a_Filename)> a_Callback);
	static void stop_watching(const std::string &a_Filename);

private:
	struct file_watcher_handle
	{
		std::string m_Filename;
		std::experimental::filesystem::file_time_type m_ModifiedTime;
		std::function<void(const std::string& a_Filename)> m_Callback;
	};

	static std::thread m_Thread;
	static std::vector<file_watcher_handle> m_Watching;
	static std::mutex m_WatchingMutex;

	static void update();
};
}