#include "../include/Depozit.h"
#include "../include/Exceptions.h"
#include "../include/UI.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <fstream>
#include <sstream>

void Depozit::adaugaProdus(const Produs& produs) {
    if (produse.find(produs.getId()) != produse.end())
        throw ProdusExistentException();
    produse[produs.getId()] = produs;
}

void Depozit::eliminaProdus(int id) {
    if (produse.find(id) == produse.end())
        throw ProdusInexistentException();
    produse.erase(id);
}

const Produs& Depozit::getProdus(int id) const {
    auto it = produse.find(id);
    if (it == produse.end()) throw ProdusInexistentException();
    return it->second;
}

void Depozit::actualizeazaCantitate(int id, int valoare) {
    if (produse.find(id) == produse.end())
        throw ProdusInexistentException();
    if (valoare > 0) produse[id] += valoare;
    else             produse[id] -= (-valoare);
}

void Depozit::aplicaTranzactie(const TranzactieIntrare& t) {
    if (produse.find(t.getProdusId()) == produse.end())
        throw ProdusInexistentException();
    produse[t.getProdusId()] += t.getCantitate();
    istoricTranzactii.push_back({
        t.getTip(), t.getProdusId(), t.getCantitate(),
        t.getTimestamp(), t.getObservatii()
    });
}

void Depozit::aplicaTranzactie(const TranzactieIesire& t) {
    if (produse.find(t.getProdusId()) == produse.end())
        throw ProdusInexistentException();
    produse[t.getProdusId()] -= t.getCantitate();
    istoricTranzactii.push_back({
        t.getTip(), t.getProdusId(), t.getCantitate(),
        t.getTimestamp(), t.getObservatii()
    });
}

void Depozit::adaugaFurnizor(const Furnizor& furnizor) {
    if (furnizori.find(furnizor.getId()) != furnizori.end())
        throw FurnizorExistentException();
    furnizori[furnizor.getId()] = furnizor;
}

void Depozit::asociazaFurnizor(int produsId, int furnizorId) {
    if (produse.find(produsId) == produse.end())
        throw ProdusInexistentException();
    if (furnizori.find(furnizorId) == furnizori.end())
        throw FurnizorInexistentException();
    produse[produsId].setFurnizorId(furnizorId);
}

void Depozit::afiseazaProduse() const {

    UI::printSectionHeader("STOCURI DEPOZIT", "📦");

    if (produse.empty()) {

        UI::printWarning("Depozitul este gol.");

        return;
    }

    std::cout << "\n  "
              << Color::BOLD
              << Color::BCYAN
              << std::left
              << std::setw(6)  << "ID"
              << std::setw(32) << "PRODUS"
              << std::setw(12) << "CANT."
              << std::setw(15) << "PRET"
              << std::setw(8)  << "PRAG"
              << std::setw(24) << "BARA STOC"
              << "FURNIZOR"
              << Color::RESET
              << "\n";

    std::cout << "  " << Color::DIM;

    for (int i = 0; i < 120; i++)
        std::cout << "─";

    std::cout << Color::RESET << "\n";

    std::vector<const Produs*> sorted;

    for (const auto& p : produse)
        sorted.push_back(&p.second);

    std::sort(
        sorted.begin(),
        sorted.end(),
        [](const Produs* a, const Produs* b) {

            return a->getId() < b->getId();
        }
    );

    for (const Produs* p : sorted) {

        bool alert = p->subPrag();

        std::string rowColor =
            alert ? Color::BRED : Color::BWHITE;

        std::string furnNume = "-";

        if (p->getFurnizorId() != 0) {

            auto it = furnizori.find(
                p->getFurnizorId()
            );

            if (it != furnizori.end())
                furnNume = it->second.getNume();
        }

        std::string bar =
            UI::stockBar(
                p->getCantitate(),
                p->getPragAlerta()
            );

        std::string alertIcon =
            alert ? " 🚨" : "";

        std::cout << "  "
                  << rowColor
                  << std::left
                  << std::setw(6)  << p->getId()
                  << std::setw(32) << p->getNume().substr(0, 30)
                  << std::setw(12)
                  << (std::to_string(p->getCantitate()) + alertIcon)
                  << std::setw(15)
                  << UI::formatPret(p->getPret())
                  << std::setw(8)
                  << p->getPragAlerta()
                  << Color::RESET;

        std::cout
            << bar
            << "  "
            << Color::DIM
            << furnNume
            << Color::RESET
            << "\n";
    }

    std::cout << "  " << Color::DIM;

    for (int i = 0; i < 120; i++)
        std::cout << "─";

    std::cout << Color::RESET << "\n";

    std::cout
        << Color::DIM
        << "  🚨 = sub pragul de alerta\n"
        << Color::RESET;
}

void Depozit::genereazaRaportStocMic() const {
    UI::printSectionHeader("RAPORT ALERTE STOC SCAZUT", "🚨");

    auto lista = produseDeReCommandat();

    if (lista.empty()) {
        UI::printSuccess("Toate produsele sunt peste pragul de alerta!");
        return;
    }

    std::cout << "\n  " << Color::BOLD << Color::BRED
              << lista.size() << " produs(e) necesita recomandare:\n\n"
              << Color::RESET;

    int rank = 1;
    for (const Produs& p : lista) {
        std::string furnNume = "-";
        auto it = furnizori.find(p.getFurnizorId());
        if (it != furnizori.end()) furnNume = it->second.getNume();

        std::cout << "  " << Color::BYELLOW << "  #" << rank++ << "  " << Color::RESET;
        std::cout << Color::BOLD << std::setw(18) << std::left << p.getNume() << Color::RESET;
        std::cout << "  Cant: " << Color::BRED    << std::setw(5) << p.getCantitate() << Color::RESET;
        std::cout << "  Prag: " << Color::BYELLOW << std::setw(5) << p.getPragAlerta() << Color::RESET;
        std::cout << "  " << UI::stockBar(p.getCantitate(), p.getPragAlerta(), 15);
        std::cout << "  Furnizor: " << Color::BCYAN << furnNume << Color::RESET << "\n";
    }
}

std::vector<Produs> Depozit::produseDeReCommandat() const {
    std::vector<Produs> result;
    for (const auto& pereche : produse)
        if (pereche.second.subPrag())
            result.push_back(pereche.second);
    std::sort(result.begin(), result.end(),
        [](const Produs& a, const Produs& b){
            return a.getCantitate() < b.getCantitate();
        });
    return result;
}
void Depozit::salveazaInFisier(const std::string& numeFisier) const {

    std::ofstream fout(numeFisier);

    for (const auto& pereche : produse) {

        const Produs& p = pereche.second;

        fout << p.getId() << ","
             << p.getNume() << ","
             << p.getCantitate() << ","
             << p.getPret() << ","
             << p.getPragAlerta()
             << "\n";
    }
}

void Depozit::incarcaDinFisier(const std::string& numeFisier) {

    std::ifstream fin(numeFisier);

    if (!fin.is_open()) {

        return;
    }

    produse.clear();

    std::string linie;

    while (getline(fin, linie)) {

        std::stringstream ss(linie);

        std::string camp;

        int id;
        std::string nume;
        int cantitate;
        double pret;
        int prag;

        getline(ss, camp, ',');
        id = std::stoi(camp);

        getline(ss, nume, ',');

        getline(ss, camp, ',');
        cantitate = std::stoi(camp);

        getline(ss, camp, ',');
        pret = std::stod(camp);

        getline(ss, camp, ',');
        prag = std::stoi(camp);

        produse[id] = Produs(id, nume, cantitate, pret, prag);
    }
}
double Depozit::valoareTotalaStoc() const {

    double total = 0;

    for (const auto& pereche : produse) {

        const Produs& p = pereche.second;

        total += p.getPret() * p.getCantitate();
    }

    return total;
}

int Depozit::numarProduseSubPrag() const {

    int count = 0;

    for (const auto& pereche : produse) {

        if (pereche.second.subPrag()) {

            count++;
        }
    }

    return count;
}

Produs Depozit::produsCelMaiScump() const {

    auto it = produse.begin();

    Produs maxim = it->second;

    for (const auto& pereche : produse) {

        if (pereche.second.getPret() > maxim.getPret()) {

            maxim = pereche.second;
        }
    }

    return maxim;
}
void Depozit::genereazaComenziReaprovizionare() const {

    UI::printSectionHeader(
        "GENERARE COMENZI REAPROVIZIONARE",
        "📄"
    );

    bool existaProduse = false;

    for (const auto& furnizorPair : furnizori) {

        int furnizorId = furnizorPair.first;

        const Furnizor& furnizor =
            furnizorPair.second;

        std::ofstream fout(
            "comanda_furnizor_"
            + std::to_string(furnizorId)
            + ".txt"
        );

        bool areProduse = false;

        fout << "=====================================\n";

        fout << "   COMANDA REAPROVIZIONARE STOC\n";

        fout << "=====================================\n\n";

        fout << "Furnizor: "
             << furnizor.getNume()
             << "\n";

        fout << "Telefon: "
             << furnizor.getTelefon()
             << "\n";

        fout << "Email: "
             << furnizor.getEmail()
             << "\n\n";

        fout << "Produse necesare:\n\n";

        for (const auto& produsPair : produse) {

            const Produs& p =
                produsPair.second;

            if (p.getFurnizorId() == furnizorId
                && p.subPrag()) {

                areProduse = true;

                existaProduse = true;

                int necesar =
                    p.getPragAlerta() * 2
                    - p.getCantitate();

                fout
                    << "- "
                    << p.getNume()
                    << " -> "
                    << necesar
                    << " bucati\n";
            }
        }

        if (!areProduse) {

            fout.close();

            std::remove(
                (
                    "comanda_furnizor_"
                    + std::to_string(furnizorId)
                    + ".txt"
                ).c_str()
            );
        }
        else {

            fout << "\n=====================================\n";

            fout << "Comanda generata automat de sistem.\n";

            fout.close();

            UI::printSuccess(
                "Generata: comanda_furnizor_"
                + std::to_string(furnizorId)
                + ".txt"
            );
        }
    }

    if (!existaProduse) {

        UI::printWarning(
            "Nu exista produse sub prag."
        );
    }
}
void Depozit::salveazaFurnizori(
    const std::string& fisier
) const {

    std::ofstream fout(fisier);

    for (const auto& pair : furnizori) {

        const Furnizor& f = pair.second;

        fout
            << f.getId() << ","
            << f.getNume() << ","
            << f.getTelefon() << ","
            << f.getEmail()
            << "\n";
    }
}

void Depozit::incarcaFurnizori(
    const std::string& fisier
) {

    std::ifstream fin(fisier);

    if (!fin.is_open()) {

        return;
    }

    furnizori.clear();

    std::string linie;

    while (getline(fin, linie)) {

        std::stringstream ss(linie);

        std::string camp;

        int id;

        std::string nume;
        std::string telefon;
        std::string email;

        getline(ss, camp, ',');
        id = std::stoi(camp);

        getline(ss, nume, ',');

        getline(ss, telefon, ',');

        getline(ss, email, ',');

        furnizori[id] =
            Furnizor(
                id,
                nume,
                telefon,
                email
            );
    }
}