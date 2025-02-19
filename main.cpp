#include "dialogs.h"
#include "cubewidget.h"

#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>

//----------------------
// MainWindow with menu and state machine
//----------------------
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        // Ajout de l'icône pour la fenêtre (via le fichier qrc)
        setWindowIcon(QIcon(":/textures/textures/mine.png"));

        cubeWidget = new CubeWidget(this);
        setCentralWidget(cubeWidget);

        QMenuBar *menuBarPtr = this->menuBar();
        QMenu *menu = menuBarPtr->addMenu("Options");

        QAction *lineRotAct = new QAction("Line Rotation", this);
        QAction *viewPosAct = new QAction("View Position", this);
        QAction *defaultPosAct = new QAction("Default Position", this);
        QAction *animAct = new QAction("Animation", this);
        QAction *glossAct = new QAction("Toggle Gloss", this);

        menu->addAction(lineRotAct);
        menu->addAction(viewPosAct);
        menu->addAction(defaultPosAct);
        menu->addAction(animAct);
        menu->addAction(glossAct);

        connect(lineRotAct, &QAction::triggered, this, &MainWindow::onLineRotation);
        connect(viewPosAct, &QAction::triggered, this, &MainWindow::onViewPosition);
        connect(defaultPosAct, &QAction::triggered, cubeWidget, &CubeWidget::resetDefault);
        connect(animAct, &QAction::triggered, cubeWidget, &CubeWidget::toggleAnimation);
        connect(glossAct, &QAction::triggered, cubeWidget, &CubeWidget::toggleGloss);
    }
private slots:
    void onLineRotation() {
        // Pré-remplissage avec la position actuelle (ici, centre du cube) et direction par défaut
        LineRotationDialog dlg(this, QVector3D(0,0,0), QVector3D(0,0,1), 0.0f);
        if (dlg.exec() == QDialog::Accepted) {
            QVector3D b = dlg.getB();
            QVector3D d = dlg.getD();
            float angle = dlg.getAngle();
            cubeWidget->setCustomRotation(b, d, angle);
        }
    }
    void onViewPosition() {
        ViewPositionDialog dlg(this);
        if (dlg.exec() == QDialog::Accepted) {
            cubeWidget->setViewPosition(dlg.getEye(), dlg.getPoint());
        }
    }
private:
    CubeWidget *cubeWidget;
};

#include "main.moc"

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    MainWindow win;
    win.resize(800,600);
    win.show();
    return app.exec();
}
