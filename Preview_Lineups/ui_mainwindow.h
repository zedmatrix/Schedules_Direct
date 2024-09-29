/********************************************************************************
** Form generated from reading UI file 'mainwindowNZTGHS.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MAINWINDOWNZTGHS_H
#define MAINWINDOWNZTGHS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QColumnView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTableWidget *guideTable;
    QPushButton *quit;
    QTableWidget *listingTable;
    QPushButton *status;
    QPushButton *lineups;
    QPushButton *schedules;
    QPushButton *programs;
    QColumnView *columnView;
    QPushButton *headends;
    QListWidget *listWidget;
    QLabel *NetworkReplyLabel;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 705);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        guideTable = new QTableWidget(centralwidget);
        guideTable->setObjectName("guideTable");
        guideTable->setGeometry(QRect(10, 10, 681, 141));
        quit = new QPushButton(centralwidget);
        quit->setObjectName("quit");
        quit->setGeometry(QRect(700, 510, 82, 28));
        QFont font;
        font.setBold(true);
        quit->setFont(font);
        listingTable = new QTableWidget(centralwidget);
        listingTable->setObjectName("listingTable");
        listingTable->setGeometry(QRect(10, 310, 681, 161));
        status = new QPushButton(centralwidget);
        status->setObjectName("status");
        status->setGeometry(QRect(700, 10, 91, 28));
        status->setFont(font);
        lineups = new QPushButton(centralwidget);
        lineups->setObjectName("lineups");
        lineups->setGeometry(QRect(700, 50, 91, 28));
        lineups->setFont(font);
        schedules = new QPushButton(centralwidget);
        schedules->setObjectName("schedules");
        schedules->setGeometry(QRect(700, 90, 91, 28));
        schedules->setFont(font);
        programs = new QPushButton(centralwidget);
        programs->setObjectName("programs");
        programs->setGeometry(QRect(700, 130, 91, 28));
        programs->setFont(font);
        columnView = new QColumnView(centralwidget);
        columnView->setObjectName("columnView");
        columnView->setGeometry(QRect(10, 480, 681, 151));
        headends = new QPushButton(centralwidget);
        headends->setObjectName("headends");
        headends->setGeometry(QRect(700, 170, 91, 28));
        headends->setFont(font);
        listWidget = new QListWidget(centralwidget);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(10, 160, 681, 141));
        NetworkReplyLabel = new QLabel(centralwidget);
        NetworkReplyLabel->setObjectName("NetworkReplyLabel");
        NetworkReplyLabel->setGeometry(QRect(10, 650, 661, 21));
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        quit->setText(QCoreApplication::translate("MainWindow", "Quit", nullptr));
        status->setText(QCoreApplication::translate("MainWindow", "Status", nullptr));
        lineups->setText(QCoreApplication::translate("MainWindow", "Lineups", nullptr));
        schedules->setText(QCoreApplication::translate("MainWindow", "Schedules", nullptr));
        programs->setText(QCoreApplication::translate("MainWindow", "Programs", nullptr));
        headends->setText(QCoreApplication::translate("MainWindow", "Head Ends", nullptr));
        NetworkReplyLabel->setText(QCoreApplication::translate("MainWindow", "Network Reply Label", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MAINWINDOWNZTGHS_H
