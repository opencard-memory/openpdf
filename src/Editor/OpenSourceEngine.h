#pragma once
#include "IPdfEditorEngine.h"
#include <QString>

class OpenSourceEngine final : public IPdfEditorEngine {
public:
    QString engineName() const override { return "OpenPDF OSS Engine (QPDF/Tesseract/OpenSSL)"; }
    bool supports(PdfFeature feature) const override;
    PdfResult open(const QString &path) override;
    PdfResult replaceText(int page,const QString &find,const QString &replacement) override;
    PdfResult replaceImage(int page,int imageIndex,const QString &imagePath) override;
    PdfResult addTextNote(int page,const QRectF &rect,const QString &text) override;
    PdfResult deletePages(const QList<int> &pages) override;
    PdfResult reorderPages(const QList<int> &order) override;
    PdfResult runOcr(const QStringList &languages) override;
    PdfResult setPassword(const QString &owner,const QString &user,quint32 permissions) override;
    PdfResult sign(const SignatureOptions &options) override;
    PdfResult createFormField(const FormFieldOptions &options) override;
    PdfResult save(const QString &path,bool incremental) override;
private:
    PdfResult runQpdf(const QStringList &args,const QString &success);
    PdfResult requireOpen() const;
    QString stagedPath(const QString &tag) const;
    QString source_;
    QString working_;
    QString pendingOwner_;
    QString pendingUser_;
    quint32 pendingPermissions_{};
};
