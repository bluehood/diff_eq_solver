#ifndef EXCEPTIONS
#define EXCEPTIONS

#include <exception>

class wrongDimension : public std::exception {
	public:
	const char* what() const throw(); 
};

class dimensionMismatch : public std::exception {
	public:
	const char* what() const throw(); 
};

#endif //EXCEPTIONS
