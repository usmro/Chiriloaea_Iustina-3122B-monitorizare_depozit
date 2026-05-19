#ifndef PRODUS_H
#define PRODUS_H

#include <string>
#include <iostream>

class Produs {

private:
    int id;
    std::string nume;
    int cantitate;
    double pret;
    int pragAlerta;

public:
    Produs();

    Produs(int id,
            const std::string& nume,
            int cantitate,
            double pret,
            int pragAlerta);

    int getId() const;

    std::string getNume() const;

    int getCantitate() const;

    double getPret() const;

    int getPragAlerta() const;

    void setCantitate(int cantitate);

    void afisare() const;

    Produs& operator+=(int valoare);

    Produs& operator-=(int valoare);
};

#endif