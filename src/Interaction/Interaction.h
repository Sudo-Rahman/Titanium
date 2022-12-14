//
// Created by rahman on 09/10/22.
//

#ifndef PROJET_QT_INTERACTION_H
#define PROJET_QT_INTERACTION_H

#include <iostream>
#include <string>
#include <ctime>
#include "../Tache/ListTache.h"

/**
 * @details Classe Interaction qui modélise une interaction avec les primitives c/c++.
 */
class Interaction
{

public:
    explicit Interaction(std::string);

    Interaction(const Interaction &interaction);

    explicit Interaction();

    ~Interaction();

private:
    std::string contenu{};
    uint64_t dateCreation{};
    uint64_t dateModif{};
    ListTache *lstTache{};

public:

    bool operator<(const Interaction &) const;

    bool operator>(const Interaction &) const;

    bool operator<=(const Interaction &) const;

    bool operator>=(const Interaction &) const;

    friend std::ostream &operator<<(std::ostream &, const Interaction &);

    [[nodiscard]] ListTache *getLstTache() const;

    void setLstTache(ListTache *);

    void setLstTache(const ListTache &);

    [[nodiscard]] uint64_t getDateModif() const;

    void setDateModif(uint64_t);

    [[nodiscard]] uint64_t getDateCreation() const;

    void setDateCreation(uint64_t);

    [[nodiscard]] const std::string &getContenu() const;

    void setContenu(const std::string &);

    void modif();

    void addTache(const Tache &);

};

#endif //PROJET_QT_INTERACTION_H