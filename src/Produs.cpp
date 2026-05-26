#include "../include/Produs.h"
#include "../include/Exceptions.h"

Produs::Produs()
    : id(0), nume(""), cantitate(0), pret(0.0), pragAlerta(0), furnizorId(0) {}

Produs::Produs(int id, const std::string& nume, int cantitate,
               double pret, int pragAlerta, int furnizorId)
    : id(id), nume(nume), cantitate(cantitate),
      pret(pret), pragAlerta(pragAlerta), furnizorId(furnizorId) {}

Produs& Produs::operator+=(int valoare) {
    if (valoare < 0) throw CantitateInvalidaException();
    cantitate += valoare;
    return *this;
}

Produs& Produs::operator-=(int valoare) {
    if (valoare < 0) throw CantitateInvalidaException();
    if (cantitate < valoare) throw StocInsuficientException(cantitate, valoare);
    cantitate -= valoare;
    return *this;
}