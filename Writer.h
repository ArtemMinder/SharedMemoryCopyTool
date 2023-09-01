#pragma once
#include <fstream>

class Writer
{
public:
	Writer() = default;
	Writer(const std::string& path) { open(path); }
	Writer(Writer&&) = delete;
	Writer& operator=(const Writer&) = delete;
	Writer& operator=(Writer&&) = delete;

	~Writer()
	{
		if (_file)
			_file.close();
	}

	void open(const std::string& path)
	{
		_file.open(path, std::ios_base::binary);
		if (!_file)
			throw std::runtime_error("Target file not found");
	}

	void write(const std::string data)
	{
		_file << data.data();
	}

	void close()
	{
		_file.close();
	}

private:
	std::ofstream _file;
};