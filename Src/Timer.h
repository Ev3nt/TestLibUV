#pragma once

#include "uvw.hpp"
#include "UvContainer.h"

class Timer : public UvContainer<uvw::timer_handle> {
public:
	enum class timer_mode : UINT {
		DEFAULT,
		ONCE
	};

	static std::shared_ptr<Timer> create(std::function<void()> callback, uvw::loop& loop) {
		auto ref = std::shared_ptr<Timer>(new Timer);

		ref->callback_ = [callback]() {
			callback();
			};

		ref->bind(loop);

		return ref;
	}

	static std::shared_ptr<Timer> create(std::function<void(Timer&)> callback, uvw::loop& loop) {
		auto ref = std::shared_ptr<Timer>(new Timer);

		ref->callback_ = [ref, callback]() {
			callback(*ref);
			};

		ref->bind(loop);

		return ref;
	}

	~Timer() {
		close();
	}

	void close() {
		timer_->close();
	}

	void stop() {
		timer_->stop();
	}

	void start(uvw::timer_handle::time time, timer_mode mode = timer_mode::DEFAULT) {
		timer_->start(time, mode == timer_mode::ONCE ? uvw::timer_handle::time() : time);
	}

	bool closing() {
		return timer_->closing();
	}

	bool active() {
		return timer_->active();
	}

private:
	Timer() = default;

	void bind(uvw::loop& loop) {
		timer_ = resource(loop);

		timer_->on<uvw::timer_event>([this](const uvw::timer_event, uvw::timer_handle&) {
			callback_();
			});
	}

	std::function<void()> callback_ = nullptr;

	std::shared_ptr<uvw::timer_handle> timer_;
};