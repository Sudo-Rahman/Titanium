//
// Created by Rahman  Yilmaz on 06/10/2022.
//

#ifndef UNTITLED2_MODIFCONTACTDIALOG_H
#define UNTITLED2_MODIFCONTACTDIALOG_H

#include <QDialog>
#include "../FicheContactDialog/FicheContactDialog.h"
#include "../Contact/QtFicheContact.h"

class ModifContactDialog : public FicheContactDialog
{
Q_OBJECT

public:
    explicit ModifContactDialog(QtFicheContact contact , QWidget *parent = nullptr);

    virtual void btAddClicked();

private:


};


#endif //UNTITLED2_MODIFCONTACTDIALOG_H
