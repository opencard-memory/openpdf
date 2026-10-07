#include "FontResolver.h"
#include <QDirIterator>
#include <QFile>
#include <QFontDatabase>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

void FontResolver::setAppFontDirectory(const QString& p){ appDir_=p; }
void FontResolver::addUserFont(const QString& p){ if(QFile::exists(p)){ userFiles_ << p; QFontDatabase::addApplicationFont(p); } }
QStringList FontResolver::scanSystemFonts() const { return QFontDatabase::families(); }
bool FontResolver::loadSubstitutionMap(const QString& path){
 QFile f(path); if(!f.open(QIODevice::ReadOnly)) return false;
 const auto rows=QJsonDocument::fromJson(f.readAll()).object()["mappings"].toArray();
 for(const auto& row:rows){ auto o=row.toObject(); QStringList fb; for(auto x:o["fallback"].toArray()) fb<<x.toString(); for(auto x:o["source"].toArray()) substitutions_[x.toString().toLower()]=fb; }
 return true;
}
ResolvedFont FontResolver::resolve(const QString& requested,const QString&) const {
 const auto families=QFontDatabase::families();
 auto find=[&](const QString& name)->ResolvedFont{ for(const auto& f:families) if(f.compare(name,Qt::CaseInsensitive)==0) return {f,{},"system",true}; QDirIterator it(appDir_,{"*.ttf","*.otf","*.ttc"},QDir::Files,QDirIterator::Subdirectories); while(it.hasNext()){ auto p=it.next(); if(QFileInfo(p).baseName().contains(name,Qt::CaseInsensitive)) return {name,p,"bundled_google",true}; } for(auto p:userFiles_) if(QFileInfo(p).baseName().contains(name,Qt::CaseInsensitive)) return {name,p,"user_added",true}; return {}; };
 // Priority: user files, system, bundled is enforced by explicit user match first for requested name.
 for(auto p:userFiles_) if(QFileInfo(p).baseName().contains(requested,Qt::CaseInsensitive)) return {requested,p,"user_added",true};
 auto exact=find(requested); if(!exact.family.isEmpty()) return exact;
 for(const auto& fb:substitutions_.value(requested.toLower())) { auto r=find(fb); if(!r.family.isEmpty()) return r; }
 return {"Noto Sans",{},"fallback",true};
}
