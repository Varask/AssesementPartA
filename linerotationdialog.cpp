#include "linerotationdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>

LineRotationDialog::LineRotationDialog(QWidget *parent,
                                       const QVector3D &defaultB,
                                       const QVector3D &defaultD,
                                       float defaultAngle)
    : QDialog(parent)
{
    setWindowTitle("Line Rotation");
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Section pour b (point sur la ligne)
    QHBoxLayout *bLayout = new QHBoxLayout();
    bLayout->addWidget(new QLabel("b.x:"));
    bx = new QDoubleSpinBox();
    bx->setRange(-100, 100);
    bx->setValue(defaultB.x());
    bLayout->addWidget(bx);
    bLayout->addWidget(new QLabel("b.y:"));
    by = new QDoubleSpinBox();
    by->setRange(-100, 100);
    by->setValue(defaultB.y());
    bLayout->addWidget(by);
    bLayout->addWidget(new QLabel("b.z:"));
    bz = new QDoubleSpinBox();
    bz->setRange(-100, 100);
    bz->setValue(defaultB.z());
    bLayout->addWidget(bz);
    layout->addLayout(bLayout);

    // Section pour d (vecteur direction)
    QHBoxLayout *dLayout = new QHBoxLayout();
    dLayout->addWidget(new QLabel("d.x:"));
    dx = new QDoubleSpinBox();
    dx->setRange(-100, 100);
    dx->setValue(defaultD.x());
    dLayout->addWidget(dx);
    dLayout->addWidget(new QLabel("d.y:"));
    dy = new QDoubleSpinBox();
    dy->setRange(-100, 100);
    dy->setValue(defaultD.y());
    dLayout->addWidget(dy);
    dLayout->addWidget(new QLabel("d.z:"));
    dz = new QDoubleSpinBox();
    dz->setRange(-100, 100);
    dz->setValue(defaultD.z());
    dLayout->addWidget(dz);
    layout->addLayout(dLayout);

    // Section pour l'angle
    QHBoxLayout *angleLayout = new QHBoxLayout();
    angleLayout->addWidget(new QLabel("Angle (deg):"));
    angleSpin = new QDoubleSpinBox();
    angleSpin->setRange(-360, 360);
    angleSpin->setValue(defaultAngle);
    angleLayout->addWidget(angleSpin);
    layout->addLayout(angleLayout);

    // Boutons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *okBtn = new QPushButton("OK");
    QPushButton *cancelBtn = new QPushButton("Cancel");
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelBtn);
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
