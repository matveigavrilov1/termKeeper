#pragma once

#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace tk
{

class command
{
public:
	virtual ~command() = default;

	using s_ptr_t = std::shared_ptr<command>;
	using u_ptr_t = std::unique_ptr<command>;
	using s_ptr_vec_t = std::vector<s_ptr_t>;

	virtual std::string getContent() const = 0;
};

class commandFolder
{
public:
	virtual ~commandFolder() = default;

	using s_ptr_t = std::shared_ptr<commandFolder>;
	using u_ptr_t = std::unique_ptr<commandFolder>;
	using folders_map_t = std::map<std::string, commandFolder::s_ptr_t>;

	virtual const command::s_ptr_vec_t& getCommands() const = 0;
	virtual const folders_map_t& getFolders() const = 0;
	virtual std::string getName() const = 0;
};

class dataModel
{
public:
	dataModel() = default;
	virtual ~dataModel() = default;

	using s_ptr_t = std::shared_ptr<dataModel>;
	using u_ptr_t = std::unique_ptr<dataModel>;

public:
	virtual const commandFolder& getCurrentFolder() const = 0;
	virtual void changeFolderUp() = 0;
	virtual void changeFolderDown(const std::string& folderName) = 0;

public:
	using cache_t = std::list<command::s_ptr_t>;

	virtual void addToCache(const command::s_ptr_t command) = 0;
	virtual const cache_t& getCache() const = 0;
	virtual void clearCache() = 0;
};

} // namespace tk