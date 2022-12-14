//
// Created by Rahman  Yilmaz on 22/09/2022.
//

#ifndef PROJET_QT_MAINWINDOW_H
#define PROJET_QT_MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QWidget>
#include <QLabel>
#include "../Contact/StdListContact.h"
#include "../BaseDeDonnees/BD.h"
#include "../Contact/Widget/ListContactWidget.h"
#include "../Historique/ListHistorique.h"
#include "../ToolBar/ToolBar.h"
#include "../Menu/MenuBar.h"


class MainWindow : public QMainWindow
{
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QHBoxLayout *layoutGauche{};
    QHBoxLayout *layoutDroit{};

    ListContactWidget *listContactWidget{};
    ListInteractionWidget *listInteractionWidget{};

    BD *bd{};
    StdListContact *lstContact{};

    ListHistorique *historique{};

    ToolBar *toolBar{};
    MenuBar *menuBar{};

public:
    ListHistorique *getHistorique() const;

private:

    QLabel *nbContactLab{};
    QLabel *nbInetractionLab{};

    void allConnnect();

public:

    [[nodiscard]] StdListContact *getLstContact() const;

    [[nodiscard]] StdListContact *getLstContactTmp() const;

    void addContact(StdContact *);

    void suppContact(StdContact *);

    void suppContacts(StdListContact *);

    void updateNbContact();

    void setNbInteraction(const QString &);

    void rechercheListContactWidget(StdListContact *);

    void setListInteractionWidget(ListInteractionWidget *);

    void reactualise();

    void removeListInteractionWidget();


protected:
    void closeEvent(QCloseEvent *event) override;

public slots:

    void clearHistorique();

    void resetListContactWidget();

};


#endif //PROJET_QT_MAINWINDOW_H
