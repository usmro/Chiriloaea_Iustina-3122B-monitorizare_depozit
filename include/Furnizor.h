#ifndef FURNIZOR_H
#define FURNIZOR_H

#include <string>

class Furnizor {
private:
    int id;
    std::string nume;
    std::string telefon;
    std::string email;

public:
    Furnizor() : id(0), nume(""), telefon(""), email("") {}
    Furnizor(int id, const std::string& nume,
             const std::string& telefon = "",
             const std::string& email = "")
        : id(id), nume(nume), telefon(telefon), email(email) {}

    int getId()              const { return id; }
    std::string getNume()    const { return nume; }
    std::string getTelefon() const { return telefon; }
    std::string getEmail()   const { return email; }
};

#endif