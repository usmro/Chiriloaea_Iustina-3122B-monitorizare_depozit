#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

class ProdusExistentException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Produsul exista deja in depozit!";
    }
};

class ProdusInexistentException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Produsul nu exista in depozit!";
    }
};

class FurnizorExistentException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Furnizorul exista deja!";
    }
};

class FurnizorInexistentException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Furnizorul nu exista!";
    }
};

class StocInsuficientException : public std::exception {
    std::string msg;
public:
    StocInsuficientException(int disponibil, int cerut)
        : msg("Stoc insuficient! Disponibil: " + std::to_string(disponibil)
              + ", Cerut: " + std::to_string(cerut)) {}
    const char* what() const noexcept override { return msg.c_str(); }
};

class CantitateInvalidaException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Cantitatea trebuie sa fie pozitiva!";
    }
};

#endif