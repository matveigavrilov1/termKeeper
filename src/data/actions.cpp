#include "data/actions.h"

namespace data
{

// action implementation
action::action(const std::string& alias, const std::string& content, const std::string& description, type type)
: alias_(alias)
, content_(content)
, description_(description)
, type_(type)
{ }

std::string action::getAlias() const
{
	return alias_;
}

std::string action::getContent() const
{
	return content_;
}

std::string action::getDescription() const
{
	return description_;
}

action::type action::getType() const
{
	return type_;
}

std::string action::getPath() const
{
	if (folder_)
	{
		return folder_->getPath() + "/" + getAlias();
	}
	return "/" + getAlias();
}

void action::setFolder(std::shared_ptr<actionsFolder> folder)
{
	folder_ = folder;
}

std::shared_ptr<actionsFolder> action::getFolder()
{
	return folder_;
}

// actionsFolder implementation
actionsFolder::actionsFolder(const std::string& name, const std::string& description)
: name_(name)
, description_(description)
{ }

std::string actionsFolder::getName() const
{
	return name_;
}

std::string actionsFolder::getDescription() const
{
	return description_;
}

actionsFolder::actions_vec_t actionsFolder::getActions() const
{
	return actions_;
}

actionsFolder::shared_ptr_t actionsFolder::getParent() const
{
	return parent_;
}

actionsFolder::folders_vec_t actionsFolder::getChildren() const
{
	return children_;
}

std::string actionsFolder::getPath() const
{
	if (parent_)
	{
		return parent_->getPath() + "/" + getName();
	}
	return "";
}

void actionsFolder::addAction(action::shared_ptr_t action)
{
	if (action)
	{
		action->setFolder(shared_from_this());
		actions_.push_back(action);
	}
}

void actionsFolder::deleteAction(uuids::uuid uuid)
{
	actions_.erase(std::remove_if(actions_.begin(), actions_.end(), [uuid](const action::shared_ptr_t& act) { return act->uuid() == uuid; }), actions_.end());
}

void actionsFolder::addFolder(shared_ptr_t folder)
{
	folder->parent_ = shared_from_this();
	children_.push_back(folder);
}

void actionsFolder::deleteFolder(uuids::uuid uuid)
{
	children_.erase(std::remove_if(children_.begin(), children_.end(), [uuid](const shared_ptr_t& folder) { return folder->uuid() == uuid; }), children_.end());
}

action::shared_ptr_t actionsFolder::searchByUuid(uuids::uuid uuid)
{
	auto it = std::find_if(actions_.begin(), actions_.end(), [uuid](const action::shared_ptr_t& act) { return act->uuid() == uuid; });
	return it != actions_.end() ? *it : nullptr;
}

actionsFolder::actions_vec_t actionsFolder::searchByAlias(const std::string& alias)
{
	actions_vec_t result;
	std::copy_if(actions_.begin(), actions_.end(), std::back_inserter(result),
		[&alias](const action::shared_ptr_t& act) { return act->getAlias().find(alias) != std::string::npos; });
	return result;
}

actionsFolder::actions_vec_t actionsFolder::searchByContent(const std::string& content)
{
	actions_vec_t result;
	std::copy_if(actions_.begin(), actions_.end(), std::back_inserter(result),
		[&content](const action::shared_ptr_t& act) { return act->getContent().find(content) != std::string::npos; });
	return result;
}

actionsFolder::actions_vec_t actionsFolder::searchByDescription(const std::string& description)
{
	actions_vec_t result;
	std::copy_if(actions_.begin(), actions_.end(), std::back_inserter(result),
		[&description](const action::shared_ptr_t& act) { return act->getDescription().find(description) != std::string::npos; });
	return result;
}

action::shared_ptr_t actionsFolder::recursiveSearchByUuid(uuids::uuid uuid)
{
	// Check current folder's actions
	if (auto action = searchByUuid(uuid))
		return action;

	// Recursively check child folders
	for (const auto& child : children_)
	{
		if (auto action = child->recursiveSearchByUuid(uuid))
			return action;
	}

	return nullptr;
}

actionsFolder::actions_vec_t actionsFolder::recursiveSearchByAlias(const std::string& alias)
{
	actions_vec_t result = searchByAlias(alias);

	for (const auto& child : children_)
	{
		auto childResults = child->recursiveSearchByAlias(alias);
		result.insert(result.end(), childResults.begin(), childResults.end());
	}

	return result;
}

actionsFolder::actions_vec_t actionsFolder::recursiveSearchByContent(const std::string& content)
{
	actions_vec_t result = searchByContent(content);

	for (const auto& child : children_)
	{
		auto childResults = child->recursiveSearchByContent(content);
		result.insert(result.end(), childResults.begin(), childResults.end());
	}

	return result;
}

actionsFolder::actions_vec_t actionsFolder::recursiveSearchByDescription(const std::string& description)
{
	actions_vec_t result = searchByDescription(description);

	for (const auto& child : children_)
	{
		auto childResults = child->recursiveSearchByDescription(description);
		result.insert(result.end(), childResults.begin(), childResults.end());
	}

	return result;
}

// actions implementation
actionsFolder::shared_ptr_t actions::getRoot()
{
	return root_;
}

actionsFolder::shared_ptr_t actions::getCurrent()
{
	return cur_;
}

void actions::folderUp()
{
	if (cur_ && cur_->getParent())
	{
		cur_ = cur_->getParent();
	}
}

void actions::folderDown(uuids::uuid uuid)
{
	if (!cur_)
		return;

	for (const auto& child : cur_->getChildren())
	{
		if (child->uuid() == uuid)
		{
			cur_ = child;
			return;
		}
	}
}

actionsFolder::actions_vec_t actions::searchByPath(const std::string& pathSnippet)
{
	actionsFolder::actions_vec_t results;

	if (!root_ || pathSnippet.empty())
	{
		return results;
	}

	std::function<void(const actionsFolder::shared_ptr_t, std::vector<std::string>)> searchRecursive;

	searchRecursive = [&](const actionsFolder::shared_ptr_t folder, std::vector<std::string> currentPath)
	{
		for (const auto& action : folder->getActions())
		{
			std::string actionPath;
			for (const auto& part : currentPath)
			{
				actionPath += "/" + part;
			}
			actionPath += "/" + action->getAlias();

			if (actionPath.find(pathSnippet) != std::string::npos)
			{
				results.push_back(action);
			}
		}

		for (const auto& child : folder->getChildren())
		{
			std::vector<std::string> newPath = currentPath;
			newPath.push_back(child->getName());
			searchRecursive(child, newPath);
		}
	};

	// Start search from root with empty path
	searchRecursive(root_, {});
	return results;
}

} // namespace data