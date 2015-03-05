#include <tuple>
#include <string>
#include <iostream>
#include <libcompbio/alignment/needleman-wunsch.h>

using namespace compbio;

int main()
{
    Sequence seq1("AGCACACA");
    Sequence seq2("ACACACTA");
    //Sequence seq1("AGTACGCA");
    //Sequence seq2("TATGC");

    NeedlemanWunsch aligner(seq1, seq2, [] (char a, char b) {
        if(a == b) {
            return 2;
        } else if(a == '-') {
            return -1;
        } else if(b == '-') {
            return -1;
        } else {
            return 0;
        }
    });

    std::string alignment1;
    std::string alignment2;
    std::tie(alignment1, alignment2) = aligner.align();

    std::cout << alignment1 << std::endl;
    std::cout << alignment2 << std::endl;
    
    return 0;
}



