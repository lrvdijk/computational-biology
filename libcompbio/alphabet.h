#ifndef COMPBIO_ALPHABET_H
#define COMPBIO_ALPHABET_H

#include <string>
#include <functional>


namespace compbio {

using std::string;
using std::function;

class Alphabet {
    private:
        string letters;
        string complements;

    public:
        // Standard constructors
        Alphabet(const string& letters, const string& complements);

        // Copying and moving
        Alphabet(const Alphabet& other);
        Alphabet(Alphabet&& other);

        virtual const std::string& getLetters() const;
        virtual const std::string& getComplements() const;
        virtual char complement(char letter) const;

        virtual ~Alphabet() { }
};

}

#endif
        
