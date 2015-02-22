#ifndef COMPBIO_ALIGNMENT_H
#define COMPBIO_ALIGNMENT_H

#include <functional>
#include <string>
#include <tuple>

namespace compbio {

typedef std::function<int(char a, char b)> cost_function_t;

class Aligner {
    protected:
        cost_function_t cost_function;

    public:
        virtual std::tuple<std::string, std::string> align() = 0;

        virtual cost_function_t& getCostFunction();
        virtual void setCostFunction(cost_function_t& cost_function);
};

}

#endif
