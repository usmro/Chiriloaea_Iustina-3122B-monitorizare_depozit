#ifndef DEPOZIT_H
#define DEPOZIT_H

#include <unordered_map>
#include "Produs.h"

class Depozit {

private:
    std::unordered_map<int, Produs> produse;

public:

    void adaugaProdus(const Produs& produs);

    void eliminaProdus(int id);

    void actualizeazaCantitate(int id, int valoare);

    void afiseazaProduse() const;

    void genereazaRaportStocMic() const;
};

#endif