#pragma once
#include <QString>
#include <QStringList>
#include <QRectF>
#include <QByteArray>
#include <memory>
#include <utility>
#include <QList>

enum class PdfFeature {
    EditText, ReplaceImage, SaveAnnotations, DeleteReorderPages,
    Ocr, Encryption, DigitalSignature, CreateFormFields, IncrementalSave
};

struct PdfResult {
    bool ok{};
    QString message;
    static PdfResult success(QString m = {}) { return {true, std::move(m)}; }
    static PdfResult failure(QString m) { return {false, std::move(m)}; }
};

struct SignatureOptions {
    QString pfxPath;
    QString password;
    QString reason;
    QString location;
    int page{};
    QRectF rect;
};

struct FormFieldOptions {
    enum class Type { Text, CheckBox, RadioButton, ComboBox, Signature } type{Type::Text};
    QString name;
    int page{};
    QRectF rect;
};

class IPdfEditorEngine {
public:
    virtual ~IPdfEditorEngine() = default;
    virtual QString engineName() const = 0;
    virtual bool supports(PdfFeature feature) const = 0;
    virtual PdfResult open(const QString &path) = 0;
    virtual PdfResult replaceText(int page, const QString &find, const QString &replacement) = 0;
    virtual PdfResult replaceImage(int page, int imageIndex, const QString &imagePath) = 0;
    virtual PdfResult addTextNote(int page, const QRectF &rect, const QString &text) = 0;
    virtual PdfResult deletePages(const QList<int> &zeroBasedPages) = 0;
    virtual PdfResult reorderPages(const QList<int> &newZeroBasedOrder) = 0;
    virtual PdfResult runOcr(const QStringList &languages) = 0;
    virtual PdfResult setPassword(const QString &owner, const QString &user, quint32 permissions) = 0;
    virtual PdfResult sign(const SignatureOptions &options) = 0;
    virtual PdfResult createFormField(const FormFieldOptions &options) = 0;
    virtual PdfResult save(const QString &path, bool incremental) = 0;
};

using CreatePdfEditorEngineFn = IPdfEditorEngine* (*)();
using DestroyPdfEditorEngineFn = void (*)(IPdfEditorEngine*);
