#pragma once


struct BelowRange : public std::exception
{
	const char* what() const noexcept
	{
		return "Below min range";
	}
};
struct AboveRange : public std::exception
{
	const char* what() const noexcept
	{
		return "Above max range";
	}
};

struct OutOfRange : public std::exception
{
	const int bound;

	OutOfRange(int _bound) : bound(_bound)
	{

	}

	const char* what() const noexcept
	{
		return "Index out of range";
	}
};


struct FileIOError : public std::exception
{
	using std::exception::exception;
};