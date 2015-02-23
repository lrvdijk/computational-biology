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

        std::unique_ptr<int[]> previous_row;
        std::unique_ptr<int[]> current_row;

    public:
        HirschbergAlignment(Sequence& seq1, Sequence& seq2,
            const cost_function_t& cost_func);

        virtual std::tuple<std::string, std::string> align();

    protected:
        std::unique_ptr<int[]> initialize_conditions(bool reverse);
        std::unique_ptr<int[]> compute_matrix(int* max_score, bool reverse);
        std::unique_ptr<int[]> compute_matrix(bool reverse);
        std::tuple<int, int, int> compute_scores(int col, char a, char b);
};

}

#endif

