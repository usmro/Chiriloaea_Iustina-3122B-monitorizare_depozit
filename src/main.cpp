#include "../include/Depozit.h"
#include "../include/Exceptions.h"

#include <iostream>

int main() {

    Depozit depozit;

    int optiune;

    do {

        std::cout << "\n===== MENIU DEPOZIT =====\n";

        std::cout << "1. Adauga produs\n";

        std::cout << "2. Afiseaza produse\n";

        std::cout << "3. Actualizeaza cantitate\n";

        std::cout << "4. Raport produse sub prag\n";

        std::cout << "0. Iesire\n";

        std::cout << "\nAlege optiunea: ";

        std::cin >> optiune;

        try {

            if (optiune == 1) {

                int id;
                std::string nume;
                int cantitate;
                double pret;
                int pragAlerta;

                std::cout << "\nID produs: ";
                std::cin >> id;

                std::cout << "Nume produs: ";
                std::cin >> nume;

                std::cout << "Cantitate: ";
                std::cin >> cantitate;

                std::cout << "Pret: ";
                std::cin >> pret;

                std::cout << "Prag alerta: ";
                std::cin >> pragAlerta;

                Produs produs(id, nume, cantitate, pret, pragAlerta);

                depozit.adaugaProdus(produs);

                std::cout << "\nProdus adaugat cu succes!\n";
            }

            else if (optiune == 2) {

                depozit.afiseazaProduse();
            }

            else if (optiune == 3) {

                int id;
                int valoare;

                std::cout << "\nID produs: ";

                std::cin >> id;

                std::cout << "Valoare modificare (+/-): ";

                std::cin >> valoare;

                depozit.actualizeazaCantitate(id, valoare);

                std::cout << "\nCantitate actualizata!\n";
            }

            else if (optiune == 4) {

                depozit.genereazaRaportStocMic();
            }

            else if (optiune == 0) {

                std::cout << "\nAplicatia se inchide...\n";
            }

            else {

                std::cout << "\nOptiune invalida!\n";
            }
        }

        catch (const std::exception& e) {

            std::cout << "\nEroare: " << e.what() << std::endl;
        }

    } while (optiune != 0);

    return 0;
}