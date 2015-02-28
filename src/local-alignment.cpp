#include <tuple>
#include <string>
#include <iostream>
#include <fstream>

#include <libcompbio/alphabets/dna.h>
#include <libcompbio/alignment/smith-waterman.h>

using namespace compbio;

int main()
{
    std::fstream input;
    input.open("sequences.txt");

    if(!input.is_open()) {
        std::cerr << "Could not open the file sequences.txt" << std::endl;
        return 1;
    }

    // Read two lines as sequences
    Sequence seq1;
    Sequence seq2;

    bool success = getline(input, seq1);
    success = getline(input, seq2);

    if(!success) {
        std::cerr << "The file sequences.txt should at least contain two lines." << std::endl;
        return 1;
    }

    input.close();

    SmithWaterman aligner(seq1, seq2, [] (char a, char b) {
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



