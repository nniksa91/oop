

#include "Student.h"
#include "Ispit.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

int main(int argc, char* argv[]) {

    if (argc < 3) {
        std::cerr << "Uporaba: " << argv[0] << " studenti.txt ispit.txt\n";
        return 1;
    }

    std::map<std::string, Student> studenti;


    try {
        std::ifstream ulazStudenti(argv[1]);
        if (!ulazStudenti) {
            throw std::runtime_error(std::string("Ne mogu otvoriti datoteku studenata: ") + argv[1]);
        }

        Student s;
        while (ulazStudenti >> s) {
            studenti[s.getMbag()] = s;
        }
    } catch (const std::exception& e) {
        std::cerr << "Greska pri ucitavanju studenata: " << e.what() << '\n';
        return 1;
    }


    Ispit ispit;
    ispit.poveziStudente(studenti);


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
        const Student* s = ispit.nadiStudenta(p.mbag);
        if (s) {
            std::cout << std::left
                      << std::setw(14) << s->getIme()
                      << std::setw(14) << s->getPrezime()
                      << std::setw(10) << p.pismeni
                      << std::setw(10) << p.usmeni
                      << '\n';
        } else {

            std::cout << std::left
                      << std::setw(14) << "?"
                      << std::setw(14) << p.mbag
                      << std::setw(10) << p.pismeni
                      << std::setw(10) << p.usmeni
                      << '\n';
        }
    }

    std::cout << std::string(48, '-') << '\n';
    std::cout << "Proslo pismeni: " << ispit.brojProsliPismeni() << '\n';
    std::cout << "Proslo usmeni:  " << ispit.brojProsliUsmeni() << '\n';

    return 0;
}

