#ifndef COMPBIO_ALIGNMENT_H
#define COMPBIO_ALIGNMENT_H

#include <functional>
#include <string>
#include <tuple>

namespace compbio {

typedef std::function<int(char a, char b)> cost_function_t;

class Aligner {
    protected:
        const cost_function_t& cost_function;

    public:
        Aligner(const cost_function_t& cost_func);

        virtual std::tuple<std::string, std::string> align() = 0;
        virtual const cost_function_t& getCostFunction() const;
};

}

#endif
