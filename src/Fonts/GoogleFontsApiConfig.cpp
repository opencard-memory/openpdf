#include "GoogleFontsApiConfig.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

GoogleFontsApiConfig GoogleFontsApiConfig::loadDevelopmentConfig(const QString& filePath) {
    GoogleFontsApiConfig result;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return result;
    const auto object = QJsonDocument::fromJson(file.readAll()).object();
    result.endpoint = object.value("endpoint").toString();
    result.apiKey = object.value("apiKey").toString();
    result.metadataCache = object.value("metadataCache").toString();
    result.offlineRequired = object.value("offlineRequired").toBool(true);
    return result;
}
