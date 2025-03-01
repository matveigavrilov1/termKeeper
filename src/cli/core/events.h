#pragma once

#include <memory>

#include "event.h"
#include "cli/core/interface.h"
#include "utils/logger.h"
namespace tk
{
class exitEvent : public event
{
public:
	exitEvent()
	: event(EXIT_EVENT)
	{ }

	void defaultHandler(shared_ptr_type) override { LOG_DBG("Exit event default handler"); cli::core::getEventManager().stop(); }
};

class inputEvent : public event
{
public:
	template<typename T>
	class inputData : public eventData
	{
	public:
		enum inputType
		{
			NONE,
			KEY_PRESSED,
			TEXT_PRINTED
		};

		inputData(T& input)
		: input_(input)
		{ }

		inputData(T&& input) noexcept
		: input_(std::move(input))
		{ }

		T& input() { return input_; }


	private:
		T input_;
		inputType type_ { NONE };
	};

	template<typename DataT>
	inputEvent(std::shared_ptr<inputData<DataT>> data)
	: event(INPUT_EVENT, data)
	{ }

	void defaultHandler(shared_ptr_type event) override
	{
		auto activeWindow = cli::core::getScreen().activeWindow();
		LOG_DBG("Input event default handler");
		if (activeWindow)
			activeWindow->handleInputEvent(event);
	}
};

class showWindowEvent : public event
{
public:
	class windowData : public eventData
	{
	public:
		windowData(const std::string& windowName)
		: windowName_(windowName)
		{ }

		windowData(std::string&& windowName)
		: windowName_(std::move(windowName))
		{ }

		std::string windowName() const { return windowName_; }

	private:
		std::string windowName_;
	};

	showWindowEvent(std::shared_ptr<windowData> data)
	: event(SHOW_WINDOW_EVENT, data)
	{ }

	void defaultHandler(shared_ptr_type event) override
	{
		LOG_DBG("Show window event default handler");
		auto eventData = std::dynamic_pointer_cast<windowData>(event->data());
		if (eventData)
		{
			cli::core::getScreen().showWindow(eventData->windowName(), cli::core::getConsoleManager());
		}
	}
};

class showScreenEvent : public event
{
public:
	showScreenEvent()
	: event(SHOW_SCREEN_EVENT)
	{ }

	void defaultHandler(eventData::shared_ptr_type) { LOG_DBG("Show screen event default handler"); cli::core::getScreen().show(cli::core::getConsoleManager()); }
};

class windowActivateEvent : public event
{
public:
	class windowActivateData : public eventData
	{
	public:
		windowActivateData(const std::string& windowName)
		: windowName_(windowName)
		{ }

		windowActivateData(std::string&& windowName)
		: windowName_(std::move(windowName))
		{ }

		std::string windowName() const { return windowName_; }

	private:
		std::string windowName_;
	};

	windowActivateEvent(std::shared_ptr<windowActivateData> data)
	: event(WINDOW_ACTIVATE_EVENT, data)
	{ }

	void defaultHandler(shared_ptr_type event) override
	{
		LOG_DBG("Window activate event default handler");
		auto eventData = std::dynamic_pointer_cast<windowActivateData>(event->data());
		if (eventData)
		{
			
			cli::core::getScreen().activateWindow(eventData->windowName());
		}
	}
};

class changeActiveWindowEvent : public event
{
public:
	class changeActiveWindowData : public eventData
	{
	public:
		changeActiveWindowData(const std::string& windowName)
		: windowName_(windowName)
		{ }

		changeActiveWindowData(std::string&& windowName)
		: windowName_(std::move(windowName))
		{ }

		std::string windowName() const { return windowName_; }

	private:
		std::string windowName_;
	};

	changeActiveWindowEvent(std::shared_ptr<changeActiveWindowData> data)
	: event(CHANGE_ACTIVE_WINDOW_EVENT, data)
	{ }

	void defaultHandler(shared_ptr_type event) override
	{
		LOG_DBG("Change active window event default handler");
		auto eventData = std::dynamic_pointer_cast<changeActiveWindowData>(event->data());
		if (eventData)
		{
			cli::core::getScreen().changeActiveWindow(eventData->windowName());
		}
	}
};
} // namespace tk