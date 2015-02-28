#ifndef COMPBIO_SEQUENCE_H
#define COMPBIO_SEQUENCE_H

#include <utility>
#include <random>
#include <algorithm>
#include <string>

#include "utils.h"
#include "alphabet.h" 

namespace compbio {

class Sequence : public std::string {
    private:
        const Alphabet& alphabet;

    protected:
        void validate_sequence();

    public:
        // Constructors for which use the default alphabet
        Sequence();
        Sequence(const std::string& seq);
        Sequence(size_t length, char default_value);

        // Constructors with custom alphabet
        Sequence(const std::string& seq, const Alphabet& alphabet);
        Sequence(size_t length, char default_value, const Alphabet& alphabet);

        // Copying and moving
        Sequence(const Sequence& other);
        Sequence(Sequence&& other);

        // Generate a random sequence from a given alphabet
        template<Alphabet& alphabet>
        static Sequence random(size_t length);

        // Retrieve sequence complements
        Sequence complement();
        Sequence reverse_complement();

        // Destructor
        virtual ~Sequence();
};

template<Alphabet& chars>
Sequence Sequence::random(size_t length)
{
    auto letters = chars.getLetters();
    static std::uniform_int_distribution<> dist(0, letters.size()-1);
    
    Sequence new_sequence(length, 0, chars);
    std::generate_n(new_sequence.begin(), length, [&letters] () -> char {
        auto engine = prng_engine();
        return letters[dist(engine)];
    });

    return new_sequence;
}


}

#endif
