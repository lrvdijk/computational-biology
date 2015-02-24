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

        std::vector<int> previous_row;
        std::vector<int> current_row;

        inline std::string prefix();

    public:
        HirschbergAlignment(Sequence& seq1, Sequence& seq2,
            const cost_function_t& cost_func);
        HirschbergAlignment(Sequence& seq1, Sequence& seq2,
            const cost_function_t& cost_func, int depth);

        virtual std::tuple<std::string, std::string> align();

    protected:
        std::vector<int> initialize_conditions(bool reverse);
        std::vector<int> compute_matrix(int* max_score, bool reverse);
        std::vector<int> compute_matrix(bool reverse);
        std::tuple<int, int, int> compute_scores(int col, char a, char b);
        std::vector<int> finish_row(int row);
        bool isHalfRow(int row);

};

}

#endif

