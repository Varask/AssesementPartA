#include "cubewidget.h"
#include <QOpenGLShader>
#include <QPainter>
#include <QImage>
#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>

CubeWidget::CubeWidget(QWidget *parent)
    : QOpenGLWidget(parent),
    animationEnabled(false),
    currentTextureIndex(0),
    cameraDistance(3.0f),
    glossEnabled(true)
{
    resetDefault();
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &CubeWidget::onAnimationTimer);

    textureTimer = new QTimer(this);
    connect(textureTimer, &QTimer::timeout, this, &CubeWidget::updateTexture);
    textureTimer->start(700);
}

CubeWidget::~CubeWidget()
{
    makeCurrent();
    vbo.destroy();
    vao.destroy();
    qDeleteAll(textures);
    doneCurrent();
}

void CubeWidget::toggleGloss()
{
    glossEnabled = !glossEnabled;
    update();
}

void CubeWidget::setCustomRotation(const QVector3D &b, const QVector3D &d, float angle)
{
    QMatrix4x4 transToOrigin; transToOrigin.translate(-b);
    QMatrix4x4 rot; rot.rotate(angle, d.normalized());
    QMatrix4x4 transBack; transBack.translate(b);
    modelMatrix = transBack * rot * modelMatrix;
    update();
}

void CubeWidget::setViewPosition(const QVector3D &eye, const QVector3D &center)
{
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(eye, center, QVector3D(0, 1, 0));
    camPos = eye;
    camTarget = center;
    cameraDistance = eye.z();
    update();
}

void CubeWidget::resetDefault()
{
    cameraDistance = 3.0f;
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(QVector3D(0, 0, cameraDistance), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
    camPos = QVector3D(0, 0, cameraDistance);
    camTarget = QVector3D(0, 0, 0);
    modelMatrix.setToIdentity();
    update();
}

void CubeWidget::toggleAnimation()
{
    animationEnabled = !animationEnabled;
    if (animationEnabled)
        animationTimer->start(16);
    else
        animationTimer->stop();
}

void CubeWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.27f, 0.41f, 0.56f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    const char *vertexSrc = R"(
        #version 330 core
        layout(location = 0) in vec3 position;
        layout(location = 1) in vec3 normal;
        layout(location = 2) in vec2 texCoord;
        uniform mat4 mvp;
        uniform mat4 model;
        out vec3 fragPos;
        out vec3 fragNormal;
        out vec2 vTexCoord;
        void main(){
            vec4 worldPos = model * vec4(position, 1.0);
            fragPos = worldPos.xyz;
            fragNormal = mat3(transpose(inverse(model))) * normal;
            vTexCoord = texCoord;
            gl_Position = mvp * worldPos;
        }
    )";

    const char *fragmentSrc = R"(
        #version 330 core
        in vec3 fragPos;
        in vec3 fragNormal;
        in vec2 vTexCoord;
        uniform sampler2D textureSampler;
        uniform vec3 lightDir;
        uniform vec3 viewPos;
        uniform bool uGlossOn;
        out vec4 fragColor;
        void main(){
            vec4 baseColor = texture(textureSampler, vTexCoord);
            vec3 norm = normalize(fragNormal);
            vec3 light = normalize(-lightDir);
            vec3 ambient = 0.2 * baseColor.rgb;
            float diff = max(dot(norm, light), 0.0);
            vec3 diffuse = diff * baseColor.rgb;
            vec3 viewDir = normalize(viewPos - fragPos);
            vec3 reflectDir = reflect(-light, norm);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
            vec3 specular = vec3(0.0);
            if(uGlossOn) {
                float sum = baseColor.r + baseColor.g + baseColor.b;
                float glossFactor = smoothstep(1.1216, 1.8588, sum);
                specular = vec3(1.0) * spec * glossFactor * 0.5;
            }
            vec3 result = ambient + diffuse + specular;
            fragColor = vec4(result, 1.0);
        }
    )";

    shaderProgram.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexSrc);
    shaderProgram.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentSrc);
    shaderProgram.link();

    GLfloat vertices[] = {
        // Front face
        //      Position       Normal          Texcoords
        -0.5f, -0.5f,  0.5f,    0, 0, 1,    0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,     0, 0, 1,    1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,     0, 0, 1,    1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,     0, 0, 1,    1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    0, 0, 1,    0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    0, 0, 1,    0.0f, 0.0f,
        // Back face
        -0.5f, -0.5f, -0.5f,   0, 0, -1,   1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   0, 0, -1,   1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,   0, 0, -1,   0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,   0, 0, -1,   0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,   0, 0, -1,   0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   0, 0, -1,   1.0f, 0.0f,
        // Left face
        -0.5f,  0.5f,  0.5f,  -1, 0, 0,    1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  -1, 0, 0,    1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  -1, 0, 0,    0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  -1, 0, 0,    0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  -1, 0, 0,    0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  -1, 0, 0,    1.0f, 0.0f,
        // Right face
        0.5f,  0.5f,  0.5f,   1, 0, 0,    1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,   1, 0, 0,    0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,   1, 0, 0,    1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,   1, 0, 0,    0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,   1, 0, 0,    1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,   1, 0, 0,    0.0f, 0.0f,
        // Top face
        -0.5f,  0.5f, -0.5f,   0, 1, 0,    0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   0, 1, 0,    0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,   0, 1, 0,    1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,   0, 1, 0,    1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,   0, 1, 0,    1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   0, 1, 0,    0.0f, 1.0f,
        // Bottom face
        -0.5f, -0.5f, -0.5f,   0, -1, 0,   1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,   0, -1, 0,   0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,   0, -1, 0,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,   0, -1, 0,   0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,   0, -1, 0,   1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   0, -1, 0,   1.0f, 1.0f
    };

    vao.create();
    vao.bind();
    vbo.create();
    vbo.bind();
    vbo.allocate(vertices, sizeof(vertices));

    shaderProgram.bind();
    shaderProgram.enableAttributeArray(0);
    shaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, 8 * sizeof(GLfloat));
    shaderProgram.enableAttributeArray(1);
    shaderProgram.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(GLfloat), 3, 8 * sizeof(GLfloat));
    shaderProgram.enableAttributeArray(2);
    shaderProgram.setAttributeBuffer(2, GL_FLOAT, 6 * sizeof(GLfloat), 2, 8 * sizeof(GLfloat));

    vao.release();

    QImage fullImage(":/textures/textures/texture.png");
    if (fullImage.isNull())
        qDebug() << "Error loading texture";
    else {
        for (int i = 0; i < 3; ++i) {
            QImage sub = fullImage.copy(0, i * 16, 16, 16);
            QOpenGLTexture *tex = new QOpenGLTexture(sub.mirrored());
            tex->setMinificationFilter(QOpenGLTexture::Nearest);
            tex->setMagnificationFilter(QOpenGLTexture::Nearest);
            tex->setWrapMode(QOpenGLTexture::ClampToEdge);
            textures.append(tex);
        }
    }

    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(45.0f, float(width())/height(), 0.1f, 100.0f);
}

void CubeWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    projectionMatrix.setToIdentity();
    projectionMatrix.perspective(45.0f, float(w)/h, 0.1f, 100.0f);
}

void CubeWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QMatrix4x4 mvp = projectionMatrix * viewMatrix * modelMatrix;
    shaderProgram.bind();
    shaderProgram.setUniformValue("mvp", mvp);
    shaderProgram.setUniformValue("model", modelMatrix);
    shaderProgram.setUniformValue("viewPos", camPos);
    shaderProgram.setUniformValue("lightDir", QVector3D(0.0f, 0.0f, -1.0f));
    shaderProgram.setUniformValue("uGlossOn", glossEnabled);
    if (!textures.isEmpty()) {
        textures[currentTextureIndex]->bind(0);
        shaderProgram.setUniformValue("textureSampler", 0);
    }
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    vao.release();

    QPainter painter(this);
    painter.setPen(Qt::white);
    QQuaternion quat = QQuaternion::fromRotationMatrix(modelMatrix.toGenericMatrix<3,3>());
    QVector3D euler = quat.toEulerAngles();
    painter.drawText(10, 20, QString("Cube Rotation (pitch,yaw,roll): (%1, %2, %3)")
                                 .arg(euler.x(), 0, 'f', 2)
                                 .arg(euler.y(), 0, 'f', 2)
                                 .arg(euler.z(), 0, 'f', 2));
    painter.drawText(10, 40, QString("Camera Pos: (%1, %2, %3)")
                                 .arg(camPos.x(), 0, 'f', 2)
                                 .arg(camPos.y(), 0, 'f', 2)
                                 .arg(camPos.z(), 0, 'f', 2));
    painter.drawText(10, 60, QString("Camera Target: (%1, %2, %3)")
                                 .arg(camTarget.x(), 0, 'f', 2)
                                 .arg(camTarget.y(), 0, 'f', 2)
                                 .arg(camTarget.z(), 0, 'f', 2));
}

void CubeWidget::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->angleDelta().y() / 8;
    int numSteps = numDegrees / 15;
    cameraDistance -= numSteps * 0.5f;

    if (cameraDistance < 1.0f)
        cameraDistance = 1.0f;
    if (cameraDistance > 20.0f)
        cameraDistance = 20.0f;
    viewMatrix.setToIdentity();
    viewMatrix.lookAt(QVector3D(0,0,cameraDistance), QVector3D(0,0,0), QVector3D(0,1,0));
    camPos = QVector3D(0,0,cameraDistance);
    update();
}

void CubeWidget::mousePressEvent(QMouseEvent *event)
{
    lastMousePos = event->pos();
    if (animationEnabled) {
        animationTimer->stop();
        animationEnabled = false;
    }
}

void CubeWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPoint delta = event->pos() - lastMousePos;
    lastMousePos = event->pos();
    float angleX = delta.y();
    float angleY = delta.x();
    QMatrix4x4 manualRot;
    manualRot.rotate(angleX, QVector3D(1,0,0));
    manualRot.rotate(angleY, QVector3D(0,1,0));
    modelMatrix = manualRot * modelMatrix;
    update();
}

void CubeWidget::onAnimationTimer()
{
    modelMatrix.rotate(1.0f, QVector3D(0,1,0));
    update();
}

void CubeWidget::updateTexture()
{
    if (!textures.isEmpty()) {
        currentTextureIndex = (currentTextureIndex + 1) % textures.size();
        update();
    }
}
