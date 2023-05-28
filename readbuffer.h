#pragma once

class ReadBuffer {
	byte* buffer;
	long size;
	int position;

	void init() {
		if (this->buffer) delete[] buffer;
		this->buffer = nullptr;
		this->position = 0;
		this->size = 0;
	}

	ReadBuffer* reader(FILE* file) {
		if (file) {
			fseek(file, 0, SEEK_END);
			this->size = ftell(file);
			if (!this->size) return this;
			this->buffer = new byte[this->size];
			fseek(file, 0, SEEK_SET);
			fread(this->buffer, 1, this->size, file);
			this->position = 0;
			fclose(file);
		}
		return this;
	}

public:
	ReadBuffer() {
		this->init();
	}

	~ReadBuffer() {
		this->init();
	}

	ReadBuffer* reader(std::string filepath) {
		this->init();
		return this->reader(fopen(filepath.c_str(), "rb"));
	}

	bool hasData() {
		return this->size && this->buffer;
	}

	void offset(int pos) {
		this->position = pos;
	}

	bool next(byte& bf) {
		if (this->position > this->size - 1)
			return false;
		bf = this->buffer[this->position++];
		return true;
	}

	bool get(byte* bf, int pos, int len) {
		if (pos > this->size || pos + len > this->size)
			return false;
		memcpy(bf, &this->buffer[pos], len);
		return true;
	}

	int getInt32(int pos, bool bigEndian = false) {
		byte tmp[4] = { 0 };
		this->get(tmp, pos, 4);
		int result = bigEndian ?
			(tmp[0] << 24) | (tmp[1] << 16) | (tmp[2] << 8) | tmp[3] :
			(tmp[3] << 24) | (tmp[2] << 16) | (tmp[1] << 8) | tmp[0];
		return result;
	}

	byte get(int pos) {
		return this->buffer[pos];
	}

	long fsize() {
		return this->size;
	}
};
