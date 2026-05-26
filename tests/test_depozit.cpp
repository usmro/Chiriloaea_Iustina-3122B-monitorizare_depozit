#include "../include/Depozit.h"
#include "../include/Exceptions.h"
#include "../include/Tranzactie.h"
#include <iostream>
#include <cassert>
#include <string>

int passed = 0, failed = 0;

#define TEST(name, expr) \
    do { \
        if (expr) { \
            std::cout << "  \033[92m✔\033[0m  " << (name) << "\n"; \
            passed++; \
        } else { \
            std::cout << "  \033[91m✖\033[0m  " << (name) << "\n"; \
            failed++; \
        } \
    } while(0)

#define TEST_THROWS(name, stmt, ExcType) \
    do { \
        bool caught = false; \
        try { stmt; } catch (const ExcType&) { caught = true; } \
        TEST(name, caught); \
    } while(0)

void testProdus() {
    std::cout << "\n\033[1;94m  [Produs]\033[0m\n";
    Produs p(1, "Test", 100, 10.0, 20);
    TEST("Constructor seteaza corect ID",         p.getId() == 1);
    TEST("Constructor seteaza corect cantitate",  p.getCantitate() == 100);
    TEST("subPrag() false cand cantitate > prag", !p.subPrag());
    p += 50;
    TEST("operator+= adauga cantitate",           p.getCantitate() == 150);
    p -= 130;
    TEST("operator-= scade cantitate",            p.getCantitate() == 20);
    TEST("subPrag() true cand cantitate == prag", p.subPrag());
    TEST_THROWS("operator-= arunca StocInsuficient",  p -= 100,  StocInsuficientException);
    TEST_THROWS("operator+= arunca CantitateInvalida", p += -5,  CantitateInvalidaException);
}

void testDepozit() {
    std::cout << "\n\033[1;94m  [Depozit]\033[0m\n";
    Depozit d;
    d.adaugaProdus(Produs(1, "Produs A", 50, 9.99, 10));
    d.adaugaProdus(Produs(2, "Produs B",  5, 4.99, 10));
    TEST("Depozit are 2 produse", d.numarProduse() == 2);
    TEST_THROWS("adaugaProdus arunca ProdusExistent",
                d.adaugaProdus(Produs(1, "X", 0, 0, 0)), ProdusExistentException);
    TEST_THROWS("eliminaProdus arunca ProdusInexistent",
                d.eliminaProdus(999), ProdusInexistentException);
    d.eliminaProdus(2);
    TEST("Dupa eliminare, 1 produs ramas", d.numarProduse() == 1);
    TEST_THROWS("getProdus arunca ProdusInexistent",
                d.getProdus(2), ProdusInexistentException);
}

void testTranzactii() {
    std::cout << "\n\033[1;94m  [Tranzactie]\033[0m\n";
    Depozit d;
    d.adaugaProdus(Produs(10, "Item", 30, 5.0, 5));
    TranzactieIntrare ti(10, 20, "Restock");
    d.aplicaTranzactie(ti);
    TEST("TranzactieIntrare mareste stocul la 50",  d.getProdus(10).getCantitate() == 50);
    TEST("TranzactieIntrare tip == INTRARE",         ti.getTip() == "INTRARE");
    TEST("TranzactieIntrare delta pozitiva",         ti.getDelta() == 20);
    TranzactieIesire te(10, 15, "Vanzare");
    d.aplicaTranzactie(te);
    TEST("TranzactieIesire scade stocul la 35",  d.getProdus(10).getCantitate() == 35);
    TEST("TranzactieIesire tip == IESIRE",        te.getTip() == "IESIRE");
    TEST("TranzactieIesire delta negativa",       te.getDelta() == -15);
    TEST_THROWS("Tranzactie cu cantitate 0 arunca invalid_argument",
                TranzactieIntrare bad(10, 0), std::invalid_argument);
    TEST("Istoricul are 2 tranzactii", d.getIstoric().size() == 2);
}

void testFurnizori() {
    std::cout << "\n\033[1;94m  [Furnizori]\033[0m\n";
    Depozit d;
    d.adaugaProdus(Produs(1, "P", 10, 1.0, 2));
    d.adaugaFurnizor(Furnizor(100, "AcmeCorp", "0700", "a@b.com"));
    TEST("Furnizor adaugat", d.getFurnizori().size() == 1);
    TEST_THROWS("Furnizor duplicat arunca FurnizorExistent",
                d.adaugaFurnizor(Furnizor(100, "X")), FurnizorExistentException);
    d.asociazaFurnizor(1, 100);
    TEST("Asociere furnizor la produs", d.getProdus(1).getFurnizorId() == 100);
    TEST_THROWS("Asociere furnizor inexistent arunca exceptie",
                d.asociazaFurnizor(1, 999), FurnizorInexistentException);
}

void testReCommandat() {
    std::cout << "\n\033[1;94m  [produseDeReCommandat]\033[0m\n";
    Depozit d;
    d.adaugaProdus(Produs(1, "A",  3, 1.0, 10));
    d.adaugaProdus(Produs(2, "B", 50, 1.0, 10));
    d.adaugaProdus(Produs(3, "C",  1, 1.0, 10));
    auto lista = d.produseDeReCommandat();
    TEST("Lista contine 2 produse sub prag",      lista.size() == 2);
    TEST("Sortate dupa cantitate: primul e minim", lista[0].getCantitate() <= lista[1].getCantitate());
    TEST("Produsul B (50) nu e in lista",          lista[0].getId() != 2 && lista[1].getId() != 2);
}

int main() {
    std::cout << "\n\033[1;97m╔══════════════════════════════════════╗\033[0m\n";
    std::cout << "\033[1;97m║       TESTE UNITARE - DEPOZIT        ║\033[0m\n";
    std::cout << "\033[1;97m╚══════════════════════════════════════╝\033[0m\n";
    testProdus();
    testDepozit();
    testTranzactii();
    testFurnizori();
    testReCommandat();
    std::cout << "\n  ────────────────────────────────────\n";
    std::cout << "  \033[1;92mPASED: " << passed << "\033[0m  "
              << "\033[1;91mFAILED: " << failed << "\033[0m\n";
    return failed > 0 ? 1 : 0;
}