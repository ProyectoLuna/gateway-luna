#include "excpt.h"

Excpt::Excpt(char const *what):_what(what)
{
}

Excpt::~Excpt() throw()
{
}

char const *Excpt::what() const throw()
{
	return this->_what.c_str();
}
