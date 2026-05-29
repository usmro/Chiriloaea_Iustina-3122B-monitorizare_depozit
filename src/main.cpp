#include "../include/Depozit.h"
#include "../include/Exceptions.h"
#include "../include/Tranzactie.h"
#include "../include/UI.h"
#include <termios.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <ncurses.h>
void testNcurses() {

    initscr();

    clear();

    mvprintw(5, 10, "NCURSES FUNCTIONEAZA!");

    mvprintw(7, 10, "Apasa orice tasta...");

    refresh();

    getch();

    endwin();
}
std::string citesteParola() {

    termios vechi, nou;

    tcgetattr(STDIN_FILENO, &vechi);

    nou = vechi;

    nou.c_lflag &= ~ECHO;

    tcsetattr(STDIN_FILENO, TCSANOW, &nou);

    std::string parola;

    char ch;

    while (true) {

        ch = getchar();

        if (ch == '\n')
            break;

        if (ch == 127 || ch == '\b') {

            if (!parola.empty()) {

                parola.pop_back();

                std::cout << "\b \b";
                std::cout.flush();
            }
        }
        else {

            parola += ch;

            std::cout << '*' << std::flush;
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &vechi);

    std::cout << "\n";

    return parola;
}
bool autentificare() {

    const std::string USER_CORECT = "admin";

    const std::string PAROLA_CORECTA = "1234";

    int incercari = 3;

    while (incercari > 0) {

        std::string user;
        std::string parola;

        UI::clearScreen();

        std::cout << Color::BCYAN;

        std::cout << R"(

==================================================
             SISTEM SECURIZAT LOGIN
==================================================

)";

        std::cout << Color::RESET;

        std::cout << " Utilizator: ";
        std::getline(std::cin, user);

        std::cout << " Parola: ";
        parola = citesteParola();

        if (user == USER_CORECT &&
            parola == PAROLA_CORECTA) {

            std::cout << "\n";

            UI::printSuccess("ACCESS GRANTED");

            std::this_thread::sleep_for(
                std::chrono::milliseconds(1000)
            );

            return true;
        }

        incercari--;

        std::cout << "\n";

        UI::printError(
            "Date incorecte! Incercari ramase: "
            + std::to_string(incercari)
        );

        std::this_thread::sleep_for(
            std::chrono::milliseconds(1500)
        );
    }

    UI::clearScreen();

    std::cout << Color::BRED << Color::BOLD;

    std::cout << R"(

==================================================
                  ACCESS DENIED
==================================================

)";

    std::cout << Color::RESET;

    return false;
}
void loadingScreen() {

    UI::clearScreen();

    std::cout << Color::BG_DARK << Color::BGREEN;

    std::cout << "\n\n";
    std::cout << "      Initializare sistem";

    std::cout.flush();

    for (int i = 0; i < 20; i++) {

        std::cout << "█";
        std::cout.flush();

        std::this_thread::sleep_for(
            std::chrono::milliseconds(80)
        );
    }

    std::cout << " 100%";

    std::cout << Color::RESET;

    std::this_thread::sleep_for(
        std::chrono::milliseconds(700)
    );
}
inline void printBanner() {

    std::cout << Color::BCYAN;

    std::cout << R"(

███╗   ███╗ ██████╗ ███╗   ██╗██╗████████╗ ██████╗ ██████╗
████╗ ████║██╔═══██╗████╗  ██║██║╚══██╔══╝██╔═══██╗██╔══██╗
██╔████╔██║██║   ██║██╔██╗ ██║██║   ██║   ██║   ██║██████╔╝
██║╚██╔╝██║██║   ██║██║╚██╗██║██║   ██║   ██║   ██║██╔══██╗
██║ ╚═╝ ██║╚██████╔╝██║ ╚████║██║   ██║   ╚██████╔╝██║  ██║
╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚═╝   ╚═╝    ╚═════╝ ╚═╝  ╚═╝

                SISTEM GESTIUNE DEPOZIT
    )";

    std::cout << Color::RESET << "\n";
}
int citesteInt(const std::string& prompt) {
    int val;
    std::cout << "  " << Color::BCYAN << prompt << Color::BWHITE;
    while (!(std::cin >> val)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        UI::printError("Valoare invalida. Reincearca: ");
        std::cout << "  " << Color::BCYAN << prompt << Color::BWHITE;
    }
    return val;
}

double citesteDouble(const std::string& prompt) {
    double val;
    std::cout << "  " << Color::BCYAN << prompt << Color::BWHITE;
    while (!(std::cin >> val) || val < 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        UI::printError("Valoare invalida. Reincearca: ");
        std::cout << "  " << Color::BCYAN << prompt << Color::BWHITE;
    }
    return val;
}

void adaugaProdus(Depozit& depozit) {
    UI::printSectionHeader("ADAUGA PRODUS NOU", "➕");
    try {
        int id = citesteInt("ID produs: ");
        std::cin.ignore();
        std::string num;
        std::cout << "  " << Color::BCYAN << "Nume produs: " << Color::BWHITE;
        std::getline(std::cin, num);
        int cant    = citesteInt("Cantitate initiala: ");
        double pret = citesteDouble("Pret unitar (RON): ");
        int prag    = citesteInt("Prag de alerta: ");

        depozit.adaugaProdus(Produs(id, num, cant, pret, prag));
        depozit.salveazaInFisier("src/produse.txt");
        UI::printSuccess("Produs \"" + num + "\" adaugat cu succes!");
    } catch (const std::exception& e) {
        UI::printError(e.what());
    }
    UI::waitEnter();
}

void eliminaProdus(Depozit& depozit) {
    UI::printSectionHeader("ELIMINA PRODUS", "🗑️");
    try {
        int id = citesteInt("ID produs de eliminat: ");
        depozit.eliminaProdus(id);
        depozit.salveazaInFisier("src/produse.txt");
        UI::printSuccess("Produs eliminat cu succes!");
    } catch (const std::exception& e) {
        UI::printError(e.what());
    }
    UI::waitEnter();
}

void restock(Depozit& depozit) {
    UI::printSectionHeader("RESTOCK - INTRARE MARFA", "🔄");
    try {
        int id   = citesteInt("ID produs: ");
        int cant = citesteInt("Cantitate de adaugat: ");
        std::cin.ignore();
        std::string obs;
        std::cout << "  " << Color::BCYAN << "Observatii (optional): " << Color::BWHITE;
        std::getline(std::cin, obs);

        TranzactieIntrare t(id, cant, obs);
        depozit.aplicaTranzactie(t);
        depozit.salveazaInFisier("src/produse.txt");
        UI::printSuccess("Stoc actualizat! +" + std::to_string(cant) + " unitati.");
    } catch (const std::exception& e) {
        UI::printError(e.what());
    }
    UI::waitEnter();
}

void vanzare(Depozit& depozit) {
    UI::printSectionHeader("VANZARE - IESIRE MARFA", "🛒");
    try {
        int id   = citesteInt("ID produs: ");
        int cant = citesteInt("Cantitate vanduta: ");
        std::cin.ignore();
        std::string obs;
        std::cout << "  " << Color::BCYAN << "Observatii (optional): " << Color::BWHITE;
        std::getline(std::cin, obs);

        TranzactieIesire t(id, cant, obs);
        depozit.aplicaTranzactie(t);
        depozit.salveazaInFisier("src/produse.txt");

        const Produs& p = depozit.getProdus(id);
        double total = cant * p.getPret();
        UI::printSuccess("Vanzare inregistrata! -" + std::to_string(cant)
                         + " unitati  |  Total: " + UI::formatPret(total));

        if (p.subPrag())
            UI::printWarning("ATENTIE: Stocul pentru \"" + p.getNume()
                             + "\" a scazut sub pragul de alerta ("
                             + std::to_string(p.getCantitate()) + "/"
                             + std::to_string(p.getPragAlerta()) + ")!");
    } catch (const std::exception& e) {
        UI::printError(e.what());
    }
    UI::waitEnter();
}

void gestionareFurnizori(Depozit& depozit) {
    UI::printSectionHeader("GESTIONARE FURNIZORI", "🏭");
    std::cout << "\n  " << Color::BCYAN << "1" << Color::BWHITE << "  Adauga furnizor\n"
              << "  " << Color::BCYAN << "2" << Color::BWHITE << "  Asociaza furnizor la produs\n"
              << "  " << Color::BCYAN << "3" << Color::BWHITE << "  Lista furnizori\n\n"
              << Color::RESET;
    int opt = citesteInt("Optiune: ");
    try {
        if (opt == 1) {
            int id = citesteInt("ID furnizor: ");
            std::cin.ignore();
            std::string num, tel, email;
            std::cout << "  " << Color::BCYAN << "Nume: "     << Color::BWHITE; std::getline(std::cin, num);
            std::cout << "  " << Color::BCYAN << "Telefon: "  << Color::BWHITE; std::getline(std::cin, tel);
            std::cout << "  " << Color::BCYAN << "Email: "    << Color::BWHITE; std::getline(std::cin, email);
            depozit.adaugaFurnizor(Furnizor(id, num, tel, email));
            UI::printSuccess("Furnizor \"" + num + "\" adaugat!");
        } else if (opt == 2) {
            int pid = citesteInt("ID produs: ");
            int fid = citesteInt("ID furnizor: ");
            depozit.asociazaFurnizor(pid, fid);
            UI::printSuccess("Furnizor asociat produsului cu succes!");
        } else if (opt == 3) {
            const auto& furnizori = depozit.getFurnizori();
            if (furnizori.empty()) {
                UI::printWarning("Nu exista furnizori inregistrati.");
            } else {
                std::cout
                    << std::left
                    << std::setw(10)  << "ID"
                    << std::setw(23) << "NUME"
                    << std::setw(20) << "TELEFON"
                    << std::setw(30) << "EMAIL"
                    << "\n";
                UI::printSeparator();
                for (const auto& f : furnizori)
                    std::cout << "  " << Color::BWHITE
                              << std::setw(10)  << f.second.getId()
                              << std::setw(23) << f.second.getNume()
                              << std::setw(20) << f.second.getTelefon()
                              << std::setw(30) << f.second.getEmail() << Color::RESET << "\n";
            }
        }
    } catch (const std::exception& e) {
        UI::printError(e.what());
    }
    UI::waitEnter();
}

void afiseazaIstoric(const Depozit& depozit) {
    UI::printSectionHeader("ISTORIC TRANZACTII", "📜");
    const auto& istoric = depozit.getIstoric();
    if (istoric.empty()) {
        UI::printWarning("Nu exista tranzactii inregistrate.");
        UI::waitEnter();
        return;
    }
    std::cout << "\n  " << Color::BOLD << Color::BCYAN
              << std::left
              << std::setw(22) << "DATA/ORA"
              << std::setw(12) << "TIP"
              << std::setw(8)  << "ID PROD"
              << std::setw(10) << "CANT."
              << "OBSERVATII\n" << Color::RESET;
    UI::printSeparator();
    for (const auto& e : istoric) {
        std::string tipColor = (e.tip == "INTRARE") ? Color::BGREEN : Color::BRED;
        std::string icon     = (e.tip == "INTRARE") ? "▲ " : "▼ ";
        std::cout << "  " << Color::DIM  << std::setw(22) << e.timestamp << Color::RESET
                  << tipColor << icon << std::setw(10) << e.tip << Color::RESET
                  << Color::BWHITE << std::setw(8) << e.produsId
                  << std::setw(10) << e.cantitate
                  << Color::DIM << e.observatii << Color::RESET << "\n";
    }
    UI::waitEnter();
}

int main() {
    
    if (!autentificare()) {

    UI::printError("Autentificare esuata!");

    return 0;
                }

    loadingScreen();
    Depozit depozit;
    depozit.incarcaDinFisier("../src/produse.txt");
    depozit.incarcaFurnizori("../src/furnizori.txt");

    int optiune;
    do {
        UI::printBanner();
        UI::printDashboard(
    depozit.numarProduse(),
    depozit.numarProduseSubPrag(),
    depozit.getFurnizori().size(),
    depozit.valoareTotalaStoc(),
    depozit.produsCelMaiScump().getNume()
);
        UI::printMenu(depozit.numarProduse());

        while (!(std::cin >> optiune)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            UI::printError("Optiune invalida!");
            std::cout << Color::BYELLOW << "  Alege optiunea: " << Color::BWHITE;
        }
        std::cout << Color::RESET;

    switch (optiune) {

    case 1:
        UI::clearScreen();
        adaugaProdus(depozit);
        break;

    case 2:
        UI::clearScreen();
        depozit.afiseazaProduse();
        UI::waitEnter();
        break;

    case 3:
        UI::clearScreen();
        restock(depozit);
        break;

    case 4:
        UI::clearScreen();
        vanzare(depozit);
        break;

    case 5:
        UI::clearScreen();
        depozit.genereazaRaportStocMic();
        UI::waitEnter();
        break;

    case 6:
        UI::clearScreen();
        gestionareFurnizori(depozit);
        break;

    case 7:
        UI::clearScreen();

        UI::printSectionHeader(
            "ISTORIC TRANZACTII",
            "📜"
        );

        depozit.afiseazaIstoricFisier();

        UI::waitEnter();
        break;

    case 8:
        UI::clearScreen();
        eliminaProdus(depozit);
        break;

    case 9:
        UI::clearScreen();

        depozit.genereazaComenziReaprovizionare();

        UI::waitEnter();
        break;

    case 0:
        UI::clearScreen();

        std::cout
            << Color::BCYAN
            << Color::BOLD
            << "\n  La revedere! 👋\n\n"
            << Color::RESET;

        break;

    default:
        UI::clearScreen();

        UI::printError(
            "Optiune invalida!"
        );

        UI::waitEnter();
    }
    } while (optiune != 0);

    return 0;
}
