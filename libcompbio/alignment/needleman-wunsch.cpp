#include <algorithm>
#include <sstream>

#include "needleman-wunsch.h"
#include "../exceptions.h"

using namespace compbio;
using std::vector;
using std::tuple;
using std::stringstream;
using std::string;

NeedlemanWunsch::NeedlemanWunsch(const Sequence& seq1, const Sequence& seq2,
    const cost_function_t& cost_func) : Aligner(seq1, seq2, cost_func)
{
    this->scores.resize(seq2.size()+1);
    for(auto& row : this->scores) {
        row.resize(seq1.size()+1, 0);
    }
}

void NeedlemanWunsch::initialConditions()
{
    int i = 1;
    // Initial conditions
    for(char x : this->seq1) {
        this->scores[0][i] = this->scores[0][i-1] + this->cost_function(x, '-');
        ++i;
    }

    i = 1;
    for(char y : this->seq2) {
        this->scores[i][0] = this->scores[i-1][0] + this->cost_function('-', y);
        ++i;
    }
}

int NeedlemanWunsch::computeScore(char x, char y, int row, int col)
{
    int score1 = this->scores[row-1][col-1] + this->cost_function(x, y);
    int score2 = this->scores[row-1][col] + this->cost_function(x, '-');
    int score3 = this->scores[row][col-1] + this->cost_function('-', y);

    return std::max({score1, score2, score3});
}

NeedlemanWunsch::Movement NeedlemanWunsch::getMove(int row, int col)
{
    if(row < 1 || col < 1) {
        throw AlignException("Row or column out of bounds.");
    }

    int score = this->scores[row][col];

    int score1 = this->scores[row-1][col-1] + this->cost_function(this->seq1[col-1], 
        this->seq2[row-1]);
    int score2 = this->scores[row-1][col] + this->cost_function(this->seq1[col-1], '-');
    int score3 = this->scores[row][col-1] + this->cost_function('-', this->seq2[row-1]);

    if(score == score1) {
        return NeedlemanWunsch::DIAGONAL;
    } else if(score == score2) {
        return NeedlemanWunsch::DOWN;
    } else {
        return NeedlemanWunsch::LEFT;
    }
}

void NeedlemanWunsch::computeMatrix()
{
    int row = 1;
    for(char y : this->seq2) {
        int col = 1;
        for(char x : this->seq1) {
            this->scores[row][col] = this->computeScore(x, y, row, col);
            ++col;
        }
        ++row;
    }
}

tuple<string, string> NeedlemanWunsch::align()
{
    stringstream alignment1;
    stringstream alignment2;

    this->initialConditions();
    this->computeMatrix();

    int col = this->seq1.size();
    int row = this->seq2.size();

    while(col > 0 && row > 0) {
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

    string align1(alignment1.str());
    string align2(alignment2.str());

    std::reverse(align1.begin(), align1.end());
    std::reverse(align2.begin(), align2.end());

    return std::make_tuple(align1, align2);
}
                
