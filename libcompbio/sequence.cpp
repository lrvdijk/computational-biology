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

Sequence::~Sequence()
{
}
