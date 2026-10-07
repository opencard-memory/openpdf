#include "GoogleFontsApiClient.h"
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>

GoogleFontsApiClient::GoogleFontsApiClient(GoogleFontsApiConfig config, QObject* parent)
    : QObject(parent), config_(std::move(config)) {}

void GoogleFontsApiClient::refreshCatalog(const QString& sort) {
    if (!config_.isValid()) { emit requestFailed("Google Fonts API development configuration is missing."); return; }
    QUrl url(config_.endpoint);
    QUrlQuery query;
    query.addQueryItem("key", config_.apiKey);
    query.addQueryItem("sort", sort);
    query.addQueryItem("capability", "VF");
    url.setQuery(query);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, "OpenPDFOffice-Development/0.4");
    auto* reply = network_.get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        const auto guard = reply->error();
        const QByteArray body = reply->readAll();
        const QString error = reply->errorString();
        reply->deleteLater();
        if (guard != QNetworkReply::NoError) emit requestFailed(error);
        else emit catalogReady(body);
    });
}
