#include <utility>
#include <random>
#include <algorithm>

#include "utils.h"
#include "sequence.h"
#include "alphabets/dna.h"

using namespace compbio;
using std::string;

Sequence::Sequence() : string(), alphabet(DNAAlphabet)
{
}

Sequence::Sequence(const string& seq) : string(seq), alphabet(DNAAlphabet)
{
    this->validate_sequence();
}

Sequence::Sequence(size_t length, char default_value) : string(length, default_value),
                   alphabet(DNAAlphabet)
{
    this->validate_sequence();
}

Sequence::Sequence(const string& seq, const Alphabet& alphabet) : string(seq),
                   alphabet(alphabet)
{
    this->validate_sequence();
}

Sequence::Sequence(size_t length, char default_value, const Alphabet& alphabet) :
                   string(length, default_value), alphabet(alphabet)
{
    this->validate_sequence();
}

Sequence::Sequence(const Sequence& other) : string(other),
                   alphabet(other.alphabet)
{
    this->validate_sequence();
}

Sequence::Sequence(Sequence&& other) : string(std::move(other)),
                   alphabet(std::move(other.alphabet))
{
    this->validate_sequence();
}

void Sequence::validate_sequence() 
{
    // TODO
}

Sequence Sequence::random(size_t length, const Alphabet& alphabet)
{
    std::uniform_int_distribution<> dist(0, alphabet.getLetters().size()-1);
    
    Sequence new_sequence(length, 0, alphabet);
    std::generate_n(new_sequence.begin(), length, [&alphabet,&dist] () {
        auto engine = prng_engine();
        return alphabet.getLetters()[dist(engine)];
    });

    return new_sequence;
}
    
Sequence Sequence::complement()
{
    // TODO
    
    return *this;
}

Sequence Sequence::reverse_complement()
{
    // TODO
    
    return *this;
}
