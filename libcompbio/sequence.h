#ifndef COMPBIO_SEQUENCE_H
#define COMPBIO_SEQUENCE_H

#include <string>
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
        static Sequence random(size_t length, const Alphabet& alphabet);

        // Retrieve sequence complements
        Sequence complement();
        Sequence reverse_complement();

        // Destructor
        virtual ~Sequence();
};

}

#endif
