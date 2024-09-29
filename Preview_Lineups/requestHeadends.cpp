#include "mainwindow.h"
void MainWindow::displayPreview(const QJsonArray &previewArray) {
    ui->listingTable->clear();
    ui->listingTable->setColumnCount(4);
    QStringList headers;
    headers << "Channel ID" << "Channel Name" << "Call Sign" << "Affiliate";
    ui->listingTable->setHorizontalHeaderLabels(headers);
    ui->listingTable->setRowCount(previewArray.size());

    for (int i = 0; i < previewArray.size(); i++) {
        QString channelName, channelID, callsign, affiliate;

        if (previewArray[i].isObject()) {
            QJsonObject previewObj = previewArray[i].toObject();

            if (!previewObj.isEmpty()) {

                channelName = previewObj["name"].toString();
                channelID = previewObj["channel"].toString();
                callsign = previewObj["callsign"].toString();
                affiliate = previewObj["affiliate"].toString();

                ui->listingTable->setItem(i, 0, new QTableWidgetItem(channelID));
                ui->listingTable->setItem(i, 1, new QTableWidgetItem(channelName));
                ui->listingTable->setItem(i, 2, new QTableWidgetItem(callsign));
                ui->listingTable->setItem(i, 3, new QTableWidgetItem(affiliate));
            } else {
                qInfo() << QString("item: %1 is empty").arg(i);
            }

        }
    }

}

void MainWindow::HeadendDoubleClicked(const QModelIndex &index) {

    QString itemText = index.data().toString();
    qInfo() << "==> Item double-clicked:" << itemText;
    int uriIndex = itemText.indexOf("URI:");
    if (uriIndex != -1) {
        QString uriText = itemText.mid(uriIndex + 4).trimmed();
        qInfo() << "==> Lineup url:" << uriText;

        int lineupIndex = uriText.indexOf("/lineups");
        if (lineupIndex != -1) {

            QString previewURL = uriText;

            previewURL.insert(lineupIndex + 8, "/preview");
            QUrl url(QString(URLBASE) + previewURL);
            if (url.isValid() && !url.host().isEmpty()) {
                qInfo() << "Valid URL:" << url.toString() << token;
                QNetworkRequest request(url);
                request.setRawHeader("token", token.toUtf8());
                request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
                QNetworkReply *reply = manager->get(request);

                connect(reply, &QNetworkReply::finished, [reply, this]() {
                    this->onNetworkReply(reply);
                    reply->deleteLater();
                });
            } else {
                qWarning() << "=> Error: Invalid URL." << url.toString();
            }
        }
    }
}

void MainWindow::useColumnView(QJsonArray headendsArray) {

    QString headID, headTransport, headLocation;
    QString lineID, lineName, lineUri;

    QStandardItemModel *model = new QStandardItemModel(this);
    QStandardItem *headendItem = new QStandardItem(QString("Headend"));

    for (const QJsonValue &headendValue : headendsArray) {

        if (headendValue.isObject()) {
            QJsonObject headendObj = headendValue.toObject();

            headID = QString("Headend ID: %1").arg(headendObj["headend"].toString());
            headTransport = QString("Transport: %1").arg(headendObj["transport"].toString());
            headLocation = QString("Location: %1").arg(headendObj["location"].toString());

            // first column (Headend ID)
            QStandardItem *headendIDItem = new QStandardItem(headID);

            // second column under headend ID
            QStandardItem *headendTransportItem = new QStandardItem(headTransport);
            QStandardItem *headendLocationItem = new QStandardItem(headLocation);
            headendIDItem->appendRow({headendTransportItem, headendLocationItem});

            model->appendRow(headendIDItem);
            if (headendObj.contains("lineups") && headendObj["lineups"].isArray()) {
                QJsonArray lineupsArray = headendObj["lineups"].toArray();

                for (const QJsonValue &lineupValue : lineupsArray) {
                    if (lineupValue.isObject()) {
                        QJsonObject lineupObj = lineupValue.toObject();
                        lineName = QString("Lineup Name: %1").arg(lineupObj["name"].toString());
                        lineID = QString("Lineup ID: %1").arg(lineupObj["lineup"].toString());
                        lineUri = QString("Lineup URI: %1").arg(lineupObj["uri"].toString());

                        QStandardItem *lineupItem = new QStandardItem(lineName);
                        QStandardItem *lineupIDItem = new QStandardItem(lineID);
                        QStandardItem *lineupURIItem = new QStandardItem(lineUri);

                        lineupItem->appendRow({lineupIDItem});          // add ID(3) to Name 2nd column
                        lineupIDItem->appendRow({lineupURIItem});       // add URI(4) to ID 3rd columm
                        headendIDItem->appendRow(lineupItem);           // add Items to headend ID(1) 1st column
                    }
                }
            }
            ui->columnView->setModel(model);

        }
    }
    connect(ui->columnView, &QColumnView::doubleClicked, this, &MainWindow::HeadendDoubleClicked);

}

void MainWindow::requestHeadend() {

    QUrl url(QString(URLBASE) + "/20141201/headends");
    QUrlQuery query;

    query.addQueryItem("country", country);
    query.addQueryItem("postalcode", postalcode);
    url.setQuery(query);

    if (url.isValid() && !url.host().isEmpty()) {
        qInfo() << "=> Request Headends:" << url.toString() << token;
        QNetworkRequest request(url);
        request.setRawHeader("token", token.toUtf8());
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        QNetworkReply *reply = manager->get(request);

        connect(reply, &QNetworkReply::finished, [reply, this]() {
            this->onNetworkReply(reply);
            reply->deleteLater();
        });

    } else {
        qWarning() << "Invalid URL:" << url.toString();

    }
}
