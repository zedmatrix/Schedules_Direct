void MainWindow::parseObject (const QJsonObject& obj) {
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        QString key = it.key();
        QJsonValue value = it.value();

        QString size, type;
        switch (value.type()) {
            case QJsonValue::Null:    type = "Null"; break;
            case QJsonValue::Bool:    type = "Bool"; break;
            case QJsonValue::Double:  type = "Double"; break;
            case QJsonValue::String: {
                type = "String";
                size = value.toString();
                break;
            }
            case QJsonValue::Array: {
                type = "Array";
                QJsonArray arr = value.toArray();
                size = QString::number(arr.size());
                break;
            }
            case QJsonValue::Object:  {
                type = "Object";
                QJsonObject vObj = value.toObject();
                size = vObj.keys().join(',');
                break;
            }
            case QJsonValue::Undefined: type = "Undefined"; break;
            default: type = "Unknown"; break;
        }
        QString outText = QString("Key: %1 => [%2] => Type: %3").arg(key).arg(size).arg(type);
        qInfo() << outText;
        //ui->listWidget->addItem(outText);
    }
}

void MainWindow::displayHeadends(QJsonArray &headendsArray) {
    QString headID, headTransport, headLocation;
    QString lineID, lineName, lineUri;

    QStandardItemModel *model = new QStandardItemModel(this);
    QStandardItem *headendItem = new QStandardItem(QString("Headend"));
    for (const QJsonValue &headendValue : headendsArray) {

        if (headendValue.isObject()) {
            QJsonObject headendObj = headendValue.toObject();
            //parseObject(headendObj);
            useColumnView(headendObj, model, headendItem);

            headID = QString("Headend ID: %1\n").arg(headendObj["headend"].toString());
            headTransport = QString("Transport: %1\n").arg(headendObj["transport"].toString());
            headLocation = QString("Location: %1\n").arg(headendObj["location"].toString());
            outText = headID + headTransport + headLocation;
            ui->listWidget->addItem(outText);

            if (headendObj.contains("lineups") && headendObj["lineups"].isArray()) {
                QJsonArray lineupsArray = headendObj["lineups"].toArray();
                for (const QJsonValue &lineupValue : lineupsArray) {
                    if (lineupValue.isObject()) {
                        QJsonObject lineupObj = lineupValue.toObject();
                        lineName = QString("Lineup Name: %1\n").arg(lineupObj["name"].toString());
                        lineID = QString("Lineup ID: %1\n").arg(lineupObj["lineup"].toString());
                        lineUri = QString("Lineup URI: %1\n").arg(lineupObj["uri"].toString());
                        outText = lineName + lineID + lineUri;
                        ui->listWidget->addItem(outText);
                    }
                }
            }
        }
        ui->listWidget->addItem("--\n");
    }
}

void MainWindow::fetchHeadends(const QString &token) {
    if (baseUrl.isEmpty()) {
        qDebug() << "=> Error => empty baseUrl.";
    }

    QUrl url(baseUrl +"20141201/headends");
    QUrlQuery query;

    query.addQueryItem("country", country);
    query.addQueryItem("postalcode", postalcode);
    url.setQuery(query);

    if (url.isValid() && !url.host().isEmpty()) {
        qInfo() << "Valid URL:" << url.toString() << token;
        QNetworkRequest request(url);
        request.setRawHeader("token", token.toUtf8());
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        QNetworkReply *reply = manager->get(request);

        connect(reply, &QNetworkReply::finished, [reply, this]() {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray responseData = reply->readAll();
                //qInfo() << "Raw Headends Response: " << QString(responseData);

                QJsonParseError parseError;
                QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData, &parseError);

                if (parseError.error != QJsonParseError::NoError) {
                    qDebug() << "JSON Parse Error: " << parseError.errorString();
                    return;
                }

                if (jsonResponse.isArray()) {
                    QJsonArray headendsArray = jsonResponse.array();
                    qInfo() << "Available headends count: " << headendsArray.size();
                    displayHeadends(headendsArray);
                } else {
                    qDebug() << "Error: Expected a JSON array but got something else.";
                }
            } else {
                qDebug() << "Error fetching headends: " << reply->errorString();
            }
            reply->deleteLater();
        });

        // End of connect lambda
    } else {
        qInfo() << "Invalid URL" << url.toString();

    }
}
