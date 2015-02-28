#ifndef COMPBIO_ALIGNMENT_SMITHWATERMAN
#define COMPBIO_ALIGNMENT_SMITHWATERMAN

#include <string>
#include <tuple>

#include "needleman-wunsch.h"

namespace compbio {

class SmithWaterman : public NeedlemanWunsch {
    private:
        int global_max;
        int max_row;
        int max_col;

    public:
        SmithWaterman(const Sequence& seq1, const Sequence& seq2,
            const cost_function_t& cost_func);

        virtual ~SmithWaterman() { }

        virtual int computeScore(char x, char y, int row, int col);
        virtual std::tuple<std::string, std::string> align();
};

}

#endif

