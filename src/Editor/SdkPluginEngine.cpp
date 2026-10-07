#include "SdkPluginEngine.h"
#include <QCoreApplication>
#include <QDir>

SdkPluginEngine::SdkPluginEngine() {
    const QString path = QDir(QCoreApplication::applicationDirPath()).filePath("pdfengine.dll");
    library_.setFileName(path);
    if (!library_.load()) { error_ = library_.errorString(); return; }
    auto create = reinterpret_cast<CreatePdfEditorEngineFn>(library_.resolve("CreatePdfEditorEngine"));
    destroy_ = reinterpret_cast<DestroyPdfEditorEngineFn>(library_.resolve("DestroyPdfEditorEngine"));
    if (!create || !destroy_) { error_ = "pdfengine.dll ABI가 올바르지 않습니다."; library_.unload(); return; }
    impl_ = create();
    if (!impl_) error_ = "PDF SDK 엔진을 초기화하지 못했습니다. 라이선스 키와 리소스를 확인하세요.";
}
SdkPluginEngine::~SdkPluginEngine(){ if(impl_ && destroy_) destroy_(impl_); }
bool SdkPluginEngine::isLoaded() const { return impl_ != nullptr; }
QString SdkPluginEngine::loadError() const { return error_; }
QString SdkPluginEngine::engineName() const { return impl_ ? impl_->engineName() : "SDK 미연결"; }
bool SdkPluginEngine::supports(PdfFeature f) const { return impl_ && impl_->supports(f); }
PdfResult SdkPluginEngine::unavailable() const { return PdfResult::failure("상용 PDF SDK 플러그인 pdfengine.dll이 필요합니다. " + error_); }
#define CALL0(method) return impl_ ? impl_->method : unavailable()
PdfResult SdkPluginEngine::open(const QString& a){ CALL0(open(a)); }
PdfResult SdkPluginEngine::replaceText(int a,const QString& b,const QString& c){ CALL0(replaceText(a,b,c)); }
PdfResult SdkPluginEngine::replaceImage(int a,int b,const QString& c){ CALL0(replaceImage(a,b,c)); }
PdfResult SdkPluginEngine::addTextNote(int a,const QRectF& b,const QString& c){ CALL0(addTextNote(a,b,c)); }
PdfResult SdkPluginEngine::deletePages(const QList<int>& a){ CALL0(deletePages(a)); }
PdfResult SdkPluginEngine::reorderPages(const QList<int>& a){ CALL0(reorderPages(a)); }
PdfResult SdkPluginEngine::runOcr(const QStringList& a){ CALL0(runOcr(a)); }
PdfResult SdkPluginEngine::setPassword(const QString& a,const QString& b,quint32 c){ CALL0(setPassword(a,b,c)); }
PdfResult SdkPluginEngine::sign(const SignatureOptions& a){ CALL0(sign(a)); }
PdfResult SdkPluginEngine::createFormField(const FormFieldOptions& a){ CALL0(createFormField(a)); }
PdfResult SdkPluginEngine::save(const QString& a,bool b){ CALL0(save(a,b)); }
#undef CALL0
