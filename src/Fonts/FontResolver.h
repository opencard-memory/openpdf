#pragma once
#include <QString>
#include <QStringList>
#include <QHash>

struct ResolvedFont { QString family; QString path; QString source; bool embeddable{}; };
class FontResolver {
public:
    void setAppFontDirectory(const QString& path);
    void addUserFont(const QString& path);
    bool loadSubstitutionMap(const QString& jsonPath);
    ResolvedFont resolve(const QString& requestedFamily, const QString& locale) const;
    QStringList scanSystemFonts() const;
private:
    QString appDir_;
    QStringList userFiles_;
    QHash<QString, QStringList> substitutions_;
};
