#pragma once

#define LITTLE_ENDIAN 1

#include <string>
#include <fstream>
#include <vector>

class BrainfuckCompiler {
private:
	std::string m_file_name;
	std::vector<std::string> m_error_list;

	class LittleEndianOFStream : public std::ofstream
	{
	public:
		LittleEndianOFStream(const char* file_name) : std::ofstream(file_name, ios_base::out | ios_base::binary) {}
		template<typename T>
		LittleEndianOFStream & operator<<(const T data);
	};
public:
	BrainfuckCompiler(std::string file_name) : m_file_name(file_name) {}
	bool load_file(std::string file_name) { m_file_name = file_name; }
	bool compile(std::string out_name);
	const std::vector<std::string>& get_errors() const { return m_error_list; }
};

#if LITTLE_ENDIAN == 1
template<typename T>
inline BrainfuckCompiler::LittleEndianOFStream & BrainfuckCompiler::LittleEndianOFStream::operator<<(const T data)
{
	static_assert(CHAR_BIT == 8, "CHAR_BIT != 8");
	write(reinterpret_cast<const char*>(&data),sizeof(T));
	return *this;
}
#else
template<typename T>
BrainfuckCompiler::LittleEndianOFStream & BrainfuckCompiler::LittleEndianOFStream::operator<<(const T data)
{
	static_assert(CHAR_BIT == 8, "CHAR_BIT != 8");

	const unsigned char* ch = reinterpret_cast<const unsigned char*>(&data);
	for (int i = sizeof(T) - 1; i >= 0; --i)
	{
		put(reinterpret_cast<const unsigned char*>(&data)[i]);
	}
	return *this;
}
#endif