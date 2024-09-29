#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QStatusBar>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QWidget>
#include <QVector>
#include <QMap>
#include <QByteArray>
#include <QBuffer>
#include <QStandardItem>
#include <QStandardPaths>
#include <QMessageBox>
#include <zlib.h>
#include "UserInputDialog.h"
#include "ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum Type {
    Null,
    Bool,
    Double,
    String,
    Array,
    Object,
    Undef
};

struct Station {
    QString name;
    QString callSign;
    QString uri;
};

extern QMap<int, Station> stationMap;

struct Lineup {
    QString name;
    QString modified;
    QString lineup;
    QString uri;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onNetworkReply(QNetworkReply* reply);

protected:

private slots:

private:
    Ui::MainWindow *ui;

    static constexpr const char* URLBASE = "https://json.schedulesdirect.org";
    static constexpr const char* TITLE = "TV Guide";
    QString username, password, country, postalcode;
    QString token, stationID, stationName, statusMsg, statusError;
    QString requestUrl, outText;
    Lineup* lineup = nullptr;
    void updateStatus();
    void setTitle(const QString &title);
    void InitReadFile(const QString& filename);
    void writeFile(const QString &filename);
    QByteArray decompressGzip(const QByteArray &data);
    QJsonDocument jsonResponse;
    QNetworkAccessManager *manager;
    void parseObject (const QJsonObject& obj);
    void requestToken(const QString &username, const QString &password);
    void parseToken(const QJsonObject &tokenObj);
    void requestStatus();
    void StatusReceived(const QJsonObject &statusObj);
    void requestHeadend();
    void useColumnView(QJsonArray headendsArray);
    void HeadendDoubleClicked(const QModelIndex &index);
    void displayPreview(const QJsonArray &previewArray);

//     void requestLineup(Lineup* lineup);
//     void previewLineup(const QJsonObject &jsonObj);
//     void catchSelectStation(QTableWidgetItem *item);
//     void appendLineup(const int &row);
//     void displayLineup();
//     void requestSchedule(const QString &stationID);
//     void fetchSchedules(const QJsonDocument& jsonResponse);
//     void initPrograms(const QString &token, const QStringList &programIDList);
//     void fetchPrograms(const QJsonDocument& jsonResponse);
//     void ProgramData(int i, const QJsonObject &programObj);
//     void onColumnItemSelected(const QModelIndex &index);
//     void requestPreview(QString &lineupURL);
//     QStringList programIDList;



};

#endif // MAINWINDOW_H
