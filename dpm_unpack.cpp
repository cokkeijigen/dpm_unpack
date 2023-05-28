#define _CRT_SECURE_NO_WARNINGS
#define byte unsigned char
#include <iostream>
#include "readbuffer.h"
#include "writebuffer.h"

int main(int argc, char const* argv[])
{
	if (argc != 2) return 0;
	ReadBuffer rb;
	WriteBuffer wb;
	std::string filePath(argv[1]);
	rb.reader(filePath);

	filePath = filePath.substr(filePath.find_last_of("\\") + 1);
	filePath = filePath.substr(0, filePath.find_last_of("."));
	system((std::string("MD \"") + filePath + "\"").c_str());

	if (rb.hasData()) {
		std::cout << "size: " << rb.fsize() << std::endl;
		int offset = rb.getInt32(8) * 32 + 16;
		int pos = 16, foffset = 0, fsize = 0;
		byte name[16] = { 0 };

		while (pos != offset) {
			rb.get(name, pos, 16);
			foffset = rb.getInt32(pos + 24);
			fsize = rb.getInt32(pos + 28);
			wb.reset();
			wb.write(rb, offset + foffset, fsize);
			wb.outFile(std::string(filePath + "\\").append((char*)name).c_str());
			std::cout << name << std::endl;
			pos += 32;
		}
	}
	else {
		std::cout << "err" << std::endl;
	}
	system("pause");
	return 0;
}
