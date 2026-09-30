#ifndef ISPIT_H
#define ISPIT_H



#include "Student.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct Pristupnik {
    std::string mbag;
    int rbIzlaska{};
    int pismeni{};
    int usmeni{};

class Ispit {
protected:

    std::string datum;
    std::string sifraKolegija;
    std::vector<Pristupnik> pristupnici;


    std::map<std::string, Student>* studenti{nullptr};

public:
    Ispit() = default;


    void poveziStudente(std::map<std::string, Student>& s) { studenti = &s; }

    const std::string& getDatum() const { return datum; }
    const std::string& getSifraKolegija() const { return sifraKolegija; }
    const std::vector<Pristupnik>& getPristupnici() const { return pristupnici; }


    const Student* nadiStudenta(const std::string& mbag) const;
    Student* nadiStudenta(const std::string& mbag);


    int brojProsliPismeni() const;
    int brojProsliUsmeni() const;


    friend std::istream& operator>>(std::istream& is, Ispit& isp);
    friend std::ostream& operator<<(std::ostream& os, const Ispit& isp);
};

#endif
