#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

class ProdusExistentException : public std::exception {

public:
    const char* what() const noexcept override {

        return "Produsul exista deja in depozit!";
    }
};

class ProdusInexistentException : public std::exception {

public:
    const char* what() const noexcept override {

        return "Produsul nu exista!";
    }
};

#endif