#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include <C:/Users/varask/Desktop/Cranfield/Vizualization/libs/glm/glm/glm.hpp>
#include <C:/Users/varask/Desktop/Cranfield/Vizualization/libs/glm/glm/gtc/matrix_transform.hpp>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = nullptr);
    void setLineRotation(const glm::vec3 &b, const glm::vec3 &d, float angle);
    void setViewPosition(const glm::vec3 &eye, const glm::vec3 &center);
    void resetView();
    void toggleAnimation();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private slots:
    void updateAnimation();

private:
    QTimer timer;
    bool animating;
    float animAngle;

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    glm::mat4 rotationMatrix;
};

#endif // GLWIDGET_H
