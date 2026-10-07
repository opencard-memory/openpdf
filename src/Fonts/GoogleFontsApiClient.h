#pragma once
#include "GoogleFontsApiConfig.h"
#include <QObject>
#include <QNetworkAccessManager>

class GoogleFontsApiClient final : public QObject {
    Q_OBJECT
public:
    explicit GoogleFontsApiClient(GoogleFontsApiConfig config, QObject* parent=nullptr);
    void refreshCatalog(const QString& sort="popularity");
signals:
    void catalogReady(const QByteArray& json);
    void requestFailed(const QString& message);
private:
    GoogleFontsApiConfig config_;
    QNetworkAccessManager network_;
};
