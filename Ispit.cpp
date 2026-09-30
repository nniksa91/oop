#include "Ispit.h"
#include <algorithm>
#include <sstream>
#include <stdexcept>


const Student* Ispit::nadiStudenta(const std::string& mbag) const {
    if (!studenti) {
        return nullptr;
    }
    auto it = studenti->find(mbag);
    if (it == studenti->end()) {
        return nullptr;
    }
    return &it->second;
}

Student* Ispit::nadiStudenta(const std::string& mbag) {
    if (!studenti) {
        return nullptr;
    }
    auto it = studenti->find(mbag);
    if (it == studenti->end()) {
        return nullptr;
    }
    return &it->second;
}


int Ispit::brojProsliPismeni() const {
    return static_cast<int>(std::count_if(
        pristupnici.begin(),
        pristupnici.end(),
        [](const Pristupnik& p) { return p.pismeni >= 2; } // ← lambda
    ));
}

int Ispit::brojProsliUsmeni() const {
    return static_cast<int>(std::count_if(
        pristupnici.begin(),
        pristupnici.end(),
        [](const Pristupnik& p) { return p.usmeni >= 2; } // ← lambda
    ));
}


std::istream& operator>>(std::istream& is, Ispit& isp) {

    if (!(is >> isp.datum >> isp.sifraKolegija)) {
        throw std::runtime_error("Neispravno zaglavlje ispit.txt (ocekivan datum i sifra kolegija)");
    }

    std::string restOfHeader;
    std::getline(is, restOfHeader);


    isp.pristupnici.clear();
    std::string line;

    while (std::getline(is, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::stringstream ss(line);
        Pristupnik p;
        std::string sRb, sPismeni, sUsmeni;

        if (!std::getline(ss, p.mbag, ';') ||
            !std::getline(ss, sRb, ';') ||
            !std::getline(ss, sPismeni, ';') ||
            !std::getline(ss, sUsmeni)) {
            throw std::runtime_error("Neispravan red pristupnika: " + line);
        }

        try {
            p.rbIzlaska = std::stoi(sRb);
            p.pismeni = std::stoi(sPismeni);
            p.usmeni = std::stoi(sUsmeni);
        } catch (const std::exception&) {
            throw std::runtime_error("Neispravan brojcani podatak u retku: " + line);
        }

        isp.pristupnici.push_back(p);
    }

    return is;
}

std::ostream& operator<<(std::ostream& os, const Ispit& isp) {
    os << isp.datum << ' ' << isp.sifraKolegija << '\n';
    for (const auto& p : isp.pristupnici) {
        os << p.mbag << ';' << p.rbIzlaska << ';' << p.pismeni << ';' << p.usmeni << '\n';
    }
    return os;
}
