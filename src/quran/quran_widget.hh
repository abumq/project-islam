#ifndef QURAN_WIDGET_HH
#define QURAN_WIDGET_HH

#include <QWidget>
#include "quran/quran.hh"

namespace Ui {
class QuranWidget;
}
class QToolBar;
namespace quran {
namespace ui {
class QuranReader;
class QuranWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit QuranWidget(QWidget *parent = 0);
    ~QuranWidget();
    
private slots:
    void on_cboChapter_currentIndexChanged(int index);
    
    void on_spnVerse_valueChanged(int arg1);
    
    void on_btnSearch_clicked();
    void toggleSearch(bool val);
    
private:
    Ui::QuranWidget *ui;
    QToolBar* toolBar;
    quran::Quran quran;
    quran::ui::QuranReader* reader;
    const quran::Chapter* chapter;
    bool selectedVerse;
};
} // namespace ui
} // namespace quran

#endif // QURAN_WIDGET_HH
