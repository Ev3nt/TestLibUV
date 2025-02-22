#pragma once

#include "uvw.hpp"
#include "CUvContainer.h"

class Timer : public CUvContainer {
public:
	enum class timer_mode : UINT {
		DEFAULT,
		ONCE
	};

	Timer(std::function<void()> callback, uvw::loop& loop) {
		callback_ = [callback]() {
			callback();
			};

		bind(loop);
	}

	Timer(std::function<void(Timer&)> callback, uvw::loop& loop) {
		callback_ = [this, callback]() {
			callback(*this);
			};

		bind(loop);
	}

	~Timer() {
		CUvContainer::close();
		timer_->close();
	}

	void stop() {
		CUvContainer::stop();
		timer_->stop();
	}

	void start(uvw::timer_handle::time time, timer_mode mode = timer_mode::DEFAULT) {
		CUvContainer::start();
		timer_->start(time, mode == timer_mode::ONCE ? uvw::timer_handle::time() : time);
	}

	bool active() {
		return timer_->active();
	}

private:
	void bind(uvw::loop& loop) {
		timer_ = loop.resource<uvw::timer_handle>();

		timer_->on<uvw::timer_event>([this](const uvw::timer_event, uvw::timer_handle&) {
			callback_();
			});
	}

	std::function<void()> callback_ = nullptr;

	std::shared_ptr<uvw::timer_handle> timer_;
};