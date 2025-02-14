#include "viewpositiondialog.h"
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>

ViewPositionDialog::ViewPositionDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("View Position");
    QFormLayout *layout = new QFormLayout(this);

    // Spin boxes for eye position
    QDoubleSpinBox *eyeX = new QDoubleSpinBox(this);
    QDoubleSpinBox *eyeY = new QDoubleSpinBox(this);
    QDoubleSpinBox *eyeZ = new QDoubleSpinBox(this);
    eyeX->setObjectName("eyeX");
    eyeY->setObjectName("eyeY");
    eyeZ->setObjectName("eyeZ");
    eyeZ->setValue(3.0);  // default eye (0,0,3)
    layout->addRow("Eye X:", eyeX);
    layout->addRow("Eye Y:", eyeY);
    layout->addRow("Eye Z:", eyeZ);

    // Spin boxes for center point
    QDoubleSpinBox *centerX = new QDoubleSpinBox(this);
    QDoubleSpinBox *centerY = new QDoubleSpinBox(this);
    QDoubleSpinBox *centerZ = new QDoubleSpinBox(this);
    centerX->setObjectName("centerX");
    centerY->setObjectName("centerY");
    centerZ->setObjectName("centerZ");
    layout->addRow("Center X:", centerX);
    layout->addRow("Center Y:", centerY);
    layout->addRow("Center Z:", centerZ);

    QDialogButtonBox *buttonBox =
        new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    layout->addRow(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ViewPositionDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ViewPositionDialog::reject);
}

glm::vec3 ViewPositionDialog::getEyePosition() const
{
    double x = findChild<QDoubleSpinBox*>("eyeX")->value();
    double y = findChild<QDoubleSpinBox*>("eyeY")->value();
    double z = findChild<QDoubleSpinBox*>("eyeZ")->value();
    return glm::vec3(x, y, z);
}

glm::vec3 ViewPositionDialog::getCenterPoint() const
{
    double x = findChild<QDoubleSpinBox*>("centerX")->value();
    double y = findChild<QDoubleSpinBox*>("centerY")->value();
    double z = findChild<QDoubleSpinBox*>("centerZ")->value();
    return glm::vec3(x, y, z);
}
