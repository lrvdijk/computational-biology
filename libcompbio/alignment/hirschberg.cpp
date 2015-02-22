#include "alignment.h"

using namespace compbio;
using std::tuple;
using std::string;
using std::unique_ptr;

HirschbergAlignment::HirschbergAlignment(Sequence& seq1, Sequence& seq2) :
    seq1(seq1), seq2(seq2)
{
}

std::unique_ptr<int> HirschbergAlignment::forward_matrix() {
}

