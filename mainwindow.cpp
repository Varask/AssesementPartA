#include "mainwindow.h"
#include "glwidget.h"
#include "linerotationdialog.h"
#include "viewpositiondialog.h"

#include <QMenuBar>
#include <QAction>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    glWidget = new GLWidget(this);
    setCentralWidget(glWidget);

    QMenu *menu = menuBar()->addMenu("Options");

    QAction *lineRotationAct = new QAction("Line Rotation", this);
    connect(lineRotationAct, &QAction::triggered, this, &MainWindow::showLineRotationDialog);
    menu->addAction(lineRotationAct);

    QAction *viewPosAct = new QAction("View Position", this);
    connect(viewPosAct, &QAction::triggered, this, &MainWindow::showViewPositionDialog);
    menu->addAction(viewPosAct);

    QAction *defaultPosAct = new QAction("Default Position", this);
    connect(defaultPosAct, &QAction::triggered, this, &MainWindow::setDefaultPosition);
    menu->addAction(defaultPosAct);

    QAction *animationAct = new QAction("Animation", this);
    animationAct->setCheckable(true);
    connect(animationAct, &QAction::triggered, this, &MainWindow::toggleAnimation);
    menu->addAction(animationAct);
}

void MainWindow::showLineRotationDialog()
{
    LineRotationDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        // Get user inputs
        glm::vec3 b = dlg.getPointB();
        glm::vec3 d = dlg.getDirection();
        float angle = dlg.getAngle();
        glWidget->setLineRotation(b, d, angle);
    }
}

void MainWindow::showViewPositionDialog()
{
    ViewPositionDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        glm::vec3 eye = dlg.getEyePosition();
        glm::vec3 center = dlg.getCenterPoint();
        glWidget->setViewPosition(eye, center);
    }
}

void MainWindow::setDefaultPosition()
{
    glWidget->resetView();
}

void MainWindow::toggleAnimation()
{
    glWidget->toggleAnimation();
}
