#ifndef COMPBIO_ALPHABET_H
#define COMPBIO_ALPHABET_H

#include <string>
#include <functional>

namespace compbio {

using std::string;
using std::function;

class Alphabet {
    private:
        std::string letters;
        std::string complements;
        function<int(char a, char b)> cost_function;

    public:
        Alphabet(const string& letters, const string& complements);

        Alphabet(const string& letters, const string& complements,
            function<int(char a, char b)>& cost_function);

        // Copying and moving
        Alphabet(const Alphabet& other);
        Alphabet(Alphabet&& other);

        char complement(char letter);

        void setCostFunction(function<int(char a, char b)>& cost_function);
};

}

#endif
        
