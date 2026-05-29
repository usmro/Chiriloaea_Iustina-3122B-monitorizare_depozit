#ifndef DEPOZIT_H
#define DEPOZIT_H

#include <unordered_map>
#include <vector>
#include <string>
#include "Produs.h"
#include "Furnizor.h"
#include "Tranzactie.h"

class Depozit {
private:
    std::unordered_map<int, Produs>   produse;
    std::unordered_map<int, Furnizor> furnizori;

    struct LogEntry {
        std::string tip;
        int produsId;
        int cantitate;
        std::string timestamp;
        std::string observatii;
    };
    std::vector<LogEntry> istoricTranzactii;

public:
    void adaugaProdus(const Produs& produs);
    void eliminaProdus(int id);
    void salveazaInFisier(const std::string& numeFisier) const;

    void incarcaDinFisier(const std::string& numeFisier);
    void actualizeazaCantitate(int id, int valoare);
    const Produs& getProdus(int id) const;
    const std::unordered_map<int, Produs>& getProduse() const { return produse; }

    void aplicaTranzactie(const TranzactieIntrare& t);
    void aplicaTranzactie(const TranzactieIesire&  t);

    void adaugaFurnizor(const Furnizor& furnizor);
    void asociazaFurnizor(int produsId, int furnizorId);
    const std::unordered_map<int, Furnizor>& getFurnizori() const { return furnizori; }

    void afiseazaProduse()        const;
    void genereazaRaportStocMic() const;
    std::vector<Produs> produseDeReCommandat() const;
    double valoareTotalaStoc() const;

    int numarProduseSubPrag() const;
    void genereazaComenziReaprovizionare() const;
    void salveazaFurnizori(const std::string& fisier) const;

    void incarcaFurnizori(const std::string& fisier);
    Produs produsCelMaiScump() const;
    const std::vector<LogEntry>& getIstoric() const { return istoricTranzactii; }
    int numarProduse() const { return (int)produse.size(); }
    void salveazaTranzactie(
    const std::string& linie
        ) const;
        void afiseazaIstoricFisier() const;
};

#endif