#pragma once
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <exception>

class Exception : public std::exception
{
protected:
	std::string msg_;
	int n_;
public:
	/** Constructor (C++ STL strings).
	 *  @param message The error message.
	 */
	explicit Exception(const std::string message, int n = 0) :
		msg_(message), n_(n)
	{
		if (n_)
		{
			char buf[80];
			sprintf(buf, "Error code=%d", n_);
			msg_ += buf;
		}
	}

	/** Destructor.
	 * Virtual to allow for subclassing.
	 */
	virtual ~Exception() throw () {}

	/** Returns a pointer to the (constant) error description.
	 *  @return A pointer to a const char*. The underlying memory
	 *          is in posession of the Exception object. Callers must
	 *          not attempt to free the memory.
	 */
	virtual const char* what() const throw () {
		return msg_.c_str();
	}
};