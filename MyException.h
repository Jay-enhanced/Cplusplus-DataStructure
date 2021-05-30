#pragma once
#include <exception>
#include <string>

using namespace std;

class MyException : public exception
{
public:
	MyException() :message("Exception.") {}
	MyException(string str)
	{
		message = "Exception:" + str;
	}
	~MyException() noexcept {}
	virtual const char* what() const noexcept
	{
		return message.c_str();
	}

private:
	string message;
};

