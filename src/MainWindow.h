#pragma once
#include <QMainWindow>
#include <QPdfDocument>

class QPdfView;
class QListWidget;
class QLabel;
class QLineEdit;
class QSlider;
class QTabWidget;
class QTimer;

class MainWindow final : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void openPath(const QString &path);

private slots:
    void openFile();
    void saveCopy();
    void printDocument();
    void rebuildThumbnails();
    void goToPage(int row);
    void updatePageStatus();
    void zoomIn();
    void zoomOut();
    void fitWidth();
    void findText();
    void showAbout();

private:
    QWidget *makeRibbon();
    QWidget *makeHomeTab();
    QWidget *makeViewTab();
    QWidget *makeProtectTab();
    void applyStyle();
    void setZoom(qreal value);
    void setDocumentUiEnabled(bool enabled);

    QPdfDocument *document_{};
    QPdfView *view_{};
    QListWidget *thumbnails_{};
    QLabel *pageStatus_{};
    QLabel *zoomStatus_{};
    QLineEdit *searchBox_{};
    QSlider *zoomSlider_{};
    QTabWidget *ribbon_{};
    QString currentPath_;
};
