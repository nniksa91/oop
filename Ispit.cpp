#include "Ispit.h"
#include <algorithm>
#include <sstream>
#include <stdexcept>

/*
 * ============================================================================
 * ZADATAK (5+3) — nadiStudenta: dinamički dohvat iz dijeljene mape
 * ============================================================================
 * Zašto find(), a ne petlja po vectoru:
 *  - mapa je indeksirana mbag-om → brzo i jasno po zahtjevu "po šifri/mbagu"
 * ============================================================================
 */

const Student* Ispit::nadiStudenta(const std::string& mbag) const {
    if (!studenti) {
        return nullptr; // nije pozvan poveziStudente()
    }
    auto it = studenti->find(mbag);
    if (it == studenti->end()) {
        return nullptr; // mbag iz ispita nema para u studenti.txt
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

/*
 * ============================================================================
 * ZADATAK (5+2+3) — brojanje "prošlih" uz LAMBDA
 * ============================================================================
 * Zašto lambda unutar count_if:
 *  - tekst ispita: "Te funkcije za prebrajenje moraju koristiti lambda
 *    konstrukcije unutar klase!"
 *  - count_if prolazi sve pristupnike i broji one za koje predikat vrati true
 *
 * Prag "prošao": ocjena >= 2 (dovoljan). Ako asistent koristi drugi prag,
 * mijenja se samo uvjet u lambdi.
 * ============================================================================
 */

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

/*
 * ============================================================================
 * ZADATAK (5+3) + (5+2+3) — učitavanje ispit.txt
 * ============================================================================
 * Redoslijed kako traži tekst:
 *  1) zaglavlje: datum + šifra kolegija (razmakom odvojeni)
 *  2) zatim retci pristupnika do kraja datoteke
 *
 * Zašto nakon >> datum >> sifra još getline:
 *  - >> ostavlja ostatak retka / '\n' u bufferu; getline to "pojede"
 *    da petlja ne pročita prazan prvi red kao pristupnika
 *
 * Zašto throw pri grešci:
 *  - glavni program (5+3+3) mora uhvatiti iznimke i javiti grešku
 * ============================================================================
 */

std::istream& operator>>(std::istream& is, Ispit& isp) {
    /* 1) Zaglavlje npr.: 31.08.2026 RINF90546 */
    if (!(is >> isp.datum >> isp.sifraKolegija)) {
        throw std::runtime_error("Neispravno zaglavlje ispit.txt (ocekivan datum i sifra kolegija)");
    }

    std::string restOfHeader;
    std::getline(is, restOfHeader); // potroši ostatak retka zaglavlja

    /* 2) Pristupnici: mbag;rb;pismeni;usmeni */
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

/* Simetričan ispis (korisno za debug; tablica u main-u ide drugim putem). */
std::ostream& operator<<(std::ostream& os, const Ispit& isp) {
    os << isp.datum << ' ' << isp.sifraKolegija << '\n';
    for (const auto& p : isp.pristupnici) {
        os << p.mbag << ';' << p.rbIzlaska << ';' << p.pismeni << ';' << p.usmeni << '\n';
    }
    return os;
}
