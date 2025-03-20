#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

namespace tk
{

class storage
{
public:
	struct folder
	{
		std::string name_;
		std::unordered_map<std::string, std::shared_ptr<folder>> subFolders_;
		std::vector<std::string> commands_;
		std::weak_ptr<folder> parent_;

		folder(const std::string& name)
		: name_(name)
		{ }
	};

	storage();
	std::shared_ptr<folder> root() const;
	std::shared_ptr<folder> currentFolder() const;
	bool curIsRoot() const;

	void setRoot();

	void folderUp();
	void folderDown(const std::string& name);

	void addFolder(const std::string& name);
	void addCommand(const std::string& command);

	void deleteFolder(const std::string& name);
	void deleteCommand(const std::string& command);

	void renameFolder(const std::string& oldName, const std::string& newName);
	void editCommand(const std::string& oldCommand, const std::string& newCommand);

private:
	std::shared_ptr<folder> root_;
	std::shared_ptr<folder> currentFolder_;
};

} // namespace tk
