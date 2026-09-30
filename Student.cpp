#include "Student.h"
#include <sstream>
#include <stdexcept>


std::istream& operator>>(std::istream& is, Student& s) {
    std::string line;


    while (true) {
        if (!std::getline(is, line)) {
            return is;
        }
        if (line.empty()) {
            continue;
        }
        if (line[0] == '#') {
            continue;
        }
        break;
    }


    std::stringstream ss(line);
    std::string godStr;
    std::string statusStr;

    if (!std::getline(ss, s.mbag, ';') ||
        !std::getline(ss, s.ime, ';') ||
        !std::getline(ss, s.prezime, ';') ||
        !std::getline(ss, godStr, ';') ||
        !std::getline(ss, s.studij, ';') ||
        !std::getline(ss, statusStr, ';')) {

        throw std::runtime_error("Neispravan zapis studenta: " + line);
    }


    if (!std::getline(ss, s.kontakt, ';')) {
        s.kontakt.clear();
    }
    if (!std::getline(ss, s.napomene)) {
        s.napomene.clear();
    }

    try {
        s.skGodina = godStr.empty() ? 0 : std::stoi(godStr);
    } catch (...) {
        throw std::runtime_error("Neispravna skolska godina: " + godStr);
    }

    s.status = statusStr.empty() ? 'r' : statusStr[0];

    if (s.studij.empty()) {
        s.studij = "RINF";
    }

    return is;
}

std::ostream& operator<<(std::ostream& os, const Student& s) {
    os << s.mbag << ';'
       << s.ime << ';'
       << s.prezime << ';'
       << s.skGodina << ';'
       << s.studij << ';'
       << s.status << ';'
       << s.kontakt << ';'
       << s.napomene;
    return os;
}
