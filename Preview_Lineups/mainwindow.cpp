#include <QDebug>
#include "mainwindow.h"

QMap<int, Station> stationMap;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    manager(new QNetworkAccessManager(this))
{
    //
    ui->setupUi(this);

    setTitle("Initializing");
    InitReadFile("settings.json");

    connect(ui->quit, &QPushButton::pressed, this, []() { qApp->quit(); });

    qInfo() << QString("username: {%1} | password: {%2}").arg(username).arg(password);
    requestToken(username, password);

}

MainWindow::~MainWindow() {
    if (lineup) {
        delete lineup;
        lineup = nullptr;
    }

    delete ui;
}

void MainWindow::updateStatus() {
    QString msg = QString("| %1 | %2 | %3 |").arg(token).arg(statusMsg).arg(statusError);
    ui->statusbar->showMessage(msg);
}
void MainWindow::setTitle(const QString &msg) {
    this->setWindowTitle(QString(TITLE) + msg);
}

void MainWindow::InitReadFile(const QString& filename) {
    // Initialize -> settings.json

    QFile file(filename);
    if (!file.exists()) {

        UserInputDialog dialog(this);
        if (dialog.exec() == QDialog::Accepted) {
            username = dialog.getUsername();
            password = dialog.getPassword();
            country = dialog.getCountry();
            postalcode = dialog.getPostalCode();

                // Save the collected data to settings.json
            QJsonArray lineupArray;
            QJsonObject lineups;
            lineups["name"] = {};
            lineups["lineup"] = {};
            lineups["modified"] = {};
            lineups["uri"] = {};
            lineupArray.append(lineups);

            QJsonObject settings;
            settings["username"] = username;
            settings["password"] = password;
            settings["country"] = country;
            settings["postalcode"] = postalcode;
            settings["lineup"] = lineupArray;

            QJsonDocument doc(settings);
            QFile saveFile(filename);
            if (saveFile.open(QIODevice::WriteOnly)) {
                saveFile.write(doc.toJson());
                saveFile.close();
            } else {
                QMessageBox::warning(this, "Error", "Failed to save settings!");
            }
        }
    } else {
        // Read the file and process settings
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray data = file.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);

            // Test read and doc formatting
            qInfo() << "=> doc" << doc.toJson(QJsonDocument::Indented);

            if (doc.isObject()) {
                QJsonObject docObj = doc.object();
                username = docObj["username"].toString();
                password = docObj["password"].toString();
                country = docObj["country"].toString();
                postalcode = docObj["postalcode"].toString();

                if (docObj["lineup"].isArray()) {
                    QJsonArray arr = docObj["lineup"].toArray();
                    if (!arr.isEmpty()) {
                        QJsonObject lineupObj = arr[0].toObject();
                        parseObject(docObj);
                    }
                }

            }

            // Process settings here
            file.close();
        }
    }
}

void MainWindow::writeFile(const QString &filename) {
    QJsonDocument jsonDoc;
    QJsonArray jsonArray;

    if (filename.contains("lineups")) {

        for (auto it = stationMap.begin(); it != stationMap.end(); ++it) {
            QJsonObject stationObj;
            int stationID = it.key();
            Station station = it.value();

            stationObj["stationID"] = stationID;
            stationObj["name"] = station.name;
            stationObj["callSign"] = station.callSign;
            stationObj["uri"] = station.uri;

            jsonArray.append(stationObj);
        }
        jsonDoc = QJsonDocument(jsonArray);

    } else if (filename.contains("settings")) {

        if (lineup) {
            QJsonArray lineupArray;
            QJsonObject lineupObj;

            //Lineup lineup; //assume its defined already!
            lineupObj["name"] = lineup->name;
            lineupObj["lineup"] = lineup->lineup;
            lineupObj["modified"] = lineup->modified;
            lineupObj["uri"] = lineup->uri;
            lineupArray.append(lineupObj);

            QJsonObject settingsObj;
            settingsObj["username"] = username;
            settingsObj["password"] = password;
            settingsObj["country"] = country;
            settingsObj["postalcode"] = postalcode;

            settingsObj["lineup"] = lineupArray;

            jsonArray.append(settingsObj);
            jsonDoc = QJsonDocument(jsonArray);
        } else {
            qWarning() << "=> Lineup not setup.";
        }
    }

    // Write the JSON document to the file
    if (!jsonDoc.isEmpty()) {
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly)) {
            qWarning("Could not open file for writing");
            return;
        }

        file.write(jsonDoc.toJson());
        file.close();
        qInfo() << "=> Filename Exported" << filename;
    } else {
        qWarning() << "=> Error jsonDoc not setup.";
    }
}

