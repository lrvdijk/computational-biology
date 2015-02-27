#include <iostream>
#include <iomanip>
#include <algorithm>
#include <iterator>
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

template<typename iter>
tuple<string, string> HirschbergAlignment::compute(iter seq1_l, iter seq1_h, iter seq2_l, iter seq2_h)
{
    stringstream alignment1;
    stringstream alignment2;
    const unsigned dist1 = std::distance(seq1_l, seq1_h);
    const unsigned dist2 = std::distance(seq2_l, seq2_h);

    std::cout << "Distances: " << dist1 << " " << dist2 << std::endl;

    if (dist1 == 0 && dist2 > 0) {
        for(iter it = seq2_l; it != seq2_h; it++) {
            alignment1 << '-';
            alignment2 << *it;
        }
    } else if(dist2 == 0 && dist1 > 0) {
        for(iter it = seq1_l; it != seq1_h; it++) {
            alignment1 << *it;
            alignment2 << '-';
        }
    } else if(dist1 == 1 && dist2 == 1) {
        // We perform standard Needleman-Wunsch calculation
        std::vector<int> prev = this->initialConditions(seq1_l, seq1_h);
        int current = this->cost_function('-', *seq2_l);
        if(*seq1_l == *seq2_l) {
            alignment1 << *seq1_l;
            alignment2 << *seq2_l;
        } else {
            int score1 = prev[1] + this->cost_function(*seq1_l, '-');
            int score2 = current + this->cost_function('-', *seq2_l);
            if(score1 >= score2) {
                alignment1 << '-';
                alignment2 << *seq2_l;
            } else {
                alignment1 << *seq1_l;
                alignment2 << '-';
            }
        }
    } else if(dist1 > 0 && dist2 > 0) {
        // Divide the sequence
        iter seq2_mid = seq2_l + dist2 / 2;

        std::reverse_iterator<iter> seq1_rl(seq1_l), seq1_rh(seq1_h), seq2_rh(seq2_h), seq2_rmid(seq2_mid);

        std::cout << &seq1_l << " " << &seq1_h << " " << &seq2_l << " " << &seq2_mid << std::endl;
        std::cout << &seq1_rh << " " << &seq1_rl << " " << &seq2_rh << " " << &seq2_rmid << std::endl;
        std::cout << "mid: " << &seq2_mid << " " << &seq2_rmid << std::endl;
        std::vector<int> forward_row = this->computeMatrix(seq1_l, seq1_h, seq2_l, seq2_mid);
        std::vector<int> backward_row = this->computeMatrix(seq1_rh, seq1_rl, seq2_rh, seq2_rmid);

        // Find the position where to split seq2
        auto bw_iter = backward_row.rbegin();
        auto seq1_iter = seq1_l, seq1_mid = seq1_l;
        int max = -999999;

        for(auto fw_iter = forward_row.begin(); fw_iter != forward_row.end(); ++fw_iter, ++bw_iter) {
            if(*fw_iter + *bw_iter > max) {
                max = *fw_iter + *bw_iter;
                seq1_mid = seq1_iter;
            }

            // Make sure we not surpass the last element of the sequence
            if(seq1_iter != seq1_h) {
                ++seq1_iter;
            }
        }

        std::cout << "mid2: " << &seq1_mid << std::endl;

        std::string subalign1;
        std::string subalign2;

        std::tie(subalign1, subalign2) = this->compute(seq1_l, seq1_mid, seq2_l, seq2_mid);
        alignment1 << subalign1;
        alignment2 << subalign2;

        std::tie(subalign1, subalign2) = this->compute(seq1_mid, seq1_h, seq2_mid, seq2_h);
        alignment1 << subalign1;
        alignment2 << subalign2;
    }

    return std::make_tuple(alignment1.str(), alignment2.str());
}

template<typename iter>
vector<int> HirschbergAlignment::computeMatrix(iter seq1_l, iter seq1_h, iter seq2_l, iter seq2_h)
{
    vector<int> prev = this->initialConditions(seq1_l, seq1_h);
    vector<int> current(1 + std::distance(seq1_l, seq1_h), 0);

    for(iter s2 = seq2_l; s2 != seq2_h; ++s2) {
        // Provide proper initial conditions
        current[0] = prev[0] + this->cost_function('-', *s2);

        int i = 0;
        for(iter s1 = seq1_l; s1 != seq1_h; ++s1, ++i) {
            if(*s1 == *s2) {
                current[i+1] = prev[i] + this->cost_function(*s1, *s2);
            } else {
                current[i+1] = std::max(
                    prev[i] + this->cost_function(*s1, '-'),
                    current[i] + this->cost_function('-', *s2)
                );
            }
        }

        std::swap(prev, current);
    }

    return prev;
}

template<typename iter>
vector<int> HirschbergAlignment::initialConditions(iter seq1_l, iter seq1_h)
{
    vector<int> initial_row(1+std::distance(seq1_l, seq1_h), 0);
    
    int i = 1;
    // Initial conditions
    for(iter s1 = seq1_l; s1 != seq1_h; ++s1, ++i) {
        initial_row[i] = initial_row[i-1] + this->cost_function(*s1, '-');
    }

    return initial_row;
}

tuple<string, string> HirschbergAlignment::align()
{
    return this->compute(this->seq1.begin(), this->seq1.end(), 
        this->seq2.begin(), this->seq2.end());
}

std::string HirschbergAlignment::prefix()
{
    stringstream str;

    for(int i = 0; i < this->depth; ++i) {
        str << "=======> ";
    }

    return str.str();
}
