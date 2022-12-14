//
// Created by rahman on 08/10/22.
//

#include "CreationContactDialog.h"

/**
 * @details Constructeur de la classe CreationContactDialog
 * @param parent
 */
CreationContactDialog::CreationContactDialog(QWidget *parent) : ContactDialog(parent)
{
    setWindowTitle("Création de la fiche contact");


    auto *timer = new QTimer(this);
    timer->setInterval(100);

    connect(timer, &QTimer::timeout, this, [this]()
    {
        QDateTime date;
        date.setMSecsSinceEpoch(time(nullptr) * 1000);
        QLocale local(QLocale::Language::French);
        labDateCreation->setText(local.toString(date, "dddd, d MMMM yyyy hh:mm:ss"));
    });
    timer->start();


    btAction->setText("Ajouter");
    btAction->setShortcut(Qt::Key_Enter);

}

/**
 * @details Clic du bouton pour ajouter un contact
 */
void CreationContactDialog::btActionClicked()
{
    QString mess("Des champs sont vides !!\n");
    bool pass = true;
    int autreQueLettreOuNombre = 0;
    for (auto *line: findChildren<QLineEdit *>())
    {
        if (line->text().isEmpty())
        {
            pass = false;
            break;
        } else
        {
            if (line->objectName() == "tel")
            {
                for (auto c: line->text())
                {
                    if (!c.isDigit())
                    {
                        mess += "Le Telephone doit contenir uniquement des chiffres !!";
                        pass = false;
                        break;
                    }
                }
            } else
            {
                for (auto c: line->text())
                {
                    if (!c.isDigit() && !c.isLetter())
                        autreQueLettreOuNombre++;
                }
            }
        }

        if (autreQueLettreOuNombre == line->text().size() && !line->text().isEmpty())
            pass = false;
        autreQueLettreOuNombre = 0;
    }
    if (!pass)
    {
        QMessageBox::critical(this, "Erreur", mess);
    } else
    {
        auto *mainWindow = new QWidget(this);
        while (mainWindow->parentWidget())
        {
            mainWindow = mainWindow->parentWidget();
            if (strcmp(mainWindow->metaObject()->className(), "MainWindow") == 0)
                break;
        }
        QtContact qtContact(getContact());

        emit btnActionClicked(new StdContact(Utility::QtFicheContactToStdFicheContact(qtContact)));

        int rep = QMessageBox::information(this, "Information", "Le contact à été ajouté avec succès.");
        if (rep == QMessageBox::Ok)
            close();
    }
}
