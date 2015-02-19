#ifndef COMPBIO_EXCEPTIONS_H
#define COMPBIO_EXCEPTIONS_H

#include <stdexcept>

class NotImplemented : public std::runtime_error
{
};

class AlphabetException : public std::runtime_error
{
};

#endif
