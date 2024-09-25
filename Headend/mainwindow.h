#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QStatusBar>
#include <QString>
#include <QWidget>
#include <QByteArray>
#include <QStandardItem>
#include <QColumnView>
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:

protected:

private slots:

private:
    Ui::MainWindow *ui;
    void parseObject (const QJsonObject& obj);
    void getToken(const QString &username, const QString &password);
    void fetchHeadends(const QString &token);
    void displayHeadends(QJsonArray &headendsArray);
    void useColumnView(QJsonObject &headendObj, QStandardItemModel *model, QStandardItem *headendItem);
    void onColumnItemSelected(const QModelIndex &index);
    void updateStatus();
    QString username, password, country, postalcode; //defaults for global use
    QString token, baseUrl, statusMsg, outText;
    QNetworkAccessManager *manager;

};

#endif // MAINWINDOW_H
