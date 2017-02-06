#include "file_watcher.h"

namespace fs{
std::thread file_watcher::m_Thread;
std::vector<file_watcher::file_watcher_handle> file_watcher::m_Watching;
std::mutex file_watcher::m_WatchingMutex;

void file_watcher::start() {
	m_Thread = std::thread([]() {
		while (true) {
			file_watcher::update();
		}
	});
}

void file_watcher::stop() {
	m_Thread.join();
}

void file_watcher::watch_file(const std::string & a_Filename, std::function<void(const std::string&a_Filename)> a_Callback) {
	file_watcher_handle h;
	h.m_Callback = a_Callback;
	h.m_ModifiedTime = std::experimental::filesystem::last_write_time(a_Filename);
	h.m_Filename = a_Filename;

	std::lock_guard<std::mutex> lockScope(m_WatchingMutex);

	m_Watching.push_back(h);
}

void file_watcher::stop_watching(const std::string & a_Filename) {
	std::lock_guard<std::mutex> lockScope(m_WatchingMutex);

	auto foundIt = std::find_if(m_Watching.begin(), m_Watching.end(), [a_Filename](const file_watcher_handle& a_Handle) {
		return a_Handle.m_Filename == a_Filename;
	});

	if (foundIt != m_Watching.end()) {
		m_Watching.erase(foundIt);
	}
}

void file_watcher::update() {
	// if this list becomes long, this mutex might be an issue
	std::lock_guard<std::mutex> lockScope(m_WatchingMutex);

	for(auto& h : m_Watching) {
		auto lastModified = std::experimental::filesystem::last_write_time(h.m_Filename);
		if (lastModified > h.m_ModifiedTime) {
			h.m_Callback(h.m_Filename);
			h.m_ModifiedTime = lastModified;
		}
	}
}
}