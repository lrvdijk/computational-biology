#include <algorithm>
#include <sstream>

#include "smith-waterman.h"
#include "../exceptions.h"

using namespace compbio;
using std::vector;
using std::tuple;
using std::stringstream;
using std::string;

SmithWaterman::SmithWaterman(const Sequence& seq1, const Sequence& seq2,
    const cost_function_t& cost_func) : NeedlemanWunsch(seq1, seq2, cost_func),
    global_max(-9999999),
    max_row(-1),
    max_col(-1)
{
}

int SmithWaterman::computeScore(char x, char y, int row, int col)
{
    int score = NeedlemanWunsch::computeScore(x, y, row, col);

    if(score > this->global_max) {
        this->max_row = row;
        this->max_col = col;
        this->global_max = score;
    }
        
    return score;
}

tuple<string, string> SmithWaterman::align()
{
    // Part 1: find local suffix alignment
    stringstream alignment1;
    stringstream alignment2;

    this->computeMatrix();

    int col = this->max_col;
    int row = this->max_row;

    while(col > 0 && row > 0) {
        if(this->scores[row][col] == 0) {
            break;
        } else {
            switch(this->getMove(row, col)) {
                case NeedlemanWunsch::DIAGONAL:
                    alignment1 << this->seq1[col-1];
                    alignment2 << this->seq2[row-1];

                    --col;
                    --row;
                break;
                case NeedlemanWunsch::LEFT:
                    alignment1 << this->seq1[col-1];
                    alignment2 << '-';

                    --col;
                break;
                case NeedlemanWunsch::DOWN:
                    alignment1 << '-';
                    alignment2 << this->seq2[row-1];

                    --row;
                break;
            }
        }
    }

    string align1(alignment1.str());
    string align2(alignment2.str());

    std::reverse(align1.begin(), align1.end());
    std::reverse(align2.begin(), align2.end());

    // Part 2: Align these local suffix alignments
    NeedlemanWunsch aligner(align1, align2, this->cost_function);

    return aligner.align();
}


