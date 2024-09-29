#include "mainwindow.h"
void MainWindow::requestStatus() {

    QUrl url(QString(URLBASE) + "/20141201/status");
    QNetworkRequest request(url);

    request.setRawHeader("token", token.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    qDebug() << "=> Sending schedules request to URL: " << request.url().toString();

    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        this->onNetworkReply(reply);
        reply->deleteLater();
    });
}

void MainWindow::StatusReceived(const QJsonObject &statusObj) {

    if (!lineup) {
        lineup = new Lineup();
    }
    QJsonObject accountObj = statusObj["account"].toObject();
    QString expires = accountObj["expires"].toString();
    int maxLineups = accountObj["maxLineups"].toInt();
    //QJsonArray messages = accountObj["messages"].toArray(); //size 0

    QString currentText = QString("Account Info: {%1} expires: %2 ").arg(QString::number(maxLineups)).arg(expires);
    ui->listWidget->addItem(currentText);

    int code = statusObj["code"].toInt();
    QString datetime = statusObj["datetime"].toString();
    QString lastUpdate = statusObj["lastDataUpdate"].toString();
    currentText = QString("Date: %1 Last Update: %2").arg(datetime).arg(lastUpdate);
    ui->listWidget->addItem(currentText);

    QJsonArray lineupsArray = statusObj["lineups"].toArray();
    QJsonObject lineupsObj = lineupsArray[0].toObject();

    //parseObject(lineupsObj);
    lineup->name = lineupsObj["name"].toString();
    lineup->modified = lineupsObj["modified"].toString();
    lineup->lineup = lineupsObj["lineup"].toString();
    lineup->uri = lineupsObj["uri"].toString();

    currentText = QString("Current Line Up: [%1] Added: {%2}\n (%3) -> %4")
        .arg(lineup->name).arg(lineup->modified).arg(lineup->lineup).arg(lineup->uri);
    ui->listWidget->addItem(currentText);

    QString serverID = statusObj["serverID"].toString();

    QJsonArray arr = statusObj["systemStatus"].toArray();
    QJsonObject systemStatusObj = arr[0].toObject();
    QString date = systemStatusObj["date"].toString();
    QString message = systemStatusObj["message"].toString();
    QString status = systemStatusObj["status"].toString();

    statusMsg = QString("Server: (%1) %2 [%3 %4] ").arg(QString::number(code))
        .arg(serverID).arg(status).arg(message);

    updateStatus();


}
