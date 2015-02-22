#include "base.h"

using namespace compbio;

cost_function_t& Aligner::getCostFunction()
{
    return this->cost_function;
}

void Aligner::setCostFunction(cost_function_t& func)
{
    this->cost_function = func;
}
