#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>

#include <uuid.h>

#include "utils/generate_uuid.h"

namespace tk
{

class storage
{
public:
	using shared_ptr_type = std::shared_ptr<storage>;

	struct command
	{
		std::string content;
		uuids::uuid uuid;

		bool operator== (const uuids::uuid& other) const { return uuid == other; }
	};

	using command_t = command;
	using command_shared_ptr_t = std::shared_ptr<command_t>;
	using command_vec_t = std::vector<command_shared_ptr_t>;

	struct folder
	{
		std::string name_;
		std::map<uuids::uuid, std::shared_ptr<folder>> subFolders_;
		command_vec_t commands_;
		std::weak_ptr<folder> parent_;
		uuids::uuid uuid_;

		folder(const std::string& name)
		: name_(name)
		, uuid_(utils::generate_uuid())
		{ }
	};
	using folder_shared_ptr_t = std::shared_ptr<folder>;

	storage();
	folder_shared_ptr_t root() const;
	folder_shared_ptr_t currentFolder() const;
	bool curIsRoot() const;

	void setRoot();

	void folderUp();
	void folderDown(const uuids::uuid& uuid);

	uuids::uuid addFolder(const std::string& name);
	uuids::uuid addCommand(const std::string& command);

	void deleteFolder(const uuids::uuid& uuid);
	void deleteCommand(const uuids::uuid& uuid);

	void renameFolder(const uuids::uuid& uuid, const std::string& newName);
	void editCommand(const uuids::uuid& uuid, const std::string& newCommand);

	const folder_shared_ptr_t findFolder(const uuids::uuid& uuid) const;
	const command_shared_ptr_t findCommand(const uuids::uuid& uuid) const;

private:
	folder_shared_ptr_t findFolderImpl(const folder_shared_ptr_t& current, const uuids::uuid& uuid) const;

	folder_shared_ptr_t root_;
	folder_shared_ptr_t currentFolder_;
};

} // namespace tk