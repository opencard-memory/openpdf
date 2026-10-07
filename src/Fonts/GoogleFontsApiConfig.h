#pragma once
#include <QString>

struct GoogleFontsApiConfig {
    QString endpoint;
    QString apiKey;
    QString metadataCache;
    bool offlineRequired{true};
    bool isValid() const { return !endpoint.isEmpty() && !apiKey.isEmpty(); }
    static GoogleFontsApiConfig loadDevelopmentConfig(const QString& filePath);
};
