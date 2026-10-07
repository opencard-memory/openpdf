#include "OpenSourceEngine.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QTemporaryDir>
#include <QUuid>

bool OpenSourceEngine::supports(PdfFeature f) const {
    switch(f) {
    case PdfFeature::DeleteReorderPages:
    case PdfFeature::Encryption:
        return true;
    default:
        return false;
    }
}
PdfResult OpenSourceEngine::requireOpen() const {
    return working_.isEmpty() ? PdfResult::failure("먼저 PDF를 여세요.") : PdfResult::success();
}
QString OpenSourceEngine::stagedPath(const QString &tag) const {
    return QDir::temp().filePath("openpdf_"+tag+"_"+QUuid::createUuid().toString(QUuid::WithoutBraces)+".pdf");
}
PdfResult OpenSourceEngine::open(const QString &path) {
    if(!QFileInfo::exists(path)) return PdfResult::failure("파일이 없습니다: "+path);
    source_=QFileInfo(path).absoluteFilePath(); working_=stagedPath("work");
    if(!QFile::copy(source_,working_)) return PdfResult::failure("작업 복사본을 만들지 못했습니다.");
    return PdfResult::success("문서를 안전 작업 복사본으로 열었습니다.");
}
PdfResult OpenSourceEngine::runQpdf(const QStringList &args,const QString &success) {
    QProcess p; p.start("qpdf",args);
    if(!p.waitForStarted(5000)) return PdfResult::failure("qpdf 실행 파일을 찾지 못했습니다.");
    if(!p.waitForFinished(120000)) { p.kill(); return PdfResult::failure("qpdf 작업 시간이 초과되었습니다."); }
    if(p.exitCode()!=0) return PdfResult::failure(QString::fromUtf8(p.readAllStandardError()));
    return PdfResult::success(success);
}
PdfResult OpenSourceEngine::deletePages(const QList<int> &pages) {
    auto ready=requireOpen(); if(!ready.ok) return ready;
    // QPDF page selection is 1-based. Build an exclusion expression such as 1-3,5-z.
    // To avoid silently corrupting documents, deletion requires the UI to provide the final kept order.
    return PdfResult::failure("삭제는 reorderPages()에 남길 페이지 순서를 전달해 실행하세요.");
}
PdfResult OpenSourceEngine::reorderPages(const QList<int> &order) {
    auto ready=requireOpen(); if(!ready.ok) return ready;
    if(order.isEmpty()) return PdfResult::failure("페이지 순서가 비어 있습니다.");
    QStringList oneBased; for(int p:order) { if(p<0) return PdfResult::failure("잘못된 페이지 번호입니다."); oneBased << QString::number(p+1); }
    const QString next=stagedPath("pages");
    QStringList a{working_,"--pages",working_}; a << oneBased.join(',') << "--" << next;
    auto r=runQpdf(a,"페이지 삭제/재정렬을 완료했습니다.");
    if(r.ok){ QFile::remove(working_); working_=next; } else QFile::remove(next);
    return r;
}
PdfResult OpenSourceEngine::setPassword(const QString &owner,const QString &user,quint32 permissions) {
    auto ready=requireOpen(); if(!ready.ok) return ready;
    pendingOwner_=owner; pendingUser_=user; pendingPermissions_=permissions;
    return PdfResult::success("AES-256 암호 설정이 저장 대기 중입니다.");
}
PdfResult OpenSourceEngine::save(const QString &path,bool incremental) {
    Q_UNUSED(incremental); auto ready=requireOpen(); if(!ready.ok) return ready;
    const QString tmp=stagedPath("save");
    PdfResult r=PdfResult::success();
    if(!pendingOwner_.isEmpty() || !pendingUser_.isEmpty()) {
        // Passwords are passed to qpdf as process arguments. Product builds should use a protected helper/pipe.
        r=runQpdf({working_,"--encrypt",pendingUser_,pendingOwner_,"256","--",tmp},"AES-256 암호화 저장을 완료했습니다.");
    } else {
        QFile::remove(tmp); if(!QFile::copy(working_,tmp)) r=PdfResult::failure("임시 출력 파일을 만들지 못했습니다.");
    }
    if(!r.ok) return r;
    if(QFile::exists(path) && !QFile::remove(path)) return PdfResult::failure("기존 출력 파일을 교체할 수 없습니다.");
    if(!QFile::rename(tmp,path)) { QFile::remove(tmp); return PdfResult::failure("출력 파일 저장에 실패했습니다."); }
    return PdfResult::success("저장했습니다: "+path);
}
PdfResult OpenSourceEngine::replaceText(int,const QString&,const QString&){return PdfResult::failure("원문 텍스트 재배치는 PDFium 개체 편집 모듈 연결 후 활성화됩니다.");}
PdfResult OpenSourceEngine::replaceImage(int,int,const QString&){return PdfResult::failure("이미지 교체는 PDFium 개체 편집 모듈 연결 후 활성화됩니다.");}
PdfResult OpenSourceEngine::addTextNote(int,const QRectF&,const QString&){return PdfResult::failure("주석 writer 모듈 연결 후 활성화됩니다.");}
PdfResult OpenSourceEngine::runOcr(const QStringList&){return PdfResult::failure("OCR 파이프라인은 페이지 렌더러와 Tesseract 좌표 병합 모듈 연결 후 활성화됩니다.");}
PdfResult OpenSourceEngine::sign(const SignatureOptions&){return PdfResult::failure("전자서명은 OpenSSL CMS와 PDF ByteRange writer 검증 후 활성화됩니다.");}
PdfResult OpenSourceEngine::createFormField(const FormFieldOptions&){return PdfResult::failure("AcroForm writer 모듈 연결 후 활성화됩니다.");}
