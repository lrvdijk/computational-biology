#include <utility>

#include "exceptions.h"
#include "utils.h"
#include "alphabet.h"

using namespace compbio;

Alphabet::Alphabet(const string& letters, const string& complements) :
                   letters(letters), complements(complements) 
{
    if(this->letters.size() != this->complements.size())
    {
        throw AlphabetException("The string length for letters and complements must match!.");
    }

}

Alphabet::Alphabet(const Alphabet& other) : letters(other.letters), complements(other.complements)
{
}

Alphabet::Alphabet(Alphabet&& other) : letters(std::move(other.letters)), 
                                       complements(std::move(other.complements))
{
}

const string& Alphabet::getLetters() const 
{
    return this->letters;
}

const string& Alphabet::getComplements() const
{
    return this->complements;
}

char Alphabet::complement(char letter) const
{
    size_t index = this->letters.find(letter);

    if(index == string::npos)
    {
        throw AlphabetException(string_format("Letter %c not part of the alphabet.",
            letter));
    }

    return this->complements[index];
}    

