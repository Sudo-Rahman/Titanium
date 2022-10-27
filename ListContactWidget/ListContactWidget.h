//
// Created by Rahman  Yilmaz on 06/10/2022.
//

#ifndef UNTITLED2_MODIFICATIONDIALOG_H
#define UNTITLED2_MODIFICATIONDIALOG_H

#include <QWidget>
#include <QScrollArea>
#include <QLayout>
#include "../Contact/TraductionQtStd.h"
#include "ListContactWidget.h"
#include "GroupeBoxContact.h"


class ListContactWidget : public QWidget
{
Q_OBJECT

public:
    explicit ListContactWidget(StdListContact *lst, QWidget *parent);

    void addContactBox(StdContact *contact);

    void setLastConctactselected(GroupeBoxContact *lastConctactselected);

    StdListContact *getLstContact() const;

private:
    QVBoxLayout *lay{};
    QScrollArea *scrollArea{};
    QWidget *scrollAreaWidget{};
    QVBoxLayout *layScrollArea{};
    StdListContact *lstContact{};

    GroupeBoxContact *lastConctactselected{};


};


#endif //UNTITLED2_MODIFICATIONDIALOG_H
