#ifndef TYPHON_EXCEPTION_H
#define TYPHON_EXCEPTION_H

#include <stdexcept>

enum TyphonExceptionType {
	NEGATIVE_DURATION
};

class TyphonException : std::exception {
public:
	TyphonException(TyphonExceptionType type) {
		switch (type) {
		case NEGATIVE_DURATION:
			std::invalid_argument("Can't integrate on negative duration");
		}
	}
	~TyphonException();
};

#endif TYPHON_EXCEPTION_H
