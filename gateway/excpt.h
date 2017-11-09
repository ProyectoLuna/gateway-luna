/*
 * Excpt.h
 *
 *  Created on: Aug 11, 2017
 *      Author: migarna
 */

#ifndef EXCPT_H_
#define EXCPT_H_

#include <exception>
#include <string>

class Excpt: public std::exception {
protected:
	std::string _what;
public:
	Excpt();
	Excpt(char const * what);
	virtual ~Excpt() throw();
	virtual char const * what() const throw();
};

#endif /* EXCPT_H_ */
