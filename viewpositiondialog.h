#ifndef VIEWPOSITIONDIALOG_H
#define VIEWPOSITIONDIALOG_H

#include <QDialog>
#include "C:\Users\varask\Desktop\Cranfield\Vizualization\libs\glm\glm\glm.hpp"

class ViewPositionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ViewPositionDialog(QWidget *parent = nullptr);
    glm::vec3 getEyePosition() const;
    glm::vec3 getCenterPoint() const;
};

#endif // VIEWPOSITIONDIALOG_H
