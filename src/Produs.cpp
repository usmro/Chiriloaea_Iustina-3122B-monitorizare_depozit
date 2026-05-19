#include "../include/Produs.h"

Produs::Produs()
    : id(0), nume(""), cantitate(0), pret(0.0), pragAlerta(0) {}

Produs::Produs(int id,
               const std::string& nume,
               int cantitate,
               double pret,
               int pragAlerta)
    : id(id),
      nume(nume),
      cantitate(cantitate),
      pret(pret),
      pragAlerta(pragAlerta) {}

int Produs::getId() const {
    return id;
}

std::string Produs::getNume() const {
    return nume;
}

int Produs::getCantitate() const {
    return cantitate;
}

double Produs::getPret() const {
    return pret;
}

int Produs::getPragAlerta() const {
    return pragAlerta;
}

void Produs::setCantitate(int cantitate) {
    this->cantitate = cantitate;
}

void Produs::afisare() const {

    std::cout << "ID: " << id << std::endl;

    std::cout << "Nume: " << nume << std::endl;

    std::cout << "Cantitate: " << cantitate << std::endl;

    std::cout << "Pret: " << pret << std::endl;

    std::cout << "Prag alerta: " << pragAlerta << std::endl;
}

Produs& Produs::operator+=(int valoare) {

    cantitate += valoare;

    return *this;
}

Produs& Produs::operator-=(int valoare) {

    cantitate -= valoare;

    return *this;
}