#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <boost/range/adaptor/reversed.hpp>
#include "hirschberg.h"

using namespace compbio;
using namespace boost;
using std::stringstream;
using std::tuple;
using std::string;
using std::unique_ptr;

HirschbergAlignment::HirschbergAlignment(Sequence& seq1, Sequence& seq2,
    const cost_function_t& cost_func) : Aligner(cost_func), 
    seq1(seq1.size() < seq2.size() ? seq1 : seq2), 
    seq2(seq1.size() < seq2.size() ? seq2 : seq1)
{
}

unique_ptr<int[]> HirschbergAlignment::initialize_conditions(bool reverse=false)
{
    unique_ptr<int[]> initial_row(new int[this->seq1.size() + 1]);
    
    // Initial conditions
    if(reverse) {
        initial_row[0] = 0;
        for(Sequence::size_type i = this->seq1.size(); i > 0; --i) {
            int col = this->seq1.size() - i + 1;
            initial_row[col] = initial_row[col-1] + this->cost_function(this->seq1[i-1], '-');
        }
    } else {
        // Initial conditions
        initial_row[0] = 0;
        for(Sequence::size_type i = 1; i <= this->seq1.size(); ++i) {
            initial_row[i] = initial_row[i-1] + this->cost_function(this->seq1[i-1], '-');
        }
    }

    return initial_row;
}

unique_ptr<int[]> HirschbergAlignment::compute_matrix(int* max_score=nullptr, bool reverse=false) 
{
    // Get initial conditions
    this->previous_row = this->initialize_conditions(reverse);
    
    // Memory for the row we will be computing on
    this->current_row = unique_ptr<int[]>(new int[this->seq1.size() + 1]);

    // We need to store the values of row number `this->seq2.size()/2`
    unique_ptr<int[]> half_row(new int[this->seq1.size() + 1]);

    // Start computing the matrix
    if(reverse) {
        int row = 0;
        std::cout << "Backward matrix" << std::endl;
        std::cout << "     ";
        for(char elem : adaptors::reverse(this->seq1)) {
            std::cout << std::setw(2) << elem << " ";
        }
        std::cout << std::endl;
        std::cout << "  ";
        for(Sequence::size_type i = 0; i <= this->seq1.size(); ++i) {
            std::cout << std::setw(2) << this->previous_row[i] << " ";
        }
        std::cout << std::endl;
        for(char elem2 : adaptors::reverse(this->seq2)) {
            int col = 1;
            
            // Prepare initial conditions
            this->current_row[0] = this->previous_row[0] + this->cost_function('-', elem2);

            for(char elem1 : adaptors::reverse(this->seq1)) {
                int score1, score2, score3;
                std::tie(score1, score2, score3) = this->compute_scores(col, elem1, elem2);
                
                current_row[col] = std::max({score1, score2, score3});
                ++col;
            }
            
            std::cout << elem2 << " ";
            if(this->isHalfRow(row)) {
                half_row = this->finish_row(row);
            } else {
                this->finish_row(row);
            }

            ++row;
        }
    } else {
        int row = 0;
        std::cout << "Forward matrix" << std::endl;
        std::cout << "     ";
        for(char elem : this->seq1) {
            std::cout << std::setw(2) << elem << " ";
        }
        std::cout << std::endl;
        std::cout << "  ";
        for(Sequence::size_type i = 0; i <= this->seq1.size(); ++i) {
            std::cout << std::setw(2) << this->previous_row[i] << " ";
        }
        std::cout << std::endl;
        for(char elem2 : this->seq2) {
            int col = 1;

            // Prepare initial conditions
            this->current_row[0] = this->previous_row[0] + this->cost_function('-', elem2);

            for(char elem1 : this->seq1) {
                int score1, score2, score3;
                std::tie(score1, score2, score3) = this->compute_scores(col, elem1, elem2);
                
                this->current_row[col] = std::max({score1, score2, score3});
                ++col;
            }
            
            std::cout << elem2 << " ";
            if(this->isHalfRow(row)) {
                half_row = this->finish_row(row);
            } else {
                this->finish_row(row);
            }

            ++row;
        }
    }
    
    if(max_score != nullptr) {
        // Last row has been swapped to previous_row
        *max_score = previous_row[this->seq1.size()];
    }

    return half_row;
}

unique_ptr<int[]> HirschbergAlignment::compute_matrix(bool reverse=false)
{
    return this->compute_matrix(nullptr, reverse);
}

unique_ptr<int[]> HirschbergAlignment::finish_row(int row)
{
    // We're finished with a row
    // Swap previous and current row
    std::swap(this->current_row, this->previous_row);

    for(Sequence::size_type i = 0; i <= this->seq1.size(); ++i) {
        std::cout << std::setw(2) << this->previous_row[i] << " ";
    }
    std::cout << std::endl;

    
    // If we have processed the 'half-row' we copy the contents to some
    // separate memory, and return it
    if(this->isHalfRow(row)) {
        std::cout << "----------------" << std::endl;
        unique_ptr<int[]> half_row(new int[this->seq1.size() + 1]);
        for(Sequence::size_type i = 0; i <= this->seq1.size(); ++i) {
            half_row[i] = this->previous_row[i];
        }

        return half_row;
    } else {
        return nullptr;
    }
}

bool HirschbergAlignment::isHalfRow(int row)
{
    return row+1 == (this->seq2.size())/2;
}

tuple<int, int, int> HirschbergAlignment::compute_scores(int col, char a, char b)
{
    int score1 = this->previous_row[col-1] + this->cost_function(a, b);
    int score2 = this->previous_row[col] + this->cost_function(a, '-');
    int score3 = this->current_row[col-1] + this->cost_function('-', b);

    return tuple<int, int, int>(score1, score2, score3);
}
        
tuple<string, string> HirschbergAlignment::align()
{
    stringstream alignment1;
    stringstream alignment2;
    
    std::cout << "Sequence 1 length: " << this->seq1.size() << std::endl;
    std::cout << "Sequence 2 length: " << this->seq2.size() << std::endl;

    if(this->seq1.size() == 0) {
        for(char elem : this->seq2) {
            alignment1 << '-';
            alignment2 << elem;
        }
    } else if(this->seq2.size() == 0) {
        for(char elem : this->seq1) {
            alignment1 << elem;
            alignment2 << '-';
        }
    } else if(this->seq1.size() == 1 && this->seq2.size() == 1) {
        // We perform standard Needleman-Wunsch calculation
        // First the initial conditions
        this->previous_row = this->initialize_conditions();
        this->current_row = unique_ptr<int[]>(new int[this->seq1.size() + 1]);
        this->current_row[0] = this->cost_function('-', this->seq2[0]);

        int score1, score2, score3;
        std::tie(score1, score2, score3) = this->compute_scores(1, this->seq1[0], this->seq2[0]);

        int max_score = std::max({score1, score2, score3});

        if(max_score == score1) {
            alignment1 << this->seq1[0];
            alignment2 << this->seq2[0];
        } else if(max_score == score2) {
            alignment1 << this->seq1[0];
            alignment2 << '-';
        } else if(max_score == score3) {
            alignment1 << '-';
            alignment2 << this->seq2[0];
        }
    } else {
        // Partition the problem
        int max_score = 0;
        auto forward_row = this->compute_matrix(&max_score);
        auto backward_row = this->compute_matrix(true);

        // Find the column where we're going to split
        // (and where the forward and backward matrix meet)
        bool found = false;
        Sequence::size_type k;
        std::cout << "Max score: " << max_score << std::endl;
        for(k = 0; k <= this->seq1.size(); ++k) {
            std::cout << forward_row[k] << " + " << backward_row[this->seq1.size() - k]; 
            std::cout << " = " << forward_row[k] + backward_row[this->seq1.size() -  k];
            std::cout << std::endl;
            if(forward_row[k] + backward_row[this->seq1.size() - k] == max_score) {
                found = true;
                break;
            }
        }

        // Free some memory because it's not needed anymore.
        forward_row.release();
        backward_row.release();
        this->current_row.release();
        this->previous_row.release();

        std::cout << k << " " << found << std::endl;
        Sequence subseq1_left = this->seq1.substr(0, k);
        Sequence subseq1_right = this->seq1.substr(k);

        Sequence subseq2_top = this->seq2.substr(0, this->seq2.size() / 2);
        Sequence subseq2_bottom = this-> seq2.substr(this->seq2.size() / 2);

        // Start new hirschberg alignments with the partitioned sequences
        string subalign1;
        string subalign2;
        
        HirschbergAlignment hirsch1(subseq1_left, subseq2_top, this->cost_function);
        std::tie(subalign1, subalign2) = hirsch1.align();
        alignment1 << subalign1;
        alignment2 << subalign2;

        HirschbergAlignment hirsch2(subseq1_right, subseq2_bottom, this->cost_function);
        std::tie(subalign1, subalign2) = hirsch2.align();
        alignment1 << subalign1;
        alignment2 << subalign2;
    }

    return tuple<string, string>(alignment1.str(), alignment2.str());
}
