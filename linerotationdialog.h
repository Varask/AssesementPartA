#ifndef LINEROTATIONDIALOG_H
#define LINEROTATIONDIALOG_H

#include <QDialog>
#include <QVector3D>

class QDoubleSpinBox;

class LineRotationDialog : public QDialog {
    Q_OBJECT
public:
    explicit LineRotationDialog(QWidget *parent = nullptr,
                                const QVector3D &defaultB = QVector3D(0,0,0),
                                const QVector3D &defaultD = QVector3D(0,0,1),
                                float defaultAngle = 0.0f);
    QVector3D getB() const;
    QVector3D getD() const;
    float getAngle() const;

private:
    QDoubleSpinBox *bx;
    QDoubleSpinBox *by;
    QDoubleSpinBox *bz;
    QDoubleSpinBox *dx;
    QDoubleSpinBox *dy;
    QDoubleSpinBox *dz;
    QDoubleSpinBox *angleSpin;
};

#endif // LINEROTATIONDIALOG_H
