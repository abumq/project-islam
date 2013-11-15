#ifndef SIDEBAR_WIDGET_HH
#define SIDEBAR_WIDGET_HH

#include <QWidget>

namespace Ui {
class SidebarWidget;
}
namespace ui {
class SidebarWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit SidebarWidget(QWidget *parent = 0);
    ~SidebarWidget();
    
private slots:
    void on_cboFeatures_currentIndexChanged(const QString &arg1);
    
private:
    Ui::SidebarWidget *ui;
};
} // namespace ui
#endif // SIDEBAR_WIDGET_HH
