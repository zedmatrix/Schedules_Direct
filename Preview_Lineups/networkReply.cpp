#include "mainwindow.h"
#include <QNetworkReply>

void MainWindow::onNetworkReply(QNetworkReply* reply) {
    if (!reply) {
        qDebug() << "Error: reply is null!";
        return;
    }
    if (reply->error() == QNetworkReply::NoError) {

        QUrl requestUrl = reply->request().url();
        qDebug() << QString("=> Request URL: %1").arg(requestUrl.toString());

        QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        qDebug() << "=> Status Code:" << statusCode.toInt();

        QString netText = QString("Request URL: %1 Status Code: %2").arg(requestUrl.toString()).arg(statusCode.toInt());
        ui->NetworkReplyLabel->setText(netText);

        QByteArray response = reply->readAll();
        QJsonParseError parseError;
        jsonResponse = QJsonDocument::fromJson(response, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            qDebug() << "JSON Parse Error: " << parseError.errorString();
            return;
        }

        QString urlPath = requestUrl.path();

        if (urlPath.contains("token")) {
            qInfo() << "=> token reply";
            //qDebug() << "=> Network Reply => Got Token: " << QString(response);
            if (jsonResponse.isObject()) {
                QJsonObject tokenObj = jsonResponse.object();

                parseToken(tokenObj);
                if (!token.isEmpty()) {
                    requestStatus();
                } else {
                    qWarning() << "Warning: token is empty.";
                }
            } else {
                qDebug() << "=> Error:" << reply->errorString();
            }

        } else if (urlPath.contains("status")) {
            jsonResponse = QJsonDocument::fromJson(response);

            if (jsonResponse.isObject()) {
                qInfo() << "=> Status Received ";
                QJsonObject statusObj = jsonResponse.object();
                StatusReceived(statusObj);

            } else {
                qWarning() << "=> Error: got something else";
            }


        } else if (urlPath.contains("schedules")) {
            QByteArray gunzippedData = decompressGzip(response);
            jsonResponse = QJsonDocument::fromJson(gunzippedData);



        } else if (urlPath.contains("programs")) {
            qDebug() << "=> Got Programs:";
            QByteArray gunzippedData = decompressGzip(response);
            jsonResponse = QJsonDocument::fromJson(gunzippedData);


        } else if (urlPath.contains("lineups")) {
            jsonResponse = QJsonDocument::fromJson(response);

            if (urlPath.contains("preview")) {
                //qInfo() << QString("=> Preview Received: %1 => %2").arg(urlPath).arg(response);

                if (jsonResponse.isObject()) {
                    qInfo() << "=> Lineups Received ";
                    QJsonObject lineupObj = jsonResponse.object();
                    parseObject(lineupObj);

                } else if (jsonResponse.isArray()) {
                    QJsonArray previewArray = jsonResponse.array();
                    qInfo() << "=> Preview Lineups Received:" << previewArray.size();
                    displayPreview(previewArray);

                } else {
                    qWarning() << "=> Error: got something else";
                }

            } else {
                qWarning() << "=> Error: got something else";
            }

        } else if (urlPath.contains("headends")) {

            if (jsonResponse.isArray()) {
                QJsonArray headendsArray = jsonResponse.array();
                qInfo() << "Available headends count: " << headendsArray.size();
                useColumnView(headendsArray);

            }

        } else {
            qInfo() << QString("=> Unexpected: %1 => %2").arg(urlPath).arg(response);

        }
    } else {
        qDebug() << "=> Error: " << reply->errorString();
    }

    // Clean Up for next call
    reply->deleteLater();
    reply = nullptr;
}
