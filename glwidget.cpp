#include "glwidget.h"
#include <QOpenGLShaderProgram>

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
    animating(false),
    animAngle(0.0f),
    rotationMatrix(1.0f)
{
    connect(&timer, &QTimer::timeout, this, &GLWidget::updateAnimation);
    timer.start(16); // ~60 FPS
    resetView();
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);
    model = glm::mat4(1.0f);
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    float aspect = (h == 0 ? 1.0f : float(w) / h);
    projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
}

void GLWidget::paintGL()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Final transformation: projection * view * model * rotation
    glm::mat4 finalMatrix = projection * view * model * rotationMatrix;

    // Normally, send finalMatrix to your shader and draw a cube.
    // For brevity, actual drawing code is omitted.
}

void GLWidget::setLineRotation(const glm::vec3 &b, const glm::vec3 &d, float angle)
{
    glm::vec3 axis = glm::normalize(d);
    glm::mat4 T1 = glm::translate(glm::mat4(1.0f), -b);
    glm::mat4 R  = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
    glm::mat4 T2 = glm::translate(glm::mat4(1.0f), b);
    rotationMatrix = T2 * R * T1;
    update();
}

void GLWidget::setViewPosition(const glm::vec3 &eye, const glm::vec3 &center)
{
    view = glm::lookAt(eye, center, glm::vec3(0.0f, 1.0f, 0.0f));
    update();
}

void GLWidget::resetView()
{
    view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
                       glm::vec3(0.0f, 0.0f, 0.0f),
                       glm::vec3(0.0f, 1.0f, 0.0f));
    update();
}

void GLWidget::toggleAnimation()
{
    animating = !animating;
}

void GLWidget::updateAnimation()
{
    if (animating) {
        animAngle += 1.0f;
        if (animAngle > 360.0f)
            animAngle -= 360.0f;
        // Example: rotate about Y-axis continuously
        rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(animAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        update();
    }
}
