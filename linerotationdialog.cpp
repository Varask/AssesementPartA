#include "linerotationdialog.h"
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>

LineRotationDialog::LineRotationDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Line Rotation");
    QFormLayout *layout = new QFormLayout(this);

    // Spin boxes for point b
    QDoubleSpinBox *bX = new QDoubleSpinBox(this);
    QDoubleSpinBox *bY = new QDoubleSpinBox(this);
    QDoubleSpinBox *bZ = new QDoubleSpinBox(this);
    bX->setObjectName("bX");
    bY->setObjectName("bY");
    bZ->setObjectName("bZ");
    layout->addRow("b.x:", bX);
    layout->addRow("b.y:", bY);
    layout->addRow("b.z:", bZ);

    // Spin boxes for direction d
    QDoubleSpinBox *dX = new QDoubleSpinBox(this);
    QDoubleSpinBox *dY = new QDoubleSpinBox(this);
    QDoubleSpinBox *dZ = new QDoubleSpinBox(this);
    dX->setObjectName("dX");
    dY->setObjectName("dY");
    dZ->setObjectName("dZ");
    dY->setValue(1.0);  // default direction (0,1,0)
    layout->addRow("d.x:", dX);
    layout->addRow("d.y:", dY);
    layout->addRow("d.z:", dZ);

    // Spin box for angle
    QDoubleSpinBox *angleSpin = new QDoubleSpinBox(this);
    angleSpin->setRange(-360, 360);
    angleSpin->setSuffix(" deg");
    angleSpin->setObjectName("angleSpin");
    layout->addRow("Angle:", angleSpin);

    QDialogButtonBox *buttonBox =
        new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    layout->addRow(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &LineRotationDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &LineRotationDialog::reject);
}

glm::vec3 LineRotationDialog::getPointB() const
{
    double x = findChild<QDoubleSpinBox*>("bX")->value();
    double y = findChild<QDoubleSpinBox*>("bY")->value();
    double z = findChild<QDoubleSpinBox*>("bZ")->value();
    return glm::vec3(x, y, z);
}

glm::vec3 LineRotationDialog::getDirection() const
{
    double x = findChild<QDoubleSpinBox*>("dX")->value();
    double y = findChild<QDoubleSpinBox*>("dY")->value();
    double z = findChild<QDoubleSpinBox*>("dZ")->value();
    return glm::vec3(x, y, z);
}

float LineRotationDialog::getAngle() const
{
    return static_cast<float>(findChild<QDoubleSpinBox*>("angleSpin")->value());
}
