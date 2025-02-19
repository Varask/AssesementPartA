#ifndef CUBEWIDGET_H
#define CUBEWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QTimer>
#include <QList>
#include <QPoint>
#include <QVector3D>
#include <QMatrix4x4>

class CubeWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit CubeWidget(QWidget *parent = nullptr);
    ~CubeWidget();

public slots:
    void toggleGloss();
    void setCustomRotation(const QVector3D &b, const QVector3D &d, float angle);
    void setViewPosition(const QVector3D &eye, const QVector3D &center);
    void resetDefault();
    void toggleAnimation();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void onAnimationTimer();
    void updateTexture();

private:
    QOpenGLShaderProgram shaderProgram;
    QOpenGLBuffer vbo { QOpenGLBuffer::VertexBuffer };
    QOpenGLVertexArrayObject vao;
    QMatrix4x4 projectionMatrix, viewMatrix, modelMatrix;
    QTimer *animationTimer;
    QTimer *textureTimer;
    bool animationEnabled;
    QList<QOpenGLTexture*> textures;
    int currentTextureIndex;
    QPoint lastMousePos;
    QVector3D camPos, camTarget;
    float cameraDistance;
    bool glossEnabled;
};

#endif // CUBEWIDGET_H
