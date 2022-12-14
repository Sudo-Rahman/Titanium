//
// Created by Rahman  Yilmaz on 22/09/2022.
//

#include "MainWindow.h"
#include <QStatusBar>
#include "../ContactDialog/CreationContactDialog.h"

/**
 * @details Constructeur de la classe MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{

    setMinimumHeight(500);
    bd = new BD;
    historique = new ListHistorique;
    historique->loadData(qApp->applicationDirPath().toStdString()+"/log.txt");

    lstContact = BD::getContactData();
    lstContact->sort(StdListContact::DateDecroissant);

    menuBar = new MenuBar(lstContact, this);
    setMenuBar(menuBar);


    layoutGauche = new QHBoxLayout;
    layoutDroit = new QHBoxLayout;


    setWindowTitle("Titanium");
    auto *central = new QWidget;
    auto *layout = new QHBoxLayout(central);
    setCentralWidget(central);

    layout->addLayout(layoutGauche);
    layout->addLayout(layoutDroit);

    auto *status = new QStatusBar(this);


    nbContactLab = new QLabel("Nombre de contact : " + QString::number(lstContact->size()));
    nbContactLab->setAlignment(Qt::AlignHCenter);
    status->addWidget(nbContactLab, 1);
    nbInetractionLab = new QLabel("Nombre d'interaction : ");
    status->addWidget(nbInetractionLab, 1);
    nbInetractionLab->setAlignment(Qt::AlignHCenter);

    setStatusBar(status);

    toolBar = new ToolBar(lstContact, this);

    addToolBar(toolBar);

    lstContact->sort(StdListContact::DateDecroissant);
    listContactWidget = new ListContactWidget(lstContact, this);


    layoutGauche->addWidget(listContactWidget);

    allConnnect();

}

/**
 * @brief Fonction où est réalisé toutes les connections signaux slots.
 */
void MainWindow::allConnnect()
{

    connect(menuBar, &MenuBar::contactImported, this, [=, this]()
    {
        lstContact->sort(StdListContact::Sort::DateDecroissant);
        listContactWidget->recreateGroupeBoxContact();
        updateNbContact();
    });

    connect(toolBar, &ToolBar::clearHistoriqueClicked, this, &MainWindow::clearHistorique);
    connect(toolBar, &ToolBar::resetActionTriggered, this, &MainWindow::resetListContactWidget);
    connect(toolBar, &ToolBar::addContact, this, &MainWindow::addContact);
    connect(toolBar, &ToolBar::contactSought, this, &MainWindow::rechercheListContactWidget);
    connect(toolBar, &ToolBar::suppContact, this, [=, this](StdListContact *lst)
    {
        suppContacts(lst);
        lstContact->sort(StdListContact::Sort::DateDecroissant);
        listContactWidget->recreateGroupeBoxContact();
    });
    connect(toolBar, &ToolBar::sorted, this, [=, this](StdListContact::Sort sort)
    {
        lstContact->sort(sort);
        listContactWidget->recreateGroupeBoxContact();
    });


    connect(listContactWidget, &ListContactWidget::suppContact, this, &MainWindow::suppContact);
    connect(listContactWidget, &ListContactWidget::resetLastConctact, this, &MainWindow::removeListInteractionWidget);
    connect(listContactWidget, &ListContactWidget::interactionWidgetsHidedOrShowed, this, [=, this](bool visible)
    {
        if (!visible)
            setNbInteraction("");
        else
            setNbInteraction(QString::number(listInteractionWidget->getLstInteraction()->size()));
    });
    connect(listContactWidget, &ListContactWidget::contactSelected, this, [=, this](GroupeBoxContact *box)
    {
        setListInteractionWidget(box->getListInteractionWidget());
    });


}


/**
 * @brief Setter de lstContact qui est la liste de contact utilisé qui stock les contacts de recherche ou tous les contacts.
 * @return lstContact
 */
StdListContact *MainWindow::getLstContact() const
{
    return lstContact;
}

/**
 * @brief Getter de la liste des historiques.
 * @return
 */
ListHistorique *MainWindow::getHistorique() const
{
    return historique;
}


/**
 * @brief Surcharge de l’événement de fermeture.
 * @param event
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    QWidget::closeEvent(event);
    delete lstContact;
    delete bd;
    historique->saveData(qApp->applicationDirPath().toStdString()+"/log.txt");
    delete historique;
}

/**
 * @details Ajouter un StdContact dans la liste des contacts et dans le Widget des contacts pour l'afficher.
 * @param contact
 */
void MainWindow::addContact(StdContact *contact)
{
    lstContact->addContact(contact);
    listContactWidget->addContactBox(contact);
    BD::addFullContactAttributesOnBD(*contact);
    historique->addLog(ListHistorique::AjoutContact, *contact);
    updateNbContact();
}

/**
 * @details Supprime un contact.
 * @param contact
 */
void MainWindow::suppContact(StdContact *contact)
{

    BD::supContact(*contact);
    historique->addLog(ListHistorique::SuppressionContact, *contact);
    lstContact->supContact(contact);
    updateNbContact();
}

/**
 * @details Supprime une liste de contact.
 * @param lst
 */
void MainWindow::suppContacts(StdListContact *lst)
{
    QString str;
    for (auto contact: *lst->getLstContact())
    {
        str += QString::fromStdString(contact->getNom() + " - " + contact->getPrenom()) + "\n";
        suppContact(contact);
    }
    QMessageBox msgb(QMessageBox::Icon::Information, "Succès", "Suppression réalisée avec succès");
    msgb.setDetailedText(str);
    msgb.exec();
}


/**
 * @details Fonction utilisée lors d'une recherche de contacts, qui cache les contacts qui ne correspondent à la recherche.
 * @param lst
 */
void MainWindow::rechercheListContactWidget(StdListContact *lst)
{
    listContactWidget->cacheGroupeBox(lst);

    nbContactLab->setText("Nombre de contact : " + QString::number(listContactWidget->getNbGroupeBoxVisible()));

    removeListInteractionWidget();

    setNbInteraction("");

}

/**
 * @details setter du widget des interactions.
 * @param widget
 */
void MainWindow::setListInteractionWidget(ListInteractionWidget *widget)
{
    removeListInteractionWidget();

    layoutDroit->addWidget(widget);
    listInteractionWidget = widget;


    connect(listInteractionWidget, &ListInteractionWidget::updateNbInteraction, this, &MainWindow::setNbInteraction);
    setNbInteraction(QString::number(listInteractionWidget->getLstInteraction()->size()));
}

/**
 * @details Remet à 0 le Widget qui affiche les contacts, c'est-à-dire supprime l'ancien listContactWidget,
 * recrée une nouvelle listContactWidget avec les contacts dans lstContact, enlève aussi le Widget qui affiche les interactions d'un contact s'il est affiché.
 */
void MainWindow::resetListContactWidget()
{
    lstContact->sort(StdListContact::DateDecroissant);
    reactualise();
}

/**
 * @brief Fonction qui met à jour la valeur de nbContactLab qui indique le nombre de contacts visible.
 */
void MainWindow::updateNbContact()
{
    nbContactLab->setText("Nombre de contact : " + QString::number(lstContact->size()));
}

/**
 * @brief Fonction qui met à jour la valeur de nbInetractionLab qui indique le nombre de d'interaction du contact sélectionné.
 */
void MainWindow::setNbInteraction(const QString &number)
{
    nbInetractionLab->setText("Nombre d'interaction : " + number);
}


/**
 * @details Réactualise l'UI, c’est-à-dire enlève le widget des interactions et affiche tous les contacts.
 */
void MainWindow::reactualise()
{
    removeListInteractionWidget();

    listContactWidget->afficheAllGroupeBox();

    setNbInteraction("");
    updateNbContact();
}

/**
 * @brief Cache le widget des interactions.
 */
void MainWindow::removeListInteractionWidget()
{
    if (listInteractionWidget)
    {
        listInteractionWidget->hide();
        setNbInteraction("");
    }
}

void MainWindow::clearHistorique()
{
    historique->clear();
}
