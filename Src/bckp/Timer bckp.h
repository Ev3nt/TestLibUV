#pragma once

#include "uvw.hpp"

enum class timer_mode : UINT {
	DEFAULT,
	ONCE
};

template <typename T>
class Timer {
public:
	Timer(T* instance, void (T::* callback)(), uvw::loop& loop): instance_(instance), callback_(callback) {
		timer_ = loop.resource<uvw::timer_handle>();

		timer_->on<uvw::timer_event>([this](const uvw::timer_event, uvw::timer_handle&) {
				(instance_->*callback_)();
			});
	}

	Timer(T* instance, void (T::* callback)(Timer&), uvw::loop& loop) : instance_(instance), callback2_(callback) {
		timer_ = loop.resource<uvw::timer_handle>();

		timer_->on<uvw::timer_event>([this](const uvw::timer_event, uvw::timer_handle&) {
				(instance_->*callback2_)(*this);
			});
	}

	~Timer() {
		timer_->close();
	}

	void stop() {
		timer_->stop();
	}

	void start(uvw::timer_handle::time time, timer_mode mode = timer_mode::DEFAULT) {
		timer_->start(time, mode == timer_mode::ONCE ? uvw::timer_handle::time() : time);
	}

private:
	T* instance_ = nullptr;
	void (T::* callback_)() = nullptr;
	void (T::* callback2_)(Timer&) = nullptr;

	std::shared_ptr<uvw::timer_handle> timer_;
};