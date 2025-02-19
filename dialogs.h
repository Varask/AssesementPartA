#ifndef DIALOGS_H
#define DIALOGS_H

#include <QDialog>
#include <QVector3D>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

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
    QDoubleSpinBox *bx, *by, *bz, *dx, *dy, *dz, *angleSpin;
};

class ViewPositionDialog : public QDialog {
    Q_OBJECT
public:
    explicit ViewPositionDialog(QWidget *parent = nullptr);
    QVector3D getEye() const;
    QVector3D getPoint() const;
private:
    QDoubleSpinBox *ex, *ey, *ez, *px, *py, *pz;
};

#endif // DIALOGS_H
