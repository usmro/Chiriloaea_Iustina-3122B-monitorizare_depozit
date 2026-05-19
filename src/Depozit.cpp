#include "../include/Depozit.h"
#include "../include/Exceptions.h"
#include <iostream>

void Depozit::adaugaProdus(const Produs& produs) {

    int id = produs.getId();

    if (produse.find(id) != produse.end()) {

        throw ProdusExistentException();
    }

    produse[id] = produs;
}

void Depozit::eliminaProdus(int id) {

    if (produse.find(id) == produse.end()) {

        throw ProdusInexistentException();
    }

    produse.erase(id);
}

void Depozit::actualizeazaCantitate(int id, int valoare) {

    if (produse.find(id) == produse.end()) {

        throw ProdusInexistentException();
    }

    if (valoare > 0) {

        produse[id] += valoare;
    }
    else {

        produse[id] -= (-valoare);
    }
}

void Depozit::afiseazaProduse() const {

    for (const auto& pereche : produse) {

        pereche.second.afisare();

        std::cout << "------------------\n";
    }
}

void Depozit::genereazaRaportStocMic() const {

    std::cout << "\nProduse sub pragul de alerta:\n\n";

    for (const auto& pereche : produse) {

        const Produs& produs = pereche.second;

        if (produs.getCantitate() <= produs.getPragAlerta()) {

            produs.afisare();

            std::cout << "------------------\n";
        }
    }
}