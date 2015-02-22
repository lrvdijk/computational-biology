#ifndef COMPBIO_ALIGNMENT_H
#define COMPBIO_ALIGNMENT_H

#include <memory>
#include <string>
#include <tuple>
#include "sequence.h"

namespace compbio {

class Aligner {
    public:
        virtual std::tuple<std::string, std::string> align() = 0;
};

class HirschbergAlignment : public Aligner {
    private:
        Sequence& seq1;
        Sequence& seq2;

    public:
        HirschbergAlignment(Sequence& seq1, Sequence& seq2);

        virtual std::tuple<std::string, std::string> align();

    protected:
        std::unique_ptr<int> forward_matrix();
        std::unique_ptr<int> backward_matrix();
};

}

#endif
