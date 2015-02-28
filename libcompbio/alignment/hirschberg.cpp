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

template<typename iter>
string iterToString(iter lo, iter hi)
{
    stringstream out;

    for(iter s = lo; s != hi; s++) {
        out << *s;
    }

    return out.str();
}

std::string prefix(int depth=-1)
{
    static int last_depth = -1;

    if(depth < 0) {
        depth = last_depth > -1 ? last_depth : 0;
    }
    
    stringstream str;

    for(int i = 0; i < depth; ++i) {
        str << "=======> ";
    }

    last_depth = depth;

    return str.str();
}

HirschbergAlignment::HirschbergAlignment(const Sequence& seq1, const Sequence& seq2,
    const cost_function_t& cost_func) : Aligner( 
        seq1.size() < seq2.size() ? seq1 : seq2, 
        seq1.size() < seq2.size() ? seq2 : seq1,
        cost_func
    )
{
}

template<typename iter>
tuple<string, string> HirschbergAlignment::compute(iter xlo, iter xhi, iter ylo, iter yhi, int depth=0)
{
    if(depth > 8) {
        throw AlignException("Too many recursions.");
    }

    stringstream alignment1;
    stringstream alignment2;
    const unsigned distx = std::distance(xlo, xhi);
    const unsigned disty = std::distance(ylo, yhi);

    std::cout << prefix(depth) << "Sequences: " << iterToString(xlo, xhi) << ", " << iterToString(ylo, yhi) << std::endl;
    std::cout << prefix(depth) << "Distances: " << distx << " " << disty << std::endl;

    if (distx == 0 && disty > 0) {
        for(iter it = ylo; it != yhi; it++) {
            alignment1 << '-';
            alignment2 << *it;
        }
    } else if(disty == 0 && distx > 0) {
        for(iter it = xlo; it != xhi; it++) {
            alignment1 << *it;
            alignment2 << '-';
        }
    } else if(distx == 1 && disty == 1) {
        // We perform standard Needleman-Wunsch calculation
        std::vector<int> prev = this->initialConditions(xlo, xhi);
        int current = this->cost_function(*xlo, '-');
        int score1 = prev[0] + this->cost_function(*xlo, *ylo);
        int score2 = prev[1] + this->cost_function('-', *xlo);
        int score3 = current + this->cost_function(*ylo, '-');
        
        int max_score = std::max({score1, score2, score3});
        if(max_score == score1) {
            alignment1 << *xlo;
            alignment2 << *ylo;
        } else if(max_score == score2) {
            alignment1 << '-';
            alignment2 << *ylo;
        } else {
            alignment1 << *xlo;
            alignment2 << '-';
        }
    } else if(distx > 0 && disty > 0) {
        // Divide the sequence
        iter ymid = ylo + disty / 2;

        std::reverse_iterator<iter> lox(xlo), hix(xhi), hiy(yhi), midy(ymid);

        std::cout << prefix(depth) << "Forward matrix:" << std::endl;
        std::vector<int> forward_row = this->computeMatrix(xlo, xhi, ylo, ymid);
        std::cout << prefix(depth) << "Backward matrix:" << std::endl;
        std::vector<int> backward_row = this->computeMatrix(hix, lox, hiy, midy);

        // Find the position where to split seq1
        auto bw_iter = backward_row.rbegin();
        iter x = xlo, xmid = xlo;
        int max = -999999;

        for(auto fw_iter = forward_row.begin(); fw_iter != forward_row.end(); ++fw_iter, ++bw_iter) {
            std::cout << prefix(depth) << *fw_iter << "+" << *bw_iter << "=" << *fw_iter + *bw_iter << " max:" << max << std::endl;
            if(*fw_iter + *bw_iter > max) {
                max = *fw_iter + *bw_iter;
                xmid = x;
            }

            // Make sure we not surpass the last element of the sequence
            if(x != xhi) {
                ++x;
            }
        }

        std::string subalign1;
        std::string subalign2;

        std::tie(subalign1, subalign2) = this->compute(xlo, xmid, ylo, ymid, depth+1);
        alignment1 << subalign1;
        alignment2 << subalign2;

        std::tie(subalign1, subalign2) = this->compute(xmid, xhi, ymid, yhi, depth+1);
        alignment1 << subalign1;
        alignment2 << subalign2;
    }

    if(alignment1.str().size() > 0 || alignment2.str().size() > 0)
    {
        std::cout << prefix(depth) << "Alignment1: " << string(alignment1.str()) << std::endl;
        std::cout << prefix(depth) << "Alignment2: " << string(alignment2.str()) << std::endl;
    }

    return std::make_tuple(alignment1.str(), alignment2.str());
}

template<typename iter>
vector<int> HirschbergAlignment::computeMatrix(iter xlo, iter xhi, iter ylo, iter yhi)
{
    vector<int> prev = this->initialConditions(xlo, xhi);
    vector<int> current(1 + std::distance(xlo, xhi), 0);

    // DEBUG
    std::cout << prefix() << std::setw(2) << "   -  ";
    for(iter s1 = xlo; s1 != xhi; s1++) {
        std::cout << *s1 << "  ";
    }
    std::cout << std::endl;

    std::cout << prefix() << "- ";
    for(auto elem : prev) {
        std::cout << std::setw(2) << elem << " ";
    }
    std::cout << std::endl;

    for(iter y = ylo; y != yhi; ++y) {
        // Provide proper initial conditions
        current[0] = prev[0] + this->cost_function('-', *y);

        int i = 0;
        for(iter x = xlo; x != xhi; ++x, ++i) {
            int score1 = prev[i] + this->cost_function(*xlo, *ylo);
            int score2 = prev[i+1] + this->cost_function('-', *ylo);
            int score3 = current[i] + this->cost_function(*xlo, '-');
            
            current[i+1] = std::max({score1, score2, score3});
        }

        std::cout << prefix() << *y << " ";
        for(int elem : current) {
            std::cout << std::setw(2) << elem << " ";
        }
        std::cout << std::endl;

        std::swap(prev, current);
    }

    return prev;
}

template<typename iter>
vector<int> HirschbergAlignment::initialConditions(iter xlo, iter xhi)
{
    vector<int> initial_row(1+std::distance(xlo, xhi), 0);
    
    int i = 1;
    // Initial conditions
    for(iter s1 = xlo; s1 != xhi; ++s1, ++i) {
        initial_row[i] = initial_row[i-1] + this->cost_function(*s1, '-');
    }

    return initial_row;
}

tuple<string, string> HirschbergAlignment::align()
{
    return this->compute(this->seq1.begin(), this->seq1.end(), 
        this->seq2.begin(), this->seq2.end());
}

