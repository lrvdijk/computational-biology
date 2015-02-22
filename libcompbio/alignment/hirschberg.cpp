#include <algorithm>
#include "hirschberg.h"

using namespace compbio;
using std::tuple;
using std::string;
using std::unique_ptr;

HirschbergAlignment::HirschbergAlignment(Sequence& seq1, Sequence& seq2) :
    seq1(seq1.size() < seq2.size() ? seq1 : seq2), 
    seq2(seq1.size() < seq2.size() ? seq2 : seq1)
{
}

tuple<unique_ptr<int[]>, unique_ptr<int[]> > HirschbergAlignment::initialize_conditions(bool reverse=false)
{
    unique_ptr<int[]> initial_row(new int[this->seq1.size() + 1]);
    unique_ptr<int[]> initial_col(new int[this->seq2.size() + 1]);
    
    // Initial conditions
    for(Sequence::size_type i = 0; i <= this->seq1.size(); ++i) {
        if(i == 0) {
            initial_row[i] = 0;
        } else {
            initial_row[i] = initial_row[i - 1] + this->cost_function(this->seq1[i], '-');
        }
    }

    // The same for the initial column
    for(Sequence::size_type i = 0; i <= this->seq2.size(); ++i) {
        if(i == 0) {
            initial_col[i] = 0;
        } else {
            initial_col[i] = initial_col[i - 1] + this->cost_function('-', this->seq2[i]);
        }
    }

    return tuple<unique_ptr<int[]>, unique_ptr<int[]> >(
        std::move(initial_row),
        std::move(initial_col)
    );
}

unique_ptr<int[]> HirschbergAlignment::compute_matrix(int* max_score=nullptr, bool reverse=false) 
{
    // Get initial conditions
    unique_ptr<int[]> previous_row;
    unique_ptr<int[]> initial_col;

    std::tie(previous_row, initial_col) = this->initialize_conditions();
    
    // Memory for the row we will be computing on
    unique_ptr<int[]> current_row(new int[this->seq1.size() + 1]);

    // We need to store the values of row number `this->seq2.size()/2`
    unique_ptr<int[]> half_row(new int[this->seq1.size() + 1]);

    // Start computing the matrix
    int row = 1;
    auto iter1 = reverse ? this->seq1.rbegin() : this->seq1.begin();
    auto iter1_end = reverse ? this->seq1.rend() : this->seq1.end();
    for(; iter1 != iter1_end; ++iter1)
        int col = 1;
        auto iter2 = reverse ? this->seq2.rbegin() : this->seq2.begin();
        auto iter2_end = reverse ? this->seq2.rend() : this->seq2.end();

        for(; iter2 != iter2_end; ++iter2) {
            int score1 = previous_row[col - 1] + this->cost_function(*iter1, *iter2);
            int score2 = previous_row[col] + this->cost_function(*iter1, '-');
            int score3 = current_row[col - 1] + this->cost_function('-', *iter2);

            current_row[col] = std::max(score1, score2, score3);
        
            ++col;
        }

        // We're finished with a row
        // Swap previous and current row
        std::swap(current_row, previous_row);

        // Make sure initial conditions are maintained.
        current_row[0] = initial_col[row];

        ++row;
    }

    if(max_score != nullptr) {
        *max_score = current_row[this->seq1.size()];
    }

    return half_row;
}


        
