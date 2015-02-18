#ifndef COMPBIO_SEQUENCE_H
#define COMPBIO_SEQUENCE_H

#include <string>
#include "alphabet.h" 

namespace compbio {

class Sequence : std::string {
    private:
        const Alphabet& alphabet;

    public:
        // Constructors for which use the default alphabet
        Sequence() : std::string() { }
        Sequence(const char* seq);
        Sequence(const std::string& seq);

        // Constructors with custom alphabet
        Sequence(const char* seq, const Alphabet& alphabet);
        Sequence(const std::string& seq, const Alphabet& alphabet);

        // Copying and moving
        Sequence(const Sequence& other);
        Sequence(Sequence&& other);

        Sequence complement();
        Sequence reverse_complement();

        // Destructor
        virtual ~Sequence();
};

}

#endif
