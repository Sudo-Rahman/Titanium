//
// Created by rahman on 09/10/22.
//

#ifndef UNTITLED2_STDLISTCONTACT_H
#define UNTITLED2_STDLISTCONTACT_H

#include <iostream>
#include <list>
#include "StdContact.h"
#include "QtContact.h"
#include "TraductionQtStd.h"

class StdListContact
{
public:
    explicit StdListContact();

    friend std::ostream &operator<<(std::ostream &os, const StdListContact &lst);

    void addContact(const StdContact &contact);

    void addContact(const QtContact &contact);

    std::list<StdContact *> *getLstContact();

private:
    std::list<StdContact *> lstContact;


};


#endif //UNTITLED2_STDLISTCONTACT_H