#pragma once
#include "IPdfEditorEngine.h"
#include <QLibrary>

class SdkPluginEngine final : public IPdfEditorEngine {
public:
    SdkPluginEngine();
    ~SdkPluginEngine() override;
    bool isLoaded() const;
    QString loadError() const;
    QString engineName() const override;
    bool supports(PdfFeature) const override;
    PdfResult open(const QString&) override;
    PdfResult replaceText(int,const QString&,const QString&) override;
    PdfResult replaceImage(int,int,const QString&) override;
    PdfResult addTextNote(int,const QRectF&,const QString&) override;
    PdfResult deletePages(const QList<int>&) override;
    PdfResult reorderPages(const QList<int>&) override;
    PdfResult runOcr(const QStringList&) override;
    PdfResult setPassword(const QString&,const QString&,quint32) override;
    PdfResult sign(const SignatureOptions&) override;
    PdfResult createFormField(const FormFieldOptions&) override;
    PdfResult save(const QString&,bool) override;
private:
    PdfResult unavailable() const;
    QLibrary library_;
    IPdfEditorEngine *impl_{};
    DestroyPdfEditorEngineFn destroy_{};
    QString error_;
};
