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

unique_ptr<int> HirschbergAlignment::forward_matrix() 
{
    // Allocate memory for computing the matrix
    unique_ptr<int[]> previous_row(new int[this->seq1.size() + 1]);
    unique_ptr<int[]> intial_col(new int[this->seq2.size() + 1]);
    unique_ptr<int[]> current_row(new int[this->seq1.size() + 1]);
    unique_ptr<int[]> half_row(new int[this->seq2.size() + 1]);

    // Initial conditions
    for(string::size_type i = 0; i <= this->seq1.size(); ++i) {
        int initial_cost = 0;
        for(char elem : this->seq1.substr(0, i)) {
            initial_cost += this->cost_function(elem, '-');
        }
        
        previous_row[i] = initial_cost;
    }
        
}

