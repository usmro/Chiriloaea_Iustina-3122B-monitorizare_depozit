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
    int furnizorId;

public:
    Produs();
    Produs(int id, const std::string& nume, int cantitate,
           double pret, int pragAlerta, int furnizorId = 0);

    int getId()          const { return id; }
    std::string getNume()const { return nume; }
    int getCantitate()   const { return cantitate; }
    double getPret()     const { return pret; }
    int getPragAlerta()  const { return pragAlerta; }
    int getFurnizorId()  const { return furnizorId; }

    void setCantitate(int c)    { cantitate = c; }
    void setFurnizorId(int fid) { furnizorId = fid; }

    Produs& operator+=(int valoare);
    Produs& operator-=(int valoare);

    bool subPrag() const { return cantitate <= pragAlerta; }
};

#endif