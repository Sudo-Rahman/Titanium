//
// Created by rahman on 01/11/22.
//

#include "ToolBar.h"
#include "../ContactDialog/CreationContactDialog.h"
#include "RechercheContact/RechercheContactDialog.h"
#include "RechercheTache/TreeTacheDialog.h"
#include "Suppression/SuppressionDialog.h"
#include <QTextEdit>
#include <QLayout>
#include <QListWidget>
#include <QApplication>


/**
 * @brief Constructeur par défaut de la classe ToolBar.
 * @param parent
 */
ToolBar::ToolBar(StdListContact *lstContact, QWidget *parent) : lstContact(lstContact), QToolBar(parent)
{

    setContextMenuPolicy(Qt::ActionsContextMenu);

    ajouter = new QAction("Ajouter un contact", this);
    ajouter->setIcon(QIcon(":/images/ajouter.png"));
    ajouter->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_A));
    addAction(ajouter);

    connect(ajouter, &QAction::triggered, this, &ToolBar::ajouterContact);

    addSeparator();

    createFindBtn();

    addSeparator();

    createTriBtn();

    addSeparator();

    resetListContactsWidget = new QAction("Afficher tous les contacts", this);
    resetListContactsWidget->setIcon(QIcon(":/images/reset.png"));
    addAction(resetListContactsWidget);

    //Remet à zero le Widget des contacts dans la mainWindow.
    connect(resetListContactsWidget, &QAction::triggered, this, [=, this]()
    {
        emit resetActionTriggered();
    });

    addSeparator();

    historique = new QAction("Historique", this);
    historique->setIcon(QIcon(":/images/historique.png"));

    connect(historique, &QAction::triggered, this, &ToolBar::afficheHistorique);

    addAction(historique);

    addSeparator();

    suppression = new QAction(QIcon(":/images/corbeille.png"), "Suppression", this);

    connect(suppression, &QAction::triggered, this, &ToolBar::supprimer);

    addAction(suppression);


}

/**
 * @brief Ouvre une fenêtre de dialog pour ajouter un contact.
 */
void ToolBar::ajouterContact()
{
    CreationContactDialog dialog(this);
    connect(&dialog, &CreationContactDialog::btnActionClicked, this, [this](StdContact *contact)
    {
        emit addContact(contact);
    });
    dialog.exec();
}

/**
 * @brief Ouvre une fenêtre de dialog pour la recherche de contact.
 */
void ToolBar::chercherContact()
{

    resetActionTriggered();
    auto *dialog = new RechercheContactDialog(lstContact, this);
    chercher->setDisabled(true);
    tri->setDisabled(true);
    connect(dialog, &RechercheContactDialog::closeDialog, this, [this]()
    {
        chercher->setDisabled(false);
        tri->setDisabled(false);
        resetActionTriggered();
    });

    connect(dialog, &RechercheContactDialog::contactSought, this, [this](StdListContact *lst)
    { emit contactSought(lst); });
    dialog->show();

}


/**
 * @brief Creation du menu pour le tri.
 */
void ToolBar::createTriBtn()
{

    tri = new QToolButton(this);

    tri->setPopupMode(QToolButton::MenuButtonPopup);

    auto *menu = new QMenu(tri);

    auto *triNomC = new QAction("Tri nom croissant", menu);
    triNomC->setIcon(QIcon(":/images/tri.png"));

    auto *triNomD = new QAction("Tri nom décroissant", menu);
    triNomD->setIcon(QIcon(":/images/tri.png"));

    auto *triDateC = new QAction("Tri date croissant", menu);
    triDateC->setIcon(QIcon(":/images/tri.png"));

    auto *triDateD = new QAction("Tri date décroissant", menu);
    triDateD->setIcon(QIcon(":/images/tri.png"));


    menu->addAction(triNomC);
    menu->addAction(triNomD);
    menu->addAction(triDateC);
    menu->addAction(triDateD);

    tri->setMenu(menu);

    tri->setDefaultAction(triDateD);

    addWidget(tri);


    // tri par nom (ordre croissant).
    connect(triNomC, &QAction::triggered, this, [this]()
    {
        emit sorted(StdListContact::NomCroissant);
    });
    // tri par nom (ordre décroissant).
    connect(triNomD, &QAction::triggered, this, [this]()
    {
        emit sorted(StdListContact::NomDecroissant);
    });


    // tri par date de creation le plus recent crée.
    connect(triDateD, &QAction::triggered, this, [this]()
    {
        emit sorted(StdListContact::DateDecroissant);
    });
    // tri par date de creation le plus ancien crée.
    connect(triDateC, &QAction::triggered, this, [this]()
    {
        emit sorted(StdListContact::DateCroissant);
    });
}

/**
 * @details Click sur l'action historique, qui affiche une fenêtre qui contient tous les historiques.
 */
void ToolBar::afficheHistorique()
{


    QDialog dialog;
    dialog.setWindowTitle("Historique");

    QGridLayout layout(&dialog);

    QListWidget wid(&dialog);
    wid.setAlternatingRowColors(true);

    for (const auto &str: *qobject_cast<MainWindow *>(Utility::getMainWindow(this))->getHistorique())
        wid.addItem(QString::fromStdString(str).replace("\n", " | "));


    QPushButton effaceHistorique("Effacer l’historique", &dialog);

    connect(&effaceHistorique, &QPushButton::clicked, this, [&, this]()
    {
        emit clearHistoriqueClicked();
        wid.clear();
    });

    layout.addWidget(&wid);

    layout.addWidget(&effaceHistorique);

    dialog.exec();

}

/**
 * @brief Creation du menu pour la recherche.
 */
void ToolBar::createFindBtn()
{
    chercher = new QToolButton(this);
    chercher->setIcon(QIcon(":/images/chercher.png"));
    addWidget(chercher);

    chercher->setPopupMode(QToolButton::MenuButtonPopup);

    auto *findContact = new QAction("Chercher contact", this);
    findContact->setIcon(QIcon(":/images/contact.png"));
    connect(findContact, &QAction::triggered, this, &ToolBar::chercherContact);

    chercher->addAction(findContact);

    auto *findTache = new QAction("Chercher taches", this);
    findTache->setIcon(QIcon(":/images/todo.png"));
    connect(findTache, &QAction::triggered, this, &ToolBar::chercherTache);

    chercher->addAction(findTache);

}

/**
 * @brief Click sur le boutton de recherche d'une tache.
 */
void ToolBar::chercherTache()
{
    TreeTacheDialog diag(lstContact);

    diag.exec();
}


/**
 * @brief Click sur le boutton suppression.
 */
void ToolBar::supprimer()
{
    SuppressionDialog dialog(lstContact, this);
    connect(&dialog, &SuppressionDialog::contactSupprimer, this, [=, this](StdListContact *lst)
    {
        emit suppContact(lst);
    });
    dialog.exec();

}
