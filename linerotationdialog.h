#ifndef LINEROTATIONDIALOG_H
#define LINEROTATIONDIALOG_H

#include <QDialog>
#include "C:\Users\varask\Desktop\Cranfield\Vizualization\libs\glm\glm\glm.hpp"

class LineRotationDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LineRotationDialog(QWidget *parent = nullptr);
    glm::vec3 getPointB() const;
    glm::vec3 getDirection() const;
    float getAngle() const;
};

#endif // LINEROTATIONDIALOG_H
