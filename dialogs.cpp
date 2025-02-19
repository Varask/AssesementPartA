#include "dialogs.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDoubleSpinBox>

//----------------------
// Implementation of LineRotationDialog
//----------------------
LineRotationDialog::LineRotationDialog(QWidget *parent,
                                       const QVector3D &defaultB,
                                       const QVector3D &defaultD,
                                       float defaultAngle)
    : QDialog(parent)
{
    setWindowTitle("Line Rotation");
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Create spin boxes for b components first
    bx = new QDoubleSpinBox();
    by = new QDoubleSpinBox();
    bz = new QDoubleSpinBox();
    bx->setRange(-100, 100);
    by->setRange(-100, 100);
    bz->setRange(-100, 100);
    bx->setValue(defaultB.x());
    by->setValue(defaultB.y());
    bz->setValue(defaultB.z());

    QHBoxLayout *bLayout = new QHBoxLayout();
    bLayout->addWidget(new QLabel("b.x:"));
    bLayout->addWidget(bx);
    bLayout->addWidget(new QLabel("b.y:"));
    bLayout->addWidget(by);
    bLayout->addWidget(new QLabel("b.z:"));
    bLayout->addWidget(bz);
    layout->addLayout(bLayout);

    // Create spin boxes for d components
    dx = new QDoubleSpinBox();
    dy = new QDoubleSpinBox();
    dz = new QDoubleSpinBox();
    dx->setRange(-100, 100);
    dy->setRange(-100, 100);
    dz->setRange(-100, 100);
    dx->setValue(defaultD.x());
    dy->setValue(defaultD.y());
    dz->setValue(defaultD.z());

    QHBoxLayout *dLayout = new QHBoxLayout();
    dLayout->addWidget(new QLabel("d.x:"));
    dLayout->addWidget(dx);
    dLayout->addWidget(new QLabel("d.y:"));
    dLayout->addWidget(dy);
    dLayout->addWidget(new QLabel("d.z:"));
    dLayout->addWidget(dz);
    layout->addLayout(dLayout);

    // Create spin box for angle
    angleSpin = new QDoubleSpinBox();
    angleSpin->setRange(-360, 360);
    angleSpin->setValue(defaultAngle);
    QHBoxLayout *angleLayout = new QHBoxLayout();
    angleLayout->addWidget(new QLabel("Angle (deg):"));
    angleLayout->addWidget(angleSpin);
    layout->addLayout(angleLayout);

    // Buttons layout
    QPushButton *cancelBtn = new QPushButton("Cancel");
    QPushButton *okBtn = new QPushButton("OK");
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(okBtn);
    layout->addLayout(btnLayout);

    connect(okBtn, &QPushButton::clicked, this, &LineRotationDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, this, &LineRotationDialog::reject);
}

QVector3D LineRotationDialog::getB() const {
    return QVector3D(bx->value(), by->value(), bz->value());
}

QVector3D LineRotationDialog::getD() const {
    return QVector3D(dx->value(), dy->value(), dz->value());
}

float LineRotationDialog::getAngle() const {
    return angleSpin->value();
}

//----------------------
// Implementation of ViewPositionDialog
//----------------------
ViewPositionDialog::ViewPositionDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("View Position");
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Create spin boxes for eye coordinates
    ex = new QDoubleSpinBox();
    ey = new QDoubleSpinBox();
    ez = new QDoubleSpinBox();
    ex->setRange(-100, 100);
    ey->setRange(-100, 100);
    ez->setRange(-100, 100);

    QHBoxLayout *eyeLayout = new QHBoxLayout();
    eyeLayout->addWidget(new QLabel("Eye x:"));
    eyeLayout->addWidget(ex);
    eyeLayout->addWidget(new QLabel("Eye y:"));
    eyeLayout->addWidget(ey);
    eyeLayout->addWidget(new QLabel("Eye z:"));
    eyeLayout->addWidget(ez);
    layout->addLayout(eyeLayout);

    // Create spin boxes for point coordinates
    px = new QDoubleSpinBox();
    py = new QDoubleSpinBox();
    pz = new QDoubleSpinBox();
    px->setRange(-100, 100);
    py->setRange(-100, 100);
    pz->setRange(-100, 100);

    QHBoxLayout *pointLayout = new QHBoxLayout();
    pointLayout->addWidget(new QLabel("Point x:"));
    pointLayout->addWidget(px);
    pointLayout->addWidget(new QLabel("Point y:"));
    pointLayout->addWidget(py);
    pointLayout->addWidget(new QLabel("Point z:"));
    pointLayout->addWidget(pz);
    layout->addLayout(pointLayout);

    // Buttons layout
    QPushButton *cancelBtn = new QPushButton("Cancel");
    QPushButton *okBtn = new QPushButton("OK");
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(okBtn);
    layout->addLayout(btnLayout);

    connect(okBtn, &QPushButton::clicked, this, &ViewPositionDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, this, &ViewPositionDialog::reject);
}

QVector3D ViewPositionDialog::getEye() const {
    return QVector3D(ex->value(), ey->value(), ez->value());
}

QVector3D ViewPositionDialog::getPoint() const {
    return QVector3D(px->value(), py->value(), pz->value());
}
