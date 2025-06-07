#pragma once

#include <memory>
#include <string>
#include <vector>

#include "utils/with_uuid.h"

namespace data
{
class actionsFolder;

class action final : public utils::with_uuid
{
public:
	using shared_ptr_t = std::shared_ptr<action>;

	enum type
	{
		NONE,
		COPY_TERMINAL_COMMAND,
		EXECUTE_TERMINAL_COMMAND
	};

	action(const std::string& alias, const std::string& content, const std::string& description_, type type);

	std::string getAlias() const;
	std::string getContent() const;
	std::string getDescription() const;
	type getType() const;

	std::string getPath() const;

	void setFolder(std::shared_ptr<actionsFolder> folder);
	std::shared_ptr<actionsFolder> getFolder();

private:
	std::string alias_;
	std::string content_;
	std::string description_;
	type type_;

	std::shared_ptr<actionsFolder> folder_;
};

class actionsFolder final
: public utils::with_uuid
, public std::enable_shared_from_this<actionsFolder>
{
public:
	using shared_ptr_t = std::shared_ptr<actionsFolder>;
	using actions_vec_t = std::vector<action::shared_ptr_t>;
	using folders_vec_t = std::vector<shared_ptr_t>;

	actionsFolder(const std::string& name = "", const std::string& description = "");

	std::string getName() const;
	std::string getDescription() const;
	actions_vec_t getActions() const;
	shared_ptr_t getParent() const;
	folders_vec_t getChildren() const;
	std::string getPath() const;

	void addAction(action::shared_ptr_t action);
	void deleteAction(uuids::uuid uuid);

	void addFolder(shared_ptr_t folder);
	void deleteFolder(uuids::uuid);

	action::shared_ptr_t searchByUuid(uuids::uuid uuid);
	actions_vec_t searchByAlias(const std::string& alias);
	actions_vec_t searchByContent(const std::string& alias);
	actions_vec_t searchByDescription(const std::string& alias);

	action::shared_ptr_t recursiveSearchByUuid(uuids::uuid uuid);
	actions_vec_t recursiveSearchByAlias(const std::string& alias);
	actions_vec_t recursiveSearchByContent(const std::string& alias);
	actions_vec_t recursiveSearchByDescription(const std::string& alias);

private:
	std::string name_;
	std::string description_;
	actions_vec_t actions_;
	shared_ptr_t parent_;
	folders_vec_t children_;
};

class actions
{
public:
	actionsFolder::shared_ptr_t getRoot();
	actionsFolder::shared_ptr_t getCurrent();

	void folderUp();
	void folderDown(uuids::uuid uuid);

	actionsFolder::actions_vec_t searchByPath(const std::string& pathSnippet);

private:
	actionsFolder::shared_ptr_t root_ { std::make_shared<actionsFolder>() };
	actionsFolder::shared_ptr_t cur_ { root_ };
};
} // namespace data