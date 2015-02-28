#include <boost/format.hpp>

#include "../sequence.h"
#include "../exceptions.h"
#include "base.h"

using namespace compbio;
using std::vector;

Aligner::Aligner(const Sequence& seq1, const Sequence& seq2, const cost_function_t& cost_func) : 
    seq1(seq1),
    seq2(seq2),
    cost_function(cost_func)
{
}

const cost_function_t& Aligner::getCostFunction() const
{
    return this->cost_function;
}
