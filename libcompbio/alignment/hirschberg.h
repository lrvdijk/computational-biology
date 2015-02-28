#ifndef COMPBIO_ALIGNMENT_HIRSCHBERG_H
#define COMPBIO_ALIGNMENT_HIRSCHBERG_H

#include <string>
#include <vector>
#include <memory>

#include "base.h"
#include "../sequence.h"

namespace compbio {

class HirschbergAlignment : public Aligner {
    public:
        HirschbergAlignment(const Sequence& seq1, const Sequence& seq2,
            const cost_function_t& cost_func);
        virtual ~HirschbergAlignment() { }

        virtual std::tuple<std::string, std::string> align();

    protected:
        template<typename iter>
        std::tuple<std::string, std::string> compute(iter seq1_l, iter seq1_h, 
            iter seq2_l, iter seq2_h, int depth);

        template<typename iter>
        std::vector<int> computeMatrix(iter seq1_l, iter seq1_h, iter seq2_l, iter seq2_h);

        template<typename iter>
        std::vector<int> initialConditions(iter seq1_l, iter seq1_h);
};

}

#endif

