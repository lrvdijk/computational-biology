#ifndef COMPBIO_ALIGNMENT_HIRSCHBERG_H
#define COMPBIO_ALIGNMENT_HIRSCHBERG_H

#include <string>
#include <memory>

#include "base.h"
#include "../sequence.h"

namespace compbio {

class HirschbergAlignment : public Aligner {
    private:
        Sequence& seq1;
        Sequence& seq2;

    public:
        HirschbergAlignment(Sequence& seq1, Sequence& seq2);

        virtual std::tuple<std::string, std::string> align();

    protected:
        std::tuple<std::unique_ptr<int[]>, std::unique_ptr<int[]> > initialize_conditions(bool reverse);
        std::unique_ptr<int[]> compute_matrix(int* max_score, bool reverse);
};

}

#endif

