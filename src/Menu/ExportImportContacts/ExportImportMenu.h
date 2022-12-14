//
// Created by rahman on 30/10/22.
//

#ifndef PROJET_QT_EXPORTIMPORTMENU_H
#define PROJET_QT_EXPORTIMPORTMENU_H

#include <QMenu>
#include <QAction>
#include "../../Contact/StdListContact.h"

class ExportImportMenu : public QMenu
{
Q_OBJECT
public:
    explicit ExportImportMenu(StdListContact *lstContact, QWidget *parent);

private:
    QAction *exportAction{};
    QAction *importAction{};
    StdListContact *lstContact{};

private slots:

    void exportActionTriggered();

    void importActionTriggered();

signals:

    void contactImported();

};

#endif //PROJET_QT_EXPORTIMPORTMENU_H
