#ifndef TRANZACTIE_H
#define TRANZACTIE_H

#include <string>
#include <stdexcept>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

struct Intrare {
    static std::string tip() { return "INTRARE"; }
    static int semn()        { return +1; }
};

struct Iesire {
    static std::string tip() { return "IESIRE"; }
    static int semn()        { return -1; }
};

template<typename TipTranzactie>
class Tranzactie {
private:
    int produsId;
    int cantitate;
    std::string timestamp;
    std::string observatii;

    static std::string now() {
        auto t = std::chrono::system_clock::now();
        std::time_t tt = std::chrono::system_clock::to_time_t(t);
        std::tm tm_s{};
#ifdef _WIN32
        localtime_s(&tm_s, &tt);
#else
        localtime_r(&tt, &tm_s);
#endif
        std::ostringstream oss;
        oss << std::put_time(&tm_s, "%Y-%m-%d %H:%M:%S");
        return oss.str();
    }

public:
    Tranzactie(int produsId, int cantitate, const std::string& obs = "")
        : produsId(produsId), cantitate(cantitate),
          timestamp(now()), observatii(obs)
    {
        if (cantitate <= 0)
            throw std::invalid_argument("Cantitatea tranzactiei trebuie sa fie pozitiva!");
    }

    int getProdusId()          const { return produsId; }
    int getCantitate()         const { return cantitate; }
    std::string getTip()       const { return TipTranzactie::tip(); }
    std::string getTimestamp() const { return timestamp; }
    std::string getObservatii()const { return observatii; }
    int getDelta()             const { return TipTranzactie::semn() * cantitate; }
};

using TranzactieIntrare = Tranzactie<Intrare>;
using TranzactieIesire  = Tranzactie<Iesire>;

#endif