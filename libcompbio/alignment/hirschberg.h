#ifndef COMPBIO_ALIGNMENT_HIRSCHBERG_H
#define COMPBIO_ALIGNMENT_HIRSCHBERG_H

#include <string>
#include <vector>
#include <memory>

#include "base.h"
#include "../sequence.h"

namespace compbio {

class HirschbergAlignment : public Aligner {
    private:
        int depth;

        Sequence& seq1;
        Sequence& seq2;

        inline std::string prefix();

    public:
        HirschbergAlignment(Sequence& seq1, Sequence& seq2,
            const cost_function_t& cost_func);
        HirschbergAlignment(Sequence& seq1, Sequence& seq2,
            const cost_function_t& cost_func, int depth);

        virtual std::tuple<std::string, std::string> align();

    protected:
        template<typename iter>
        std::tuple<std::string, std::string> compute(iter seq1_l, iter seq1_h, 
            iter seq2_l, iter seq2_h);

        template<typename iter>
        std::vector<int> computeMatrix(iter seq1_l, iter seq1_h, iter seq2_l, iter seq2_h);

        template<typename iter>
        std::vector<int> initialConditions(iter seq1_l, iter seq1_h);
};

}

#endif

