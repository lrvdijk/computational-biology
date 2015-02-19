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
        function<int(char a, char b)> cost_function;

    public:
        // Standard constructors
        Alphabet(const string& letters, const string& complements);
        Alphabet(const string& letters, const string& complements,
            function<int(char a, char b)>& cost_function);

        // Copying and moving
        Alphabet(const Alphabet& other);
        Alphabet(Alphabet&& other);

        virtual const std::string& getLetters() const;
        virtual const std::string& getComplements() const;
        virtual char complement(char letter) const;
        void setCostFunction(function<int(char a, char b)>& cost_function);

        virtual ~Alphabet() { }
};

}

#endif
        
