// void MainWindow::onColumnItemSelected(const QModelIndex &index) {
//     QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->columnView->model());
//
//     if (model) {
//         QString selectedItemText = model->itemFromIndex(index)->text();
//         qInfo() << "Selected item:" << selectedItemText;
//     }
// }

void MainWindow::onColumnItemSelected(const QModelIndex &index) {
    QStandardItemModel *model = qobject_cast<QStandardItemModel *>(ui->columnView->model());

    if (model) {
        QStandardItem *selectedItem = model->itemFromIndex(index);

        // Check if this is the last column by looking at the column index
        int column = index.column();

        if (selectedItem && column == model->columnCount() - 1) {
            QString selectedItemText = selectedItem->text();
            qInfo() << "Last column item selected:" << selectedItemText;
        } else {
            qInfo() << "Item selected in column:" << column;
        }
    }
}

void MainWindow::useColumnView(QJsonObject &headendObj, QStandardItemModel *model, QStandardItem *headendItem) {

    QString headID = QString("Headend ID: %1").arg(headendObj["headend"].toString());
    QString headTransport = QString("Transport: %1").arg(headendObj["transport"].toString());
    QString headLocation = QString("Location: %1").arg(headendObj["location"].toString());

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
                QString lineName = QString("Lineup Name: %1").arg(lineupObj["name"].toString());
                QString lineID = QString("Lineup ID: %1").arg(lineupObj["lineup"].toString());
                QString lineUri = QString("Lineup URI: %1").arg(lineupObj["uri"].toString());

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
