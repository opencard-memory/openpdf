#include "MainWindow.h"
#include <QPdfView>
#include <QPdfPageNavigator>
#include <QApplication>
#include <QBoxLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPainter>
#include <QPrintDialog>
#include <QPrinter>
#include <QPushButton>
#include <QScrollArea>
#include <QSlider>
#include <QSplitter>
#include <QStatusBar>
#include <QTabWidget>
#include <QToolButton>

static QToolButton *commandButton(const QString &text, const QString &tip = {}) {
    auto *b = new QToolButton;
    b->setText(text);
    b->setToolTip(tip);
    b->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    b->setMinimumSize(82, 62);
    b->setAutoRaise(true);
    return b;
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), document_(new QPdfDocument(this)) {
    setWindowTitle("OpenPDF Office");
    resize(1380, 880);
    setMinimumSize(980, 620);

    auto *root = new QWidget;
    auto *layout = new QVBoxLayout(root);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    layout->addWidget(makeRibbon());

    thumbnails_ = new QListWidget;
    thumbnails_->setIconSize(QSize(150, 200));
    thumbnails_->setMinimumWidth(205);
    thumbnails_->setMaximumWidth(260);
    thumbnails_->setSpacing(8);

    view_ = new QPdfView;
    view_->setDocument(document_);
    view_->setPageMode(QPdfView::PageMode::MultiPage);
    view_->setZoomMode(QPdfView::ZoomMode::FitToWidth);
    view_->setDocumentMargins(QMargins(24,24,24,24));
    view_->setPageSpacing(14);

    auto *splitter = new QSplitter;
    splitter->addWidget(thumbnails_);
    splitter->addWidget(view_);
    splitter->setStretchFactor(1, 1);
    layout->addWidget(splitter, 1);
    setCentralWidget(root);

    pageStatus_ = new QLabel("문서 없음");
    zoomStatus_ = new QLabel("100%");
    zoomSlider_ = new QSlider(Qt::Horizontal);
    zoomSlider_->setRange(25, 300);
    zoomSlider_->setValue(100);
    zoomSlider_->setMaximumWidth(150);
    statusBar()->addWidget(pageStatus_);
    statusBar()->addPermanentWidget(zoomSlider_);
    statusBar()->addPermanentWidget(zoomStatus_);

    connect(thumbnails_, &QListWidget::currentRowChanged, this, &MainWindow::goToPage);
    connect(zoomSlider_, &QSlider::valueChanged, this, [this](int v){ setZoom(v / 100.0); });
    connect(document_, &QPdfDocument::statusChanged, this, [this](QPdfDocument::Status status){
        if (status == QPdfDocument::Status::Ready) rebuildThumbnails();
        if (status == QPdfDocument::Status::Error)
            QMessageBox::critical(this, "열기 실패", "PDF를 열 수 없습니다. 파일이 손상되었거나 암호가 필요할 수 있습니다.");
    });
    connect(view_->pageNavigator(), &QPdfPageNavigator::currentPageChanged, this, &MainWindow::updatePageStatus);
    applyStyle();
    setDocumentUiEnabled(false);
}

QWidget *MainWindow::makeRibbon() {
    auto *wrap = new QWidget;
    auto *v = new QVBoxLayout(wrap);
    v->setContentsMargins(0,0,0,0); v->setSpacing(0);

    auto *title = new QFrame;
    title->setObjectName("titleBar");
    auto *h = new QHBoxLayout(title);
    auto *brand = new QLabel("▣  OpenPDF Office"); brand->setObjectName("brand");
    h->addWidget(brand); h->addStretch();
    auto *open = new QPushButton("열기");
    auto *save = new QPushButton("복사본 저장");
    open->setObjectName("titleButton"); save->setObjectName("titleButton");
    h->addWidget(open); h->addWidget(save);
    connect(open, &QPushButton::clicked, this, &MainWindow::openFile);
    connect(save, &QPushButton::clicked, this, &MainWindow::saveCopy);
    v->addWidget(title);

    ribbon_ = new QTabWidget;
    ribbon_->setDocumentMode(true);
    ribbon_->setFixedHeight(122);
    ribbon_->addTab(makeHomeTab(), "홈");
    ribbon_->addTab(makeViewTab(), "보기");
    ribbon_->addTab(makeProtectTab(), "보호");
    v->addWidget(ribbon_);
    return wrap;
}

QWidget *MainWindow::makeHomeTab() {
    auto *w = new QWidget; auto *h = new QHBoxLayout(w); h->setAlignment(Qt::AlignLeft);
    auto *open = commandButton("PDF 열기", "Ctrl+O");
    auto *save = commandButton("복사본 저장", "Ctrl+Shift+S");
    auto *print = commandButton("인쇄", "Ctrl+P");
    searchBox_ = new QLineEdit; searchBox_->setPlaceholderText("문서에서 찾기"); searchBox_->setMinimumWidth(230);
    auto *find = commandButton("찾기", "Enter");
    h->addWidget(open); h->addWidget(save); h->addWidget(print); h->addSpacing(16); h->addWidget(searchBox_); h->addWidget(find); h->addStretch();
    connect(open, &QToolButton::clicked, this, &MainWindow::openFile);
    connect(save, &QToolButton::clicked, this, &MainWindow::saveCopy);
    connect(print, &QToolButton::clicked, this, &MainWindow::printDocument);
    connect(find, &QToolButton::clicked, this, &MainWindow::findText);
    connect(searchBox_, &QLineEdit::returnPressed, this, &MainWindow::findText);
    return w;
}

QWidget *MainWindow::makeViewTab() {
    auto *w = new QWidget; auto *h = new QHBoxLayout(w); h->setAlignment(Qt::AlignLeft);
    auto *zin=commandButton("확대"); auto *zout=commandButton("축소"); auto *fit=commandButton("너비 맞춤");
    h->addWidget(zin); h->addWidget(zout); h->addWidget(fit); h->addStretch();
    connect(zin,&QToolButton::clicked,this,&MainWindow::zoomIn);
    connect(zout,&QToolButton::clicked,this,&MainWindow::zoomOut);
    connect(fit,&QToolButton::clicked,this,&MainWindow::fitWidth);
    return w;
}

QWidget *MainWindow::makeProtectTab() {
    auto *w=new QWidget; auto *h=new QHBoxLayout(w);
    auto *info=new QLabel("보호·전자서명·원문 개체 편집은 상용 PDF SDK 연결이 필요한 다음 단계 기능입니다.");
    auto *about=commandButton("제품 정보"); h->addWidget(info); h->addStretch(); h->addWidget(about);
    connect(about,&QToolButton::clicked,this,&MainWindow::showAbout); return w;
}

void MainWindow::openFile() {
    const QString p=QFileDialog::getOpenFileName(this,"PDF 열기",{},"PDF 문서 (*.pdf)");
    if(!p.isEmpty()) openPath(p);
}

void MainWindow::openPath(const QString &path) {
    document_->close(); thumbnails_->clear();
    const auto error=document_->load(path);
    if(error != QPdfDocument::Error::None) {
        QMessageBox::critical(this,"열기 실패","선택한 PDF를 열 수 없습니다."); return;
    }
    currentPath_=path; setWindowTitle(QFileInfo(path).fileName()+" - OpenPDF Office"); setDocumentUiEnabled(true);
}

void MainWindow::saveCopy() {
    if(currentPath_.isEmpty()) return;
    const QString p=QFileDialog::getSaveFileName(this,"복사본 저장",QFileInfo(currentPath_).completeBaseName()+" - copy.pdf","PDF 문서 (*.pdf)");
    if(p.isEmpty()) return;
    if(QFile::exists(p)) QFile::remove(p);
    if(!QFile::copy(currentPath_,p)) QMessageBox::warning(this,"저장 실패","복사본을 저장하지 못했습니다.");
    else statusBar()->showMessage("복사본을 저장했습니다: "+p,4000);
}

void MainWindow::printDocument() {
    if(document_->pageCount()<=0) return;
    QPrinter printer(QPrinter::HighResolution); QPrintDialog dialog(&printer,this);
    if(dialog.exec()!=QDialog::Accepted) return;
    QPainter painter(&printer);
    for(int i=0;i<document_->pageCount();++i){
        if(i>0) printer.newPage();
        const QRect pageRect=printer.pageLayout().paintRectPixels(printer.resolution());
        QImage image=document_->render(i,pageRect.size(),QPdfDocumentRenderOptions());
        painter.drawImage(pageRect,image);
    }
}

void MainWindow::rebuildThumbnails() {
    thumbnails_->clear();
    for(int i=0;i<document_->pageCount();++i){
        QImage image=document_->render(i,QSize(150,200),QPdfDocumentRenderOptions());
        auto *item=new QListWidgetItem(QIcon(QPixmap::fromImage(image)),QString("페이지 %1").arg(i+1));
        item->setTextAlignment(Qt::AlignHCenter); thumbnails_->addItem(item);
    }
    if(document_->pageCount()>0) thumbnails_->setCurrentRow(0);
    updatePageStatus();
}

void MainWindow::goToPage(int row) {
    if(row>=0 && row<document_->pageCount()) view_->pageNavigator()->jump(row, QPointF(), view_->zoomFactor());
}
void MainWindow::updatePageStatus() {
    const int p=view_->pageNavigator()->currentPage()+1;
    pageStatus_->setText(document_->pageCount()>0?QString("페이지 %1 / %2").arg(p).arg(document_->pageCount()):"문서 없음");
    if(p>0 && thumbnails_->currentRow()!=p-1) thumbnails_->setCurrentRow(p-1);
}
void MainWindow::setZoom(qreal value){
    value=qBound(0.25,value,3.0); view_->setZoomMode(QPdfView::ZoomMode::Custom); view_->setZoomFactor(value);
    const int pct=qRound(value*100); zoomStatus_->setText(QString::number(pct)+"%");
    if(zoomSlider_->value()!=pct) { QSignalBlocker b(zoomSlider_); zoomSlider_->setValue(pct); }
}
void MainWindow::zoomIn(){ setZoom(view_->zoomFactor()*1.15); }
void MainWindow::zoomOut(){ setZoom(view_->zoomFactor()/1.15); }
void MainWindow::fitWidth(){ view_->setZoomMode(QPdfView::ZoomMode::FitToWidth); zoomStatus_->setText("너비 맞춤"); }
void MainWindow::findText(){
    if(searchBox_->text().trimmed().isEmpty()) return;
    QMessageBox::information(this,"검색","이 MVP에서는 검색 UI가 준비되어 있습니다. 실제 검색 결과 모델 연결은 다음 버전에 포함됩니다.");
}
void MainWindow::showAbout(){
    QMessageBox::about(this,"OpenPDF Office","OpenPDF Office 0.1\nC++20 + Qt 6 PDF 기반 Windows MVP\n\n현재 기능: 열기, 보기, 썸네일, 확대/축소, 인쇄, 복사본 저장.");
}
void MainWindow::setDocumentUiEnabled(bool enabled){ if(ribbon_) ribbon_->setEnabled(true); Q_UNUSED(enabled); }
void MainWindow::applyStyle(){
    setStyleSheet(R"(
        QMainWindow { background:#d7d3cc; }
        #titleBar { background:#702044; color:white; }
        #brand { font-size:16px; font-weight:700; padding:7px; }
        #titleButton { background:white; color:#702044; border:0; border-radius:5px; padding:7px 13px; font-weight:600; }
        QTabWidget::pane { border:0; background:#f7f5f1; }
        QTabBar::tab { padding:8px 22px; background:#651b3d; color:white; }
        QTabBar::tab:selected { background:#f7f5f1; color:#651b3d; font-weight:700; }
        QToolButton { color:#323238; border-radius:7px; padding:5px; }
        QToolButton:hover { background:#ead8e1; color:#651b3d; }
        QListWidget { background:#f7f5f1; border:0; padding:8px; }
        QListWidget::item { padding:8px; border-radius:7px; }
        QListWidget::item:selected { background:#ead4df; color:#651b3d; }
        QPdfView { background:#d3cfc7; border:0; }
        QStatusBar { background:#f8f7f3; }
    )");
}
