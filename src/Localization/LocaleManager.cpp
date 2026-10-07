#include "LocaleManager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLocale>
#include <QDir>

QString LocaleManager::supportedLocaleForSystem() {
    const QString name = QLocale::system().name().replace('_','-').toLower();
    if (name == "ko" || name.startsWith("ko-")) return "ko";
    if (name == "es" || name.startsWith("es-")) return "es";
    return "en";
}
bool LocaleManager::initialize(const QString& dir, const QString& preferred) {
    locale_ = preferred.isEmpty() ? supportedLocaleForSystem() : preferred.toLower();
    if (locale_.startsWith("ko")) locale_="ko";
    else if (locale_.startsWith("es")) locale_="es";
    else locale_="en";
    if (!load(QDir(dir).filePath(locale_+".json")) && locale_!="en") { locale_="en"; return load(QDir(dir).filePath("en.json")); }
    return !strings_.isEmpty();
}
bool LocaleManager::load(const QString& path) {
    QFile f(path); if(!f.open(QIODevice::ReadOnly)) return false;
    const auto values=QJsonDocument::fromJson(f.readAll()).object().value("strings").toObject();
    strings_.clear(); for(auto it=values.begin();it!=values.end();++it) strings_[it.key()]=it.value().toString();
    return true;
}
QString LocaleManager::text(const QString& key) const { return strings_.value(key,key); }
