/*
 * ============================================================================
 * MAPA ZADATAKA U OVOJ DATOTECI (glavni.cpp)
 * ============================================================================
 * (1)        Projekt + git: ova datoteka je ulazna točka programa;
 *            git init/commit/tag/bundle rade se u terminalu (ne u kodu).
 *
 * (5+4)      Klasa Student — Student.h / Student.cpp (ovdje se samo koristi).
 *
 * (5+3)      Klasa Ispit + nadiStudenta — Ispit.h / Ispit.cpp.
 *
 * (5+2+3)    Lambde + dijeljena mapa — u Ispit.cpp; ovdje poveziStudente().
 *
 * (5+3+3)    OVA DATOTEKA: argv[1]/studenti → map, argv[2]/ispit → Ispit,
 *            try/catch, tablični ispis, brojevi prošlih na dnu.
 *
 * (1)+(1)    git bundle OOPJESENSKI --all + test datoteke studenti.txt / ispit.txt
 * ============================================================================
 */

#include "Student.h"
#include "Ispit.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

int main(int argc, char* argv[]) {
    /*
     * --- ZADATAK (5+3+3): argumenti naredbenog retka ---
     * Zašto argv, a ne hardkodirane putanje:
     *  - ispit traži: 1. arg = studenti.txt, 2. arg = ispit.txt
     *  - iste binarne datoteke rade s Merlinovim testnim fajlovima
     */
    if (argc < 3) {
        std::cerr << "Uporaba: " << argv[0] << " studenti.txt ispit.txt\n";
        return 1;
    }

    /*
     * --- ZADATAK (5+3+3): std::map<mbag, Student> ---
     * Zašto map, a ne vector:
     *  - zadatak eksplicitno predlaže mapu (ključ = mbag)
     *  - kasnije Ispit::nadiStudenta radi brzi find po mbag-u
     *  - mbag kao string čuva vodeće nule
     */
    std::map<std::string, Student> studenti;

    /*
     * --- ZADATAK (5+3+3) + (5+4): učitavanje studenata uz IZNIMKE ---
     * while (ulaz >> s) koristi operator>> iz Student.cpp
     *  → komentari '#' se već filtriraju u operatoru
     * throw / catch: "predvidjeti iznimke i javiti grešku"
     */
    try {
        std::ifstream ulazStudenti(argv[1]);
        if (!ulazStudenti) {
            throw std::runtime_error(std::string("Ne mogu otvoriti datoteku studenata: ") + argv[1]);
        }

        Student s;
        while (ulazStudenti >> s) {
            studenti[s.getMbag()] = s; // ključ = mbag
        }
    } catch (const std::exception& e) {
        std::cerr << "Greska pri ucitavanju studenata: " << e.what() << '\n';
        return 1;
    }

    /*
     * --- ZADATAK (5+2+3): dijeljena agregatna struktura ---
     * Mapa ostaje ovdje u main-u; Ispit dobije samo pokazivač (poveziStudente).
     * Zašto prije učitavanja ispita: učitavanje ne treba mapu, ali ispis treba —
     * povezujemo odmah da ne zaboravimo.
     */
    Ispit ispit;
    ispit.poveziStudente(studenti);

    /*
     * --- ZADATAK (5+3+3) + (5+3)/(5+2+3): učitavanje ispit.txt ---
     * operator>> u Ispit.cpp čita zaglavlje pa pristupnike.
     */
    try {
        std::ifstream ulazIspit(argv[2]);
        if (!ulazIspit) {
            throw std::runtime_error(std::string("Ne mogu otvoriti datoteku ispita: ") + argv[2]);
        }
        ulazIspit >> ispit;
    } catch (const std::exception& e) {
        std::cerr << "Greska pri ucitavanju ispita: " << e.what() << '\n';
        return 1;
    }

    /*
     * --- ZADATAK (5+3+3): tablični ispis ---
     * Zahtjev: ime, prezime, ocjene + zaglavlje stupaca.
     * Ime/prezime NISU u ispit.txt → dohvat preko nadiStudenta(mbag) (5+3).
     * iomanip (setw, left) → uredna konzolna tablica.
     */
    std::cout << "Ispitni rok: " << ispit.getDatum()
              << "   Kolegij: " << ispit.getSifraKolegija() << "\n\n";

    std::cout << std::left
              << std::setw(14) << "Ime"
              << std::setw(14) << "Prezime"
              << std::setw(10) << "Pismeni"
              << std::setw(10) << "Usmeni"
              << '\n';
    std::cout << std::string(48, '-') << '\n';

    for (const auto& p : ispit.getPristupnici()) {
        const Student* s = ispit.nadiStudenta(p.mbag); // (5+3) dinamički dohvat
        if (s) {
            std::cout << std::left
                      << std::setw(14) << s->getIme()
                      << std::setw(14) << s->getPrezime()
                      << std::setw(10) << p.pismeni
                      << std::setw(10) << p.usmeni
                      << '\n';
        } else {
            /* Obrana: pristupnik postoji, ali student nije u mapi. */
            std::cout << std::left
                      << std::setw(14) << "?"
                      << std::setw(14) << p.mbag
                      << std::setw(10) << p.pismeni
                      << std::setw(10) << p.usmeni
                      << '\n';
        }
    }

    /*
     * --- ZADATAK (5+3+3) + (5+2+3): sažetak na dnu tablice ---
     * Koriste se gotove metode s lambdama — ne brojimo ručno u main-u,
     * jer zadatak traži da se prebrojavanje radi preko članova klase.
     */
    std::cout << std::string(48, '-') << '\n';
    std::cout << "Proslo pismeni: " << ispit.brojProsliPismeni() << '\n';
    std::cout << "Proslo usmeni:  " << ispit.brojProsliUsmeni() << '\n';

    return 0;
}

/*
 * ============================================================================
 * GIT DIO ISPITA (1)+(1) — nije u kodu, nego u terminalu (mapa Ispit/):
 * ============================================================================
 *   git init
 *   git add ... && git commit -m "Početak 1. jesenskog roka" && git tag Početno
 *   ... nakon svakog zadatka: commit + tag (zad1, zad2, ...)
 *   git bundle create OOPJESENSKI --all
 *   predaja OOPJESENSKI na Merlin
 *
 * Pokretanje testa:
 *   ./ispit studenti.txt ispit.txt
 * ============================================================================
 */
