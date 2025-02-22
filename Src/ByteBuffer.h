#pragma once

#include <streambuf>
#include <vector>

// TODO: stream extends
class ByteBuffer {
public:
	ByteBuffer() = default;

	ByteBuffer& operator<<(char value) {
		buffer_.push_back(value);

		return *this;
	}

	template<typename T>
	ByteBuffer& operator<<(T value) {
		for (int i = 0; i < sizeof(T); i++) {
			buffer_.push_back(*(((char*)&value) + i));
		}

		return *this;
	}

	template<typename T>
	ByteBuffer& operator>>(T& value) {
		if (sizeof(T) > buffer_.size()) {
			while (sizeof(T) > buffer_.size()) {
				buffer_.push_back(NULL);
			}
		}

		value = *(T*)&buffer_[0];

		buffer_.erase(buffer_.begin(), buffer_.begin() + sizeof(T));

		return *this;
	}

private:
	std::vector<char> buffer_;
};