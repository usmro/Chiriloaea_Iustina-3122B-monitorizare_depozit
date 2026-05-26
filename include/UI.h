#ifndef UI_H
#define UI_H

#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace Color {
    const std::string RESET    = "\033[0m";
    const std::string BOLD     = "\033[1m";
    const std::string DIM      = "\033[2m";
    const std::string RED      = "\033[31m";
    const std::string GREEN    = "\033[32m";
    const std::string YELLOW   = "\033[33m";
    const std::string BLUE     = "\033[34m";
    const std::string CYAN     = "\033[36m";
    const std::string BRED     = "\033[91m";
    const std::string BGREEN   = "\033[92m";
    const std::string BYELLOW  = "\033[93m";
    const std::string BBLUE    = "\033[94m";
    const std::string BCYAN    = "\033[96m";
    const std::string BWHITE   = "\033[97m";
    const std::string BG_DARK  = "\033[48;5;235m";
    const std::string BG_RED   = "\033[41m";
    const std::string BG_GREEN = "\033[42m";
}

namespace UI {

inline void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

inline std::string repeat(const std::string& s, int n) {
    std::string r;
    for (int i = 0; i < n; i++) r += s;
    return r;
}

inline void printBanner() {
    clearScreen();
    std::cout << Color::BCYAN << Color::BOLD;
    std::cout << "╔══════════════════════════════════════════════════════╗\n";
    std::cout << "║   📦  SISTEM DE MONITORIZARE A STOCURILOR            ║\n";
    std::cout << "║       Gestiune Depozit                               ║\n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n";
    std::cout << Color::RESET << "\n";
}
inline std::string formatPret(double pret) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << pret << " RON";
    return oss.str();
}
inline void printDashboard(
    int produse,
    int produseSubPrag,
    int furnizori,
    double valoare,
    const std::string& scump
) {

    std::string numeScurt = scump.substr(0, 20);

    std::cout << Color::BG_DARK << Color::BWHITE;

    std::cout << " +------------------------------------------------+\n";
    std::cout << " |                DASHBOARD STOC                  |\n";
    std::cout << " +------------------------------------------------+\n";

    std::cout << " | Produse totale      : "
              << std::setw(24) << produse << " |\n";

    std::cout << " | Produse sub prag    : "
              << std::setw(24) << produseSubPrag << " |\n";

    std::cout << " | Furnizori activi    : "
              << std::setw(24) << furnizori << " |\n";

    std::cout << " | Valoare stoc        : "
              << std::setw(24) << formatPret(valoare) << " |\n";

    std::cout << " | Cel mai scump       : "
              << std::setw(24) << numeScurt << " |\n";

    std::cout << " +------------------------------------------------+\n";

    std::cout << Color::RESET << "\n";
}

inline void printMenu(int numarProduse) {
    std::cout << Color::BG_DARK << Color::BWHITE << Color::BOLD;
    std::cout << " ┌─────────────────────────────────────────┐ \n";
    std::cout << " │            MENIU PRINCIPAL              │ \n";
    std::cout << " ├─────────────────────────────────────────┤ \n";
    std::cout << " │  " << Color::BCYAN << "1" << Color::BWHITE << "   Adauga produs                      │ \n";
    std::cout << " │  " << Color::BCYAN << "2" << Color::BWHITE << "   Vizualizeaza stocuri               │ \n";
    std::cout << " │  " << Color::BCYAN << "3" << Color::BWHITE << "   Restock (intrare marfa)            │ \n";
    std::cout << " │  " << Color::BCYAN << "4" << Color::BWHITE << "   Vanzare (iesire marfa)             │ \n";
    std::cout << " │  " << Color::BCYAN << "5" << Color::BWHITE << "   Raport alerte stoc scazut          │ \n";
    std::cout << " │  " << Color::BCYAN << "6" << Color::BWHITE << "   Gestionare furnizori               │ \n";
    std::cout << " │  " << Color::BCYAN << "7" << Color::BWHITE << "   Istoric tranzactii                 │ \n";
    std::cout << " │  " << Color::BCYAN << "8" << Color::BWHITE << "   Elimina produs                     │ \n";
    std::cout << " │  " << Color::BCYAN  << "9" << Color::BWHITE << "   Genereaza comenzi reaprovizionare  │ \n";
    std::cout << " │  " << Color::BRED  << "0" << Color::BWHITE << "   Iesire                             │ \n";
    std::cout << " └─────────────────────────────────────────┘ \n";
    std::cout << Color::RESET;
    std::cout << Color::DIM << "  Produse in depozit: " << Color::BGREEN << numarProduse
              << Color::RESET << "\n\n";
    std::cout << Color::BYELLOW << "  Alege optiunea: " << Color::BWHITE;
}

inline std::string stockBar(int cantitate, int prag, int maxVizual = 20) {
    int refMax = std::max(prag * 3, cantitate + 1);
    int filled = (cantitate * maxVizual) / refMax;
    filled = std::max(0, std::min(filled, maxVizual));

    std::string bar, col;
    double ratio = (double)cantitate / (double)(prag > 0 ? prag : 1);
    if (ratio <= 1.0)      col = Color::BRED;
    else if (ratio <= 2.0) col = Color::BYELLOW;
    else                   col = Color::BGREEN;

    bar += col;
    for (int i = 0; i < filled; i++)         bar += "█";
    bar += Color::DIM;
    for (int i = filled; i < maxVizual; i++) bar += "░";
    bar += Color::RESET;
    return bar;
}

inline void printSuccess(const std::string& msg) {
    std::cout << "\n  " << Color::BG_GREEN << Color::BOLD << " ✔  " << Color::RESET
              << Color::BGREEN << " " << msg << Color::RESET << "\n";
}

inline void printError(const std::string& msg) {
    std::cout << "\n  " << Color::BG_RED << Color::BOLD << " ✖  " << Color::RESET
              << Color::BRED << " " << msg << Color::RESET << "\n";
}

inline void printWarning(const std::string& msg) {
    std::cout << "\n  " << Color::BYELLOW << " ⚠  " << msg << Color::RESET << "\n";
}

inline void printSeparator() {
    std::cout << Color::DIM << "  " << repeat("─", 54) << Color::RESET << "\n";
}

inline void printSectionHeader(const std::string& title, const std::string& icon = "📌") {
    std::cout << "\n";
    printSeparator();
    std::cout << "  " << Color::BOLD << Color::BBLUE << icon << "  " << title << Color::RESET << "\n";
    printSeparator();
}

inline void waitEnter() {
    std::cout << "\n  " << Color::DIM << "Apasa ENTER pentru a continua..." << Color::RESET;
    std::cin.ignore();
    std::cin.get();
}



} // namespace UI

#endif