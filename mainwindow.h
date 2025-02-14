#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class GLWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void showLineRotationDialog();
    void showViewPositionDialog();
    void setDefaultPosition();
    void toggleAnimation();

private:
    GLWidget *glWidget;
};

#endif // MAINWINDOW_H
