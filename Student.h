#ifndef STUDENT_H
#define STUDENT_H



#include <iostream>
#include <string>

class Student {
protected:

    std::string mbag;
    std::string ime;
    std::string prezime;
    int skGodina{};
    std::string studij{"RINF"};
    char status{'r'};
    std::string kontakt;
    std::string napomene;

public:
    Student() = default;


    const std::string& getMbag() const { return mbag; }
    const std::string& getIme() const { return ime; }
    const std::string& getPrezime() const { return prezime; }
    int getSkGodina() const { return skGodina; }
    const std::string& getStudij() const { return studij; }
    char getStatus() const { return status; }
    const std::string& getKontakt() const { return kontakt; }
    const std::string& getNapomene() const { return napomene; }


    void setMbag(const std::string& v) { mbag = v; }
    void setIme(const std::string& v) { ime = v; }
    void setPrezime(const std::string& v) { prezime = v; }
    void setSkGodina(int v) { skGodina = v; }
    void setStudij(const std::string& v) { studij = v; }
    void setStatus(char v) { status = v; }
    void setKontakt(const std::string& v) { kontakt = v; }
    void setNapomene(const std::string& v) { napomene = v; }

    friend std::istream& operator>>(std::istream& is, Student& s);
    friend std::ostream& operator<<(std::ostream& os, const Student& s);
};

#endif
