#pragma once
#include <QString>
#include <QHash>
class LocaleManager {
public:
    bool initialize(const QString& i18nDirectory, const QString& preferredLocale = {});
    QString text(const QString& key) const;
    QString locale() const { return locale_; }
    static QString supportedLocaleForSystem();
private:
    bool load(const QString& path);
    QString locale_{"en"};
    QHash<QString, QString> strings_;
};
