#pragma once

#include <memory>

#include "model/interface.h"

namespace tk
{

class commandImpl : public command
{
public:
	commandImpl(const std::string& content);
	std::string getContent() const override;

private:
	std::string content_ {};
};

class commandFolderImpl : public commandFolder
{
public:
	const command::s_ptr_vec_t& getCommands() const override;
	const folders_map_t& getFolders() const override;
	std::string getName() const override;

private:
	std::string name_ {"unknown"};
	command::s_ptr_vec_t commands_ {};
	folders_map_t folders_ {};
	s_ptr_t parent_ { nullptr };

	friend class dataModelImpl;
};

class dataModelImpl : public dataModel
{
public:
	const commandFolder& getCurrentFolder() const override;
	void changeFolderUp() override;
	void changeFolderDown(const std::string& folderName) override;

	static constexpr size_t MAX_CACHE_SIZE = 15;
	void addToCache(const command::s_ptr_t command) override;
	const cache_t& getCache() const override;
	void clearCache() override;

private:
	cache_t cache_ {};

	std::shared_ptr<commandFolderImpl> root_ { nullptr };
	std::shared_ptr<commandFolderImpl> currentFolder_ { nullptr };
};

} // namespace tk