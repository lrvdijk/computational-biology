#ifndef COMPBIO_UTILS_H
#define COMPBIO_UTILS_H

#include <string>
#include <random>

namespace compbio {

std::string string_format(const std::string fmt_str, ...);

std::default_random_engine& prng_engine();

}

#endif
