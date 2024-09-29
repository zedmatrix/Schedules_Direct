#include "mainwindow.h"
/*
    JSON Type Parser for Debug purposes

    enum Type {      <- #include in mainwindow.h
    Null,
    Bool,
    Double,
    String,
    Array,
    Object,
    Undef
    };

*/
auto toType = [](QJsonValue::Type type) -> QString {
    switch (type) {
        case QJsonValue::Null:
            return "Null";
        case QJsonValue::Bool:
            return "Boolean";
        case QJsonValue::Double:
            return "Double";
        case QJsonValue::String:
            return "String";
        case QJsonValue::Array:
            return "Array";
        case QJsonValue::Object:
            return "Object";
        default:
            return "Undefined";
    }
};

auto mapQJsonType = [](QJsonValue::Type jsonType) -> Type {
    switch (jsonType) {
        case QJsonValue::Null:
            return Null;
        case QJsonValue::Bool:
            return Bool;
        case QJsonValue::Double:
            return Double;
        case QJsonValue::String:
            return String;
        case QJsonValue::Array:
            return Array;
        case QJsonValue::Object:
            return Object;
        default:
            return Undef;
    }
};

void MainWindow::parseObject (const QJsonObject& obj) {
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        QString key = it.key();
        QJsonValue value = it.value();
        QString type = toType(value.type());
        QString size;
        if (type.contains("Array")) {
            QJsonArray arr = value.toArray();
            size = QString::number(arr.size());
        } else if (type.contains("Object")) {
            QJsonObject vObj = value.toObject();
            size = vObj.keys().join(',');
        } else if (type.contains("String")) {
            size = value.toString();
        } else {
            size = {};
        }

        qInfo() << QString("=> parser => Key: [%1] => Size: (%2) => Type: (%3)").arg(key).arg(size).arg(type);
    }

}

