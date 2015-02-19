#ifndef COMPBIO_ALPHABET_H
#define COMPBIO_ALPHABET_H

#include <string>
#include <memory>
#include <vector>
#include <functional>

namespace compbio {

using std::string;
using std::unique_ptr;
using std::vector;
using std::function;

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
        function<int(const Letter&, const Letter&)> cost_function;

    public:
        Alphabet(const string& letters, const string& complement);
        Alphabet(const char* letters, const char* complement);
};

}

#endif
        
