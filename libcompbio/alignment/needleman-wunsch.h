#ifndef COMPBIO_ALIGNMENT_NEEDLEMANWUNSCH
#define COMPBIO_ALIGNMENT_NEEDLEMANWUNSCH

#include <string>
#include <tuple>
#include <vector>

#include "base.h"

namespace compbio {

class NeedlemanWunsch : public Aligner {
    protected:
        std::vector<std::vector<int> > scores;

    public:
        NeedlemanWunsch(const Sequence& seq1, const Sequence& seq2, 
            const cost_function_t& cost_func);
        virtual ~NeedlemanWunsch() { }

        virtual std::tuple<std::string, std::string> align();

        enum Movement {
            DIAGONAL,
            LEFT,
            DOWN
        };

        void computeMatrix();

        Movement getMove(int row, int col);
        virtual int computeScore(char x, char y, int row, int col);
        void initialConditions();
        
};

}

#endif

    


