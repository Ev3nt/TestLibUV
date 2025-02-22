#pragma once

#include <set>
#include <memory>
#include <uvw.hpp>

template<typename T>
class UvContainer {
public:
	void attach(std::shared_ptr<UvContainer> container) {
		if (!getContainerCloseCallback(container)) {
			containers_.insert(container);
			container->setContainerCloseCallback([this, container]() {
				this->detach(container);
				});
		}
	}

	void detach(std::shared_ptr<UvContainer> container) {
		if (containers_.erase(container)) {
			container->setContainerCloseCallback(nullptr);
		}
	}

	void setCloseCallback(std::function<void()> closeCallback) {
		closeCallback_ = closeCallback;
	}

	size_t getContainersCount() {
		return containers_.size();
	}

	size_t getActiveContainersCount() {
		size_t counter = 0;
		for (auto container : containers_) {
			if (container->active()) {
				counter++;
			}

			counter += container->getActiveContainersCount();
		}

		return counter;
	}

	template<typename... Args>
	std::shared_ptr<T> resource(uvw::loop& loop, Args &&...args) {
		auto ref = loop.resource<T>(std::forward<Args>(args)...);

		ref->on<uvw::close_event>([this](const uvw::close_event, T&) {
			UvContainer::close();
			});

		return ref;
	}

	virtual void start() {}
	virtual void stop() {}
	virtual void close() {
		for (auto container : containers_) {
			if (!container->closing()) {
				container->close();
			}
		}

		// TODO: close callback
		if (closeCallback_) {
			closeCallback_();
		}

		if (containerCloseCallback_) {
			containerCloseCallback_();
		}
	}

	virtual bool closing() {
		bool result = true;

		for (auto container : containers_) {
			result &= container->closing();
		}

		return result;
	}
	//{
	//	// TODO: if every child cont closed

	//	return true;
	//}

	virtual bool active() {
		for (auto container : containers_) {
			if (container->active()) {
				return true;
			}
		}

		return false;
	}
	//{
	//	// TODO: if any child cont active

	//	return false;
	//}

private:
	void setContainerCloseCallback(std::function<void()> callback) {
		containerCloseCallback_ = callback;
	}

	std::function<void()> getContainerCloseCallback(std::shared_ptr<UvContainer> container) {
		return container->containerCloseCallback_;
	}

	std::set<std::shared_ptr<UvContainer>> containers_;
	std::function<void()> containerCloseCallback_ = nullptr;
	std::function<void()> closeCallback_ = nullptr;
};