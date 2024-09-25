#include <QDebug>
#include "mainwindow.h"
#include "fetch_Headend.cpp"
#include "columnView.cpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    manager(new QNetworkAccessManager(this))
{
    //
    ui->setupUi(this);

    connect(ui->quit, &QPushButton::pressed, this, []() { qApp->quit(); });
    connect(ui->columnView, &QColumnView::clicked, this, &MainWindow::onColumnItemSelected);

    baseUrl = "https://json.schedulesdirect.org/";
    username = /* Your Schedules Direct Username */
    password = /* Your Schedules Direct Password */
    country =  /* USA / CAN */
    postalcode = /* 96718 / T9H 1T0 */

    getToken(username, password);

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::updateStatus() {
    ui->statusbar->showMessage(statusMsg);
}

void MainWindow::getToken(const QString &username, const QString &password) {

    QJsonObject authPayload;
    QByteArray passwordHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha1).toHex();
    authPayload["username"] = username;
    authPayload["password"] = QString(passwordHash);

    baseUrl = "https://json.schedulesdirect.org/";
    QUrl url(QString(baseUrl) + "/20141201/token");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    qDebug() << "=> Requesting URL: " << request.url();

    QByteArray requestData = QJsonDocument(authPayload).toJson();
    qDebug() << "=> Payload: " << QString(requestData);

    QNetworkReply *reply = manager->post(request, requestData);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QByteArray response = reply->readAll();
        qDebug() << "=> Network Reply => Got Token: " << QString(response);
        QJsonDocument jsonResponse = QJsonDocument::fromJson(response);
        if (jsonResponse.isObject()) {
            QJsonObject tokenObj = jsonResponse.object();
            int code = tokenObj.value("code").toDouble();
            QString message = tokenObj.value("message").toString();
            QString datetime = tokenObj.value("datetime").toString();

            if (code == 0) {
                token = jsonResponse.object().value("token").toString();
                statusMsg = QString("Token Received: %1 @ %2 => %3").arg(token).arg(datetime).arg(message);
                if (!token.isEmpty()) fetchHeadends(token);

            } else {
                statusMsg = QString("Error: (%1) => %2 => %3").arg(QString::number(code)).arg(datetime).arg(message);
                qDebug() << "=> Error";
            }

        } else {
            qDebug() << "=> Error:" << reply->errorString();
        }
        updateStatus();
    });
}
