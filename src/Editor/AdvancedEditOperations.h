#pragma once
#include <QString>
#include <QRectF>

struct TextReflowRequest {
    int page{};
    QRectF bounds;
    QString originalText;
    QString replacementText;
    QString preferredFont{"Noto Sans CJK KR"};
    double minimumPointSize{6.0};
    bool preserveBackground{true};
};
struct ImageXObjectReplacement {
    int page{};
    QString resourceName; // e.g. Im4
    QString imagePath;
    bool preservePlacementMatrix{true};
    bool preserveSoftMask{true};
};
struct PadesSignatureRequest {
    QString inputPath;
    QString outputPath;
    QString pfxPath;
    QString pfxPassword;
    QString reason;
    QString location;
    int page{};
    QRectF widgetRect;
    bool incremental{true};
};
