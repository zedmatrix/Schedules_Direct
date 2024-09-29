#include "mainwindow.h"
void MainWindow::requestToken(const QString &username, const QString &password) {

    QJsonObject authPayload;
    QByteArray passwordHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha1).toHex();
    authPayload["username"] = username;
    authPayload["password"] = QString(passwordHash);

    QUrl url(QString(URLBASE) + "/20141201/token");
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    qDebug() << "=> Requesting URL: " << request.url();

    QByteArray requestData = QJsonDocument(authPayload).toJson();
    qDebug() << "=> Payload: " << QString(requestData);

    QNetworkReply *reply = manager->post(request, requestData);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        this->onNetworkReply(reply);
        reply->deleteLater();
    });
}

void MainWindow::parseToken(const QJsonObject &tokenObj) {
    QString message, datetime, serverID;
    statusMsg = {};
    statusError = {};

    if (!tokenObj.isEmpty()) {

        int code = tokenObj["code"].toInt();
        message = tokenObj["message"].toString();
        datetime = tokenObj["datetime"].toString();
        serverID = tokenObj["serverID"].toString();

        if (code == 0) {
            token = tokenObj["token"].toString();
            statusMsg = QString("Parsed: (%1) %2 @ %3").arg(QString::number(code)).arg(message).arg(datetime);
        } else {
            statusError = QString("Error: (%1) @ %2 => %3 => %4").arg(QString::number(code)).arg(datetime).arg(message).arg(serverID);
        }
        updateStatus();
        connect(ui->headends, &QPushButton::clicked, this, [this]() {
           requestHeadend() ;
        });
    } else {
        qWarning() << "Warning: Unexpected did not receive tokenObj.";
    }
}
