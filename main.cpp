#include <QApplication>
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QMatrix4x4>
#include <QTimer>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QList>
#include <QImage>
#include <QVector3D>
#include <QDebug>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMouseEvent>
#include <QPainter>
#include <QQuaternion>
#include <QWheelEvent>
#include <QIcon>

//----------------------
// Dialog for line rotation
//----------------------
class LineRotationDialog : public QDialog {
    Q_OBJECT
public:
    // Ajout d'un constructeur avec valeurs par défaut
    LineRotationDialog(QWidget *parent = nullptr,
                       const QVector3D &defaultB = QVector3D(0,0,0),
                       const QVector3D &defaultD = QVector3D(0,0,1),
                       float defaultAngle = 0.0f) : QDialog(parent)
    {
        setWindowTitle("Line Rotation");
        QVBoxLayout *layout = new QVBoxLayout(this);

        QHBoxLayout *bLayout = new QHBoxLayout();
        bLayout->addWidget(new QLabel("b.x:"));
        bx = new QDoubleSpinBox(); bx->setRange(-100, 100); bx->setValue(defaultB.x()); bLayout->addWidget(bx);
        bLayout->addWidget(new QLabel("b.y:"));
        by = new QDoubleSpinBox(); by->setRange(-100, 100); by->setValue(defaultB.y()); bLayout->addWidget(by);
        bLayout->addWidget(new QLabel("b.z:"));
        bz = new QDoubleSpinBox(); bz->setRange(-100, 100); bz->setValue(defaultB.z()); bLayout->addWidget(bz);
        layout->addLayout(bLayout);

        QHBoxLayout *dLayout = new QHBoxLayout();
        dLayout->addWidget(new QLabel("d.x:"));
        dx = new QDoubleSpinBox(); dx->setRange(-100, 100); dx->setValue(defaultD.x()); dLayout->addWidget(dx);
        dLayout->addWidget(new QLabel("d.y:"));
        dy = new QDoubleSpinBox(); dy->setRange(-100, 100); dy->setValue(defaultD.y()); dLayout->addWidget(dy);
        dLayout->addWidget(new QLabel("d.z:"));
        dz = new QDoubleSpinBox(); dz->setRange(-100, 100); dz->setValue(defaultD.z()); dLayout->addWidget(dz);
        layout->addLayout(dLayout);

        QHBoxLayout *angleLayout = new QHBoxLayout();
        angleLayout->addWidget(new QLabel("Angle (deg):"));
        angleSpin = new QDoubleSpinBox(); angleSpin->setRange(-360, 360); angleSpin->setValue(defaultAngle); angleLayout->addWidget(angleSpin);
        layout->addLayout(angleLayout);

        QHBoxLayout *btnLayout = new QHBoxLayout();
        QPushButton *okBtn = new QPushButton("OK");
        QPushButton *cancelBtn = new QPushButton("Cancel");
        btnLayout->addWidget(okBtn);
        btnLayout->addWidget(cancelBtn);
        layout->addLayout(btnLayout);

        connect(okBtn, &QPushButton::clicked, this, &LineRotationDialog::accept);
        connect(cancelBtn, &QPushButton::clicked, this, &LineRotationDialog::reject);
    }
    QVector3D getB() const { return QVector3D(bx->value(), by->value(), bz->value()); }
    QVector3D getD() const { return QVector3D(dx->value(), dy->value(), dz->value()); }
    float getAngle() const { return angleSpin->value(); }
private:
    QDoubleSpinBox *bx, *by, *bz, *dx, *dy, *dz, *angleSpin;
};

//----------------------
// Dialog for view position
//----------------------
class ViewPositionDialog : public QDialog {
    Q_OBJECT
public:
    ViewPositionDialog(QWidget *parent = nullptr) : QDialog(parent) {
        setWindowTitle("View Position");
        QVBoxLayout *layout = new QVBoxLayout(this);

        QHBoxLayout *eyeLayout = new QHBoxLayout();
        eyeLayout->addWidget(new QLabel("Eye x:"));
        ex = new QDoubleSpinBox(); ex->setRange(-100, 100); eyeLayout->addWidget(ex);
        eyeLayout->addWidget(new QLabel("Eye y:"));
        ey = new QDoubleSpinBox(); ey->setRange(-100, 100); eyeLayout->addWidget(ey);
        eyeLayout->addWidget(new QLabel("Eye z:"));
        ez = new QDoubleSpinBox(); ez->setRange(-100, 100); eyeLayout->addWidget(ez);
        layout->addLayout(eyeLayout);

        QHBoxLayout *pointLayout = new QHBoxLayout();
        pointLayout->addWidget(new QLabel("Point x:"));
        px = new QDoubleSpinBox(); px->setRange(-100, 100); pointLayout->addWidget(px);
        pointLayout->addWidget(new QLabel("Point y:"));
        py = new QDoubleSpinBox(); py->setRange(-100, 100); pointLayout->addWidget(py);
        pointLayout->addWidget(new QLabel("Point z:"));
        pz = new QDoubleSpinBox(); pz->setRange(-100, 100); pointLayout->addWidget(pz);
        layout->addLayout(pointLayout);

        QHBoxLayout *btnLayout = new QHBoxLayout();
        QPushButton *okBtn = new QPushButton("OK");
        QPushButton *cancelBtn = new QPushButton("Cancel");
        btnLayout->addWidget(okBtn);
        btnLayout->addWidget(cancelBtn);
        layout->addLayout(btnLayout);

        connect(okBtn, &QPushButton::clicked, this, &ViewPositionDialog::accept);
        connect(cancelBtn, &QPushButton::clicked, this, &ViewPositionDialog::reject);
    }
    QVector3D getEye() const { return QVector3D(ex->value(), ey->value(), ez->value()); }
    QVector3D getPoint() const { return QVector3D(px->value(), py->value(), pz->value()); }
private:
    QDoubleSpinBox *ex, *ey, *ez, *px, *py, *pz;
};

//----------------------
// CubeWidget with zoom, texture animation, mouse control, lighting, gloss effect (toggle) and background color
//----------------------
class CubeWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT
public:
    CubeWidget(QWidget *parent = nullptr)
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
    ~CubeWidget() {
        makeCurrent();
        vbo.destroy();
        vao.destroy();
        qDeleteAll(textures);
        doneCurrent();
    }

public slots:
    // Toggle gloss effect on/off
    void toggleGloss() {
        glossEnabled = !glossEnabled;
        update();
    }

    // Apply custom rotation: modelMatrix = T(b)*R(angle,d)*T(-b)*modelMatrix
    void setCustomRotation(const QVector3D &b, const QVector3D &d, float angle) {
        QMatrix4x4 transToOrigin; transToOrigin.translate(-b);
        QMatrix4x4 rot; rot.rotate(angle, d.normalized());
        QMatrix4x4 transBack; transBack.translate(b);
        modelMatrix = transBack * rot * modelMatrix;
        update();
    }
    // Set view position and store camera info
    void setViewPosition(const QVector3D &eye, const QVector3D &center) {
        viewMatrix.setToIdentity();
        viewMatrix.lookAt(eye, center, QVector3D(0,1,0));
        camPos = eye;
        camTarget = center;
        cameraDistance = eye.z();
        update();
    }
    // Reset default: camera at (0,0,3) so that the front face is visible
    void resetDefault() {
        cameraDistance = 3.0f;
        viewMatrix.setToIdentity();
        viewMatrix.lookAt(QVector3D(0,0,cameraDistance), QVector3D(0,0,0), QVector3D(0,1,0));
        camPos = QVector3D(0,0,cameraDistance);
        camTarget = QVector3D(0,0,0);
        modelMatrix.setToIdentity();
        update();
    }
    // Toggle automatic rotation animation
    void toggleAnimation() {
        animationEnabled = !animationEnabled;
        if (animationEnabled)
            animationTimer->start(16);
        else
            animationTimer->stop();
    }
protected:
    void initializeGL() override {
        initializeOpenGLFunctions();
        glClearColor(0.27f, 0.41f, 0.56f, 1.0f);  // Couleur de fond #456990
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // Vertex shader : positions, normales, texCoord
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

        // Fragment shader avec éclairage Phong, gloss réglable et toggle
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
                    specular = vec3(1.0) * spec * glossFactor * 1.5; // Coefficient augmenté de 0.5 à 1.5
                }
                vec3 result = ambient + diffuse + specular;
                fragColor = vec4(result, 1.0);
            }
        )";

        shaderProgram.addShaderFromSourceCode(QOpenGLShader::Vertex, vertexSrc);
        shaderProgram.addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentSrc);
        shaderProgram.link();

        // Sommets du cube : position (vec3), normale (vec3), texCoord (vec2)
        // 8 floats par sommet
        GLfloat vertices[] = {
            // Front face (normal: 0,0,1)
            -0.5f, -0.5f,  0.5f,   0, 0, 1,    0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,   0, 0, 1,    1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,   0, 0, 1,    1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,   0, 0, 1,    1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,   0, 0, 1,    0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,   0, 0, 1,    0.0f, 0.0f,
            // Back face (normal: 0,0,-1)
            -0.5f, -0.5f, -0.5f,   0, 0, -1,   1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,   0, 0, -1,   1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,   0, 0, -1,   0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,   0, 0, -1,   0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,   0, 0, -1,   0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,   0, 0, -1,   1.0f, 0.0f,
            // Left face (normal: -1,0,0)
            -0.5f,  0.5f,  0.5f,  -1, 0, 0,    1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  -1, 0, 0,    1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  -1, 0, 0,    0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  -1, 0, 0,    0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  -1, 0, 0,    0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  -1, 0, 0,    1.0f, 0.0f,
            // Right face (normal: 1,0,0)
            0.5f,  0.5f,  0.5f,   1,0,0,     1.0f, 0.0f,
            0.5f, -0.5f, -0.5f,   1,0,0,     0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,   1,0,0,     1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,   1,0,0,     0.0f, 1.0f,
            0.5f,  0.5f,  0.5f,   1,0,0,     1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,   1,0,0,     0.0f, 0.0f,
            // Top face (normal: 0,1,0)
            -0.5f,  0.5f, -0.5f,   0,1,0,     0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,   0,1,0,     0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,   0,1,0,     1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,   0,1,0,     1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,   0,1,0,     1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,   0,1,0,     0.0f, 1.0f,
            // Bottom face (normal: 0,-1,0)
            -0.5f, -0.5f, -0.5f,   0,-1,0,    1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,   0,-1,0,    0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,   0,-1,0,    0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,   0,-1,0,    0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,   0,-1,0,    1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,   0,-1,0,    1.0f, 1.0f
        };

        vao.create();
        vao.bind();
        vbo.create();
        vbo.bind();
        vbo.allocate(vertices, sizeof(vertices));
        shaderProgram.bind();
        // Attribut 0 : position (3 floats), 1 : normal (3 floats), 2 : texCoord (2 floats)
        shaderProgram.enableAttributeArray(0);
        shaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, 8 * sizeof(GLfloat));
        shaderProgram.enableAttributeArray(1);
        shaderProgram.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(GLfloat), 3, 8 * sizeof(GLfloat));
        shaderProgram.enableAttributeArray(2);
        shaderProgram.setAttributeBuffer(2, GL_FLOAT, 6 * sizeof(GLfloat), 2, 8 * sizeof(GLfloat));
        vao.release();

        // Chargement de la texture depuis les ressources (votre fichier doit être référencé dans le .qrc)
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
    void resizeGL(int w, int h) override {
        glViewport(0, 0, w, h);
        projectionMatrix.setToIdentity();
        projectionMatrix.perspective(45.0f, float(w)/h, 0.1f, 100.0f);
    }
    void paintGL() override {
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

        // Affichage des informations sur le cube et la caméra
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
    // Zoom/Dézoom via la molette de la souris
    void wheelEvent(QWheelEvent *event) override {
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
    // Rotation manuelle par souris (désactive l'animation automatique)
    void mousePressEvent(QMouseEvent *event) override {
        lastMousePos = event->pos();
        if (animationEnabled) {
            animationTimer->stop();
            animationEnabled = false;
        }
    }
    void mouseMoveEvent(QMouseEvent *event) override {
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
private slots:
    void onAnimationTimer() {
        modelMatrix.rotate(1.0f, QVector3D(0,1,0));
        update();
    }
    void updateTexture() {
        currentTextureIndex = (currentTextureIndex + 1) % textures.size();
        update();
    }
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

//----------------------
// MainWindow with menu and state machine
//----------------------
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        // Ajout de l'icône pour la fenêtre (via le fichier qrc)
        setWindowIcon(QIcon(":/textures/textures/mine.png"));

        cubeWidget = new CubeWidget(this);
        setCentralWidget(cubeWidget);

        QMenuBar *menuBarPtr = this->menuBar();
        QMenu *menu = menuBarPtr->addMenu("Options");

        QAction *lineRotAct = new QAction("Line Rotation", this);
        QAction *viewPosAct = new QAction("View Position", this);
        QAction *defaultPosAct = new QAction("Default Position", this);
        QAction *animAct = new QAction("Animation", this);
        QAction *glossAct = new QAction("Toggle Gloss", this);

        menu->addAction(lineRotAct);
        menu->addAction(viewPosAct);
        menu->addAction(defaultPosAct);
        menu->addAction(animAct);
        menu->addAction(glossAct);

        connect(lineRotAct, &QAction::triggered, this, &MainWindow::onLineRotation);
        connect(viewPosAct, &QAction::triggered, this, &MainWindow::onViewPosition);
        connect(defaultPosAct, &QAction::triggered, cubeWidget, &CubeWidget::resetDefault);
        connect(animAct, &QAction::triggered, cubeWidget, &CubeWidget::toggleAnimation);
        connect(glossAct, &QAction::triggered, cubeWidget, &CubeWidget::toggleGloss);
    }
private slots:
    void onLineRotation() {
        // Pré-remplissage avec la position actuelle (ici, centre du cube) et direction par défaut
        LineRotationDialog dlg(this, QVector3D(0,0,0), QVector3D(0,0,1), 0.0f);
        if (dlg.exec() == QDialog::Accepted) {
            QVector3D b = dlg.getB();
            QVector3D d = dlg.getD();
            float angle = dlg.getAngle();
            cubeWidget->setCustomRotation(b, d, angle);
        }
    }
    void onViewPosition() {
        ViewPositionDialog dlg(this);
        if (dlg.exec() == QDialog::Accepted) {
            cubeWidget->setViewPosition(dlg.getEye(), dlg.getPoint());
        }
    }
private:
    CubeWidget *cubeWidget;
};

#include "main.moc"

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    MainWindow win;
    win.resize(800,600);
    win.show();
    return app.exec();
}
