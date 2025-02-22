#pragma once

#include <set>
#include <uvw.hpp>

class CUvContainer {
public:
	void attach(CUvContainer* container) {
		containers_.insert(container);
		container->setCloseCallback([this](CUvContainer* container) {
			this->detach(container);
			});
	}

	void detach(CUvContainer* container) {
		if (containers_.erase(container)) {
			container->setCloseCallback(nullptr);
		}
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
		}
		
		return counter;
	}

	virtual void start() {}
	virtual void stop() {}
	virtual void close() {}
	virtual bool active() = 0;
protected:
	void setCloseCallback(std::function<void(CUvContainer*)> callback) {
		closeCallback_ = callback;
	}
private:
	std::set<CUvContainer*> containers_;
	std::function<void(CUvContainer*)> closeCallback_ = nullptr;
};