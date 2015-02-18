#ifndef COMPBIO_ALPHABET_H
#define COMPBIO_ALPHABET_H

#include <memory>
#include <vector>

namespace compbio {

using std::unique_ptr;
using std::vector;

class Letter {
    private:
        char letter;
        char complement;

    public:
        Letter(char letter); // can be used implicitly
        Letter(char letter, char complement);

        char getLetter() const;
        void setLetter(char letter);

        char getComplement() const;
        void setComplement(char complement);

        virtual ~Letter() { }
};

class Alphabet {
    private:
        vector<unique_ptr<Letter> > letters;

    public:
};

}

#endif
        
