/**
 * @file main.cpp
 * @brief Main entry point for the Cube Rotation Visualization application.
 *
 * This file sets up the main window and integrates the CubeWidget along with dialogs for
 * line rotation and view position. It also configures the application menu, window icon,
 * and signal-slot connections.
 */

#include "dialogs.h"
#include "cubewidget.h"

#include <QApplication>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QTimer>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QList>
#include <QImage>
#include <QVector3D>
#include <QDebug>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMouseEvent>
#include <QPainter>
#include <QQuaternion>
#include <QWheelEvent>
#include <QIcon>

/**
 * @brief MainWindow class that provides the main interface and menu for the application.
 *
 * MainWindow creates and displays the CubeWidget along with a menu for accessing
 * different functionalities such as line rotation, view position, default view, animation,
 * and toggling the gloss effect.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    /**
     * @brief Constructs a MainWindow.
     * @param parent Pointer to the parent widget (default is nullptr).
     *
     * This constructor sets the window icon, creates a CubeWidget as the central widget,
     * and configures the menu options with the corresponding actions.
     */
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        // Set the window icon from the Qt resource system.
        setWindowIcon(QIcon(":/textures/textures/mine.png"));

        // Create the cube widget and set it as the central widget.
        cubeWidget = new CubeWidget(this);
        setCentralWidget(cubeWidget);

        // Create menu and menu actions.
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

        // Connect menu actions to their corresponding slots.
        connect(lineRotAct, &QAction::triggered, this, &MainWindow::onLineRotation);
        connect(viewPosAct, &QAction::triggered, this, &MainWindow::onViewPosition);
        connect(defaultPosAct, &QAction::triggered, cubeWidget, &CubeWidget::resetDefault);
        connect(animAct, &QAction::triggered, cubeWidget, &CubeWidget::toggleAnimation);
        connect(glossAct, &QAction::triggered, cubeWidget, &CubeWidget::toggleGloss);
    }
private slots:
    /**
     * @brief Slot called when the "Line Rotation" action is triggered.
     *
     * Opens the LineRotationDialog pre-filled with default values and, if accepted,
     * applies the specified rotation to the cube.
     */
    void onLineRotation() {
        // Pre-fill with default values (e.g., b=(0,0,0), d=(0,0,1), angle=0)
        LineRotationDialog dlg(this, QVector3D(0, 0, 0), QVector3D(0, 0, 1), 0.0f);
        if (dlg.exec() == QDialog::Accepted) {
            QVector3D b = dlg.getB();
            QVector3D d = dlg.getD();
            float angle = dlg.getAngle();
            cubeWidget->setCustomRotation(b, d, angle);
        }
    }

    /**
     * @brief Slot called when the "View Position" action is triggered.
     *
     * Opens the ViewPositionDialog and, if accepted, updates the camera view.
     */
    void onViewPosition() {
        ViewPositionDialog dlg(this);
        if (dlg.exec() == QDialog::Accepted) {
            cubeWidget->setViewPosition(dlg.getEye(), dlg.getPoint());
        }
    }
private:
    CubeWidget *cubeWidget; ///< Pointer to the cube rendering widget.
};

#include "main.moc"

/**
 * @brief Main entry point of the application.
 *
 * Initializes the QApplication, creates and displays the MainWindow, and starts
 * the event loop.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return int Exit status.
 */
int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    MainWindow win;
    win.resize(800, 600);
    win.show();
    return app.exec();
}
