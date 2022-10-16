//
// Created by Rahman  Yilmaz on 06/10/2022.
//

#ifndef UNTITLED2_CONTACTDIALOG_H
#define UNTITLED2_CONTACTDIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QDateTime>
#include "../Contact/TraductionQtStd.h"

class ContactDialog : public QDialog
{
Q_OBJECT
public:
    explicit ContactDialog(QWidget *parent = nullptr);

protected:
    QVBoxLayout *layout{};
    QHBoxLayout *layoutTop{};
    QVBoxLayout *layoutCenter{};
    QLabel *labIm{};
    QLabel *labDateCreation{};
    QLineEdit *line1{};
    QLineEdit *line2{};
    QLineEdit *line3{};
    QLineEdit *line4{};
    QLineEdit *line5{};
    QLineEdit *line6{};
    QPushButton *bt6{};
    QPushButton *btAction{};

public slots:

    void bt6Clicked();

    virtual void btActionClicked();

    QtContact getContact(uint64_t time);
};


#endif //UNTITLED2_CONTACTDIALOG_H
