#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <boost/range/adaptor/reversed.hpp>

#include "../exceptions.h"
#include "hirschberg.h"

using namespace compbio;
using namespace boost;
using std::stringstream;
using std::tuple;
using std::string;
using std::vector;
using std::unique_ptr;

HirschbergAlignment::HirschbergAlignment(Sequence& seq1, Sequence& seq2,
    const cost_function_t& cost_func) : Aligner(cost_func), 
    depth(0),
    seq1(seq1.size() < seq2.size() ? seq1 : seq2), 
    seq2(seq1.size() < seq2.size() ? seq2 : seq1)
{
}

HirschbergAlignment::HirschbergAlignment(Sequence& seq1, Sequence& seq2,
    const cost_function_t& cost_func, int depth) : Aligner(cost_func), 
    depth(depth),
    seq1(seq1), 
    seq2(seq2)
{
}

vector<int> HirschbergAlignment::initialize_conditions(bool reverse=false)
{
    vector<int> initial_row;
    initial_row.reserve(this->seq1.size()+1);
    
    // Initial conditions
    initial_row.push_back(0);
    int index = 1;
    if(reverse) {
        for(char elem : adaptors::reverse(this->seq1)) {
            initial_row.push_back(initial_row[index - 1] + this->cost_function(elem, '-'));
            ++index;
        }
    } else {
        // Initial conditions
        for(char elem : this->seq1) {
            initial_row.push_back(initial_row[index - 1] + this->cost_function(elem, '-'));
            ++index;
        }
    }

    return initial_row;
}

vector<int> HirschbergAlignment::compute_matrix(int* max_score=nullptr, bool reverse=false) 
{
    // Get initial conditions
    this->previous_row = this->initialize_conditions(reverse);
    
    // Memory for the row we will be computing on
    this->current_row.resize(this->seq1.size()+1, 0);

    // Save memory for the half row
    vector<int> half_row;

    // Start computing the matrix
    if(reverse) {
        int row = 0;
        std::cout << this->prefix() << "Backward matrix" << std::endl;
        std::cout << this->prefix() << "     ";
        for(char elem : adaptors::reverse(this->seq1)) {
            std::cout << std::setw(2) << elem << " ";
        }
        std::cout << std::endl;
        std::cout << this->prefix() << "  ";
        for(int elem : this->previous_row) {
            std::cout << std::setw(2) << elem << " ";
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
            
            std::cout << this->prefix() << elem2;
            if(this->isHalfRow(row)) {
                std::cout << "-";
                half_row = this->finish_row();
            } else {
                std::cout << " ";
                this->finish_row();
            }

            ++row;
        }
    } else {
        int row = 0;
        std::cout << this->prefix() << "Forward matrix" << std::endl;
        std::cout << this->prefix() << "     ";
        for(char elem : this->seq1) {
            std::cout << std::setw(2) << elem << " ";
        }
        std::cout << std::endl;
        std::cout << this->prefix() << "  ";
        for(int elem : this->previous_row) {
            std::cout << std::setw(2) << elem << " ";
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
            
            std::cout << this->prefix() << elem2;
            if(this->isHalfRow(row)) {
                std::cout << "-";
                half_row = this->finish_row();
            } else {
                std::cout << " ";
                this->finish_row();
            }

            ++row;
        }
    }

    if(max_score != nullptr) {
        *max_score = this->previous_row[this->seq1.size()];
    }

    return half_row;
}

vector<int> HirschbergAlignment::compute_matrix(bool reverse=false)
{
    return this->compute_matrix(nullptr, reverse);
}

vector<int> HirschbergAlignment::finish_row()
{
    // We're finished with a row
    // Swap previous and current row
    std::swap(this->current_row, this->previous_row);

    for(int elem : this->previous_row) {
        std::cout << std::setw(2) << elem << " ";
    }

    std::cout << std::endl;
    
    return vector<int>(this->previous_row);
}

bool HirschbergAlignment::isHalfRow(int row)
{
    int half_row = ((this->seq2.size()-1)/2);
    return row == std::max(0, half_row);
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
    
    std::cout << this->prefix() << "Sequence 1: " << this->seq1 << " length: " << this->seq1.size() << std::endl;
    std::cout << this->prefix() << "Sequence 2: " << this->seq2 << " length: " << this->seq2.size() << std::endl;

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
        this->current_row = {this->cost_function('-', this->seq2[0])};
        this->current_row.reserve(2);

        int score1, score2, score3;
        std::tie(score1, score2, score3) = this->compute_scores(1, this->seq1[0], this->seq2[0]);

        int max_score = std::max({score1, score2, score3});

        if(max_score == score1) {
            alignment1 << this->seq1[0];
            alignment2 << this->seq2[0];
        } else if(max_score == score2) {
            alignment1 << '-';
            alignment2 << this->seq2[0];
        } else if(max_score == score3) {
            alignment1 << this->seq1[0];
            alignment2 << '-';
        }
    } else {
        // Partition the problem
        int max_score;
        auto forward_row = this->compute_matrix(&max_score);
        auto backward_row = this->compute_matrix(true);

        std::cout << this->prefix() << "Max score: " << max_score << std::endl;

        // Find the column where we're going to split
        // (and where the forward and backward matrix meet)
        int k = -1;
        for(Sequence::size_type i = 0; i < this->seq1.size(); ++i) {
            std::cout << this->prefix() << forward_row[i] << " + " << backward_row[this->seq1.size() - i];
            std::cout << " = " << forward_row[i] + backward_row[this->seq1.size() - i];
            std::cout << std::endl;
            if(forward_row[i] + backward_row[this->seq1.size() - i] == max_score) {
                k = i;
            }
        }
                
        if(k < 0) {
            throw AlignException("Could not find matching column");
        }

        // Free some memory because it's not needed anymore.
        forward_row.clear();
        backward_row.clear();
        this->current_row.clear();
        this->previous_row.clear();

        std::cout << this->prefix() << "K: " << k << std::endl;
        Sequence subseq1_left = this->seq1.substr(0, k+1);
        Sequence subseq1_right = this->seq1.substr(k+1);

        std::cout << this->prefix() << "Left: " << subseq1_left << " Right: " 
                  << subseq1_right << std::endl;

        Sequence subseq2_top = this->seq2.substr(0, (this->seq2.size() / 2));
        Sequence subseq2_bottom = this-> seq2.substr((this->seq2.size() / 2));
        std::cout << this->prefix() << "Top: " << subseq2_top << " Bottom: " 
                  << subseq2_bottom << std::endl;

        // Start new hirschberg alignments with the partitioned sequences
        string subalign1;
        string subalign2;
        
        HirschbergAlignment hirsch1(subseq1_left, subseq2_top, this->cost_function,
            this->depth+1);
        std::tie(subalign1, subalign2) = hirsch1.align();
        alignment1 << subalign1;
        alignment2 << subalign2;

        HirschbergAlignment hirsch2(subseq1_right, subseq2_bottom, this->cost_function,
            this->depth+1);
        std::tie(subalign1, subalign2) = hirsch2.align();
        alignment1 << subalign1;
        alignment2 << subalign2;
    }

    return tuple<string, string>(alignment1.str(), alignment2.str());
}

std::string HirschbergAlignment::prefix()
{
    stringstream str;

    for(int i = 0; i < this->depth; ++i) {
        str << "=======> ";
    }

    return str.str();
}
