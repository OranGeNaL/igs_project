#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGLFunctions>
#include <QOpenGLFunctions_3_3_Core>

MainWindow::MainWindow() : QGLWidget(){}

float angle = 0;
int deltaTime = 1;

float speed = 0.003;
float sensitivity = 0.05;

void MainWindow::ReadIndexesFromFile(QVector<GLuint>* indexes, QString fileName)
{
    QFile file(fileName);

    file.open(QIODevice::ReadOnly);

    foreach (QString i,QString(file.readAll()).split(QRegExp("[\r\n]"),QString::SkipEmptyParts)){
        GLuint tmpIndex;
        tmpIndex = i.toInt();
//        qDebug() << tmpIndex;

        indexes->append(tmpIndex);
    }

    file.close();
}

void MainWindow::ReadVertexesFromFile(QVector<MyVertex>* vertexes, QString fileName)
{
    QFile file(fileName);

    file.open(QIODevice::ReadOnly);

    QStringList strings = QString(file.readAll()).split(QRegExp("[\r\n]"));

    for(int i = 0; i < strings.count() - 1; i += 8)
    {
        MyVertex tmpVertex;
        tmpVertex.x = strings[i].toFloat();
        tmpVertex.y = strings[i + 1].toFloat();
        tmpVertex.z = strings[i + 2].toFloat();
        tmpVertex.nx = strings[i + 3].toFloat();
        tmpVertex.ny = strings[i + 4].toFloat();
        tmpVertex.nz = strings[i + 5].toFloat();
        tmpVertex.s0 = strings[i + 6].toFloat();
        tmpVertex.t0 = strings[i + 7].toFloat();


        vertexes->append(tmpVertex);
    }

    file.close();
}

void MainWindow::initializeGL(){

    m_VertexShader = new QGLShader(QGLShader::Vertex);
    m_FragmentShader = new QGLShader(QGLShader::Fragment);

    if(!(m_VertexShader->compileSourceFile("/Users/orangenal/Documents/igs_project/vertex.glsl") &&
            m_FragmentShader->compileSourceFile("/Users/orangenal/Documents/igs_project/fragment.glsl")))
       {
           qDebug() << "initializeGL::Source files not found!";
           exit(EXIT_FAILURE);
       }

    m_program.addShader(m_VertexShader);
    m_program.addShader(m_FragmentShader);
    m_program.link();

    QGLFunctions glFunctions(context());


    ReadIndexesFromFile(&m_indexes, "/Users/orangenal/Documents/igs_project/griffin_animated_dae-indexes.txt");
    ReadVertexesFromFile(&m_vertexes, "/Users/orangenal/Documents/igs_project/griffin_animated_dae-vertexes.txt");

    qDebug() << "\n\nVertices readed:" << m_vertexes.size();
    qDebug() << "Indexes readed:" << m_indexes.size();


    glEnable(GL_DEPTH_TEST);

    m_VAO = new QOpenGLVertexArrayObject();
    m_VBO = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    m_EBO = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

    m_VAO->create();
    m_VBO->create();
    m_EBO->create();

    m_VAO->bind();

    m_VBO->bind();
    m_VBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_VBO->allocate(m_vertexes.data(), m_vertexes.size() * sizeof(MyVertex));

    m_EBO->bind();
    m_EBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_EBO->allocate(m_indexes.data(), m_indexes.size() * sizeof(GLuint));


    int vertexBlock = m_program.attributeLocation("glPoint");
    m_program.enableAttributeArray(vertexBlock);
    m_program.setAttributeBuffer(vertexBlock, GL_FLOAT, 0, 3, sizeof(MyVertex));

    int normalBlock = m_program.attributeLocation("glNormal");
    m_program.enableAttributeArray(normalBlock);
    m_program.setAttributeBuffer(normalBlock, GL_FLOAT, offsetof(MyVertex, nx), 3, sizeof(MyVertex));

    int textureBlock = m_program.attributeLocation("glTextureCoord");
    m_program.enableAttributeArray(textureBlock);
    m_program.setAttributeBuffer(textureBlock, GL_FLOAT, offsetof(MyVertex, s0), 2, sizeof(MyVertex));

    m_VAO->release();

    glMatrixMode(GL_MODELVIEW);
    glOrtho(-3, 3, -3, 3, -3, 3);

    QImage grifon_tex_image;

    grifon_tex_image.load("/Users/orangenal/Documents/igs_project/griffon_Diff.png");
    grifon_tex_image = QGLWidget::convertToGLFormat(grifon_tex_image);

    glGenTextures(1, grifon_tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grifon_tex[0]);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei)grifon_tex_image.width(), (GLsizei)grifon_tex_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, grifon_tex_image.bits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    startTimer(10);

}

void MainWindow::resizeGL(int width, int height){
    glViewport(0, 0, (GLint)width, (GLint)height);
}

void MainWindow::paintGL(){
    QElapsedTimer timer;
    timer.start();

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    Move();
    Rotate();

    m_program.bind();
    //?????????????????? ?????????????? ????????????????????
    setUniformVariables();


    m_VAO->bind();
    glDrawElements(GL_TRIANGLES, m_indexes.size(), GL_UNSIGNED_INT, 0);
    m_VAO->release();

    glFlush();
    m_program.release();

    deltaTime = timer.elapsed();
}

void MainWindow::setUniformVariables()
{
    m_program.setUniformValue("lightSourcePosition", shaderVariables.lightSourcePosition);

    m_program.setUniformValue("ambientLightColor", shaderVariables.ambientLightColor);
    m_program.setUniformValue("ambientMaterialColor", shaderVariables.ambientMaterialColor);
    m_program.setUniformValue("ambientLightIntensivity", shaderVariables.ambientLightIntensivity);

    m_program.setUniformValue("diffuseLightColor", shaderVariables.diffuseLightColor);
    m_program.setUniformValue("diffuseMaterialColor", shaderVariables.diffuseMaterialColor);

    m_program.setUniformValue("specularLightColor", shaderVariables.specularLightColor);
    m_program.setUniformValue("specularMaterialColor", shaderVariables.specularMaterialColor);

    m_program.setUniformValue("materialShiness", shaderVariables.materialShiness);

    m_program.setUniformValue("viewerPosition", shaderVariables.viewerPosition);

//    m_program.setUniformValue("cameraPosition", shaderVariables.cameraPosition);
//    m_program.setUniformValue("cameraFront", shaderVariables.cameraFront);
//    m_program.setUniformValue("cameraUp", shaderVariables.cameraUp);


    /*
     * ?????????????????????? ?? ??????????????????
     * */
    shaderVariables.modelMatrix.setToIdentity();
    shaderVariables.modelMatrix.translate(QVector3D(0.0, 0.0, 0.0));
    shaderVariables.modelMatrix.scale(QVector3D(0.15, 0.15, 0.15));
    shaderVariables.modelMatrix.rotate(90.0, QVector3D(-1.0, 0.0, 0.0));
    shaderVariables.modelMatrix.rotate(angle, QVector3D(0.0, 0.0, 1.0));
    m_program.setUniformValue("modelMatrix", shaderVariables.modelMatrix);

    shaderVariables.viewMatrix.setToIdentity();
    shaderVariables.viewMatrix.lookAt(shaderVariables.viewerPosition, shaderVariables.viewerPosition + shaderVariables.viewerFront, shaderVariables.viewerUp);
    m_program.setUniformValue("viewMatrix", shaderVariables.viewMatrix);

    shaderVariables.projectionMatrix.setToIdentity();
    shaderVariables.projectionMatrix.perspective(45.0, (GLfloat)width()/(GLfloat)height(), 0.1, 100.0);
    m_program.setUniformValue("projectionMatrix", shaderVariables.projectionMatrix);

}

MainWindow::~MainWindow(){
    delete m_VertexShader;
    delete m_FragmentShader;
}


void MainWindow::timerEvent(QTimerEvent*){
    angle += 0.5;
    //GLfloat camX = std::sin(angle/16.0) * 3.0;
    //GLfloat camZ = std::cos(angle/16.0) * 3.0;
    //m_vars.viewer_position = QVector3D(camX, 0.0, camZ);
    QPaintEvent ev(rect());
    paintEvent(&ev);
}

void MainWindow::keyPressEvent(QKeyEvent* event){
    switch(event->key()){
    case Qt::Key::Key_W:
        m_move[moveUp] = true;
        break;
    case Qt::Key::Key_S:
        m_move[moveDown] = true;
        break;
    case Qt::Key::Key_D:
        m_move[moveRight] = true;
        break;
    case Qt::Key::Key_A:
        m_move[moveLeft] = true;
        break;
    case Qt::Key::Key_Up:
        m_rotate[rotateUp] = true;
        break;
    case Qt::Key::Key_Down:
        m_rotate[rotateDown] = true;
        break;
    case Qt::Key::Key_Right:
        m_rotate[rotateRight] = true;
        break;
    case Qt::Key::Key_Left:
        m_rotate[rotateLeft] = true;
        break;
    default:
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* event){
    switch(event->key()){
    case Qt::Key::Key_W:
        m_move[moveUp] = false;
        break;
    case Qt::Key::Key_S:
        m_move[moveDown] = false;
        break;
    case Qt::Key::Key_D:
        m_move[moveRight] = false;
        break;
    case Qt::Key::Key_A:
        m_move[moveLeft] = false;
        break;
    case Qt::Key::Key_Up:
        m_rotate[rotateUp] = false;
        break;
    case Qt::Key::Key_Down:
        m_rotate[rotateDown] = false;
        break;
    case Qt::Key::Key_Right:
        m_rotate[rotateRight] = false;
        break;
    case Qt::Key::Key_Left:
        m_rotate[rotateLeft] = false;
        break;
    default:
        break;
    }
}

void MainWindow::Move(){
    GLfloat roundedSpeed = speed * deltaTime;
    if(m_move[moveUp]){
        shaderVariables.viewerPosition += roundedSpeed * shaderVariables.viewerFront;
    }
    if(m_move[moveDown]){
        shaderVariables.viewerPosition -= roundedSpeed * shaderVariables.viewerFront;
    }
    if(m_move[moveLeft]){
        QVector3D tmp(QVector3D::crossProduct(shaderVariables.viewerFront, shaderVariables.viewerUp));
        tmp.normalize();
        shaderVariables.viewerPosition -= tmp * roundedSpeed;
    }
    if(m_move[moveRight]){
        QVector3D tmp(QVector3D::crossProduct(shaderVariables.viewerFront, shaderVariables.viewerUp));
        tmp.normalize();
        shaderVariables.viewerPosition += tmp * roundedSpeed;
    }
}

void MainWindow::Rotate(){
    float roundedSensetivity = sensitivity * deltaTime;
    if(m_rotate[rotateUp]){
        if(shaderVariables.viewerPitch < 89.0)
            shaderVariables.viewerPitch += roundedSensetivity;
    }
    if(m_rotate[rotateDown]){
        if(shaderVariables.viewerPitch > -89.0)
            shaderVariables.viewerPitch -= roundedSensetivity;
    }
    if(m_rotate[rotateLeft]){
        shaderVariables.viewerYaw -= roundedSensetivity;
    }
    if(m_rotate[rotateRight]){
        shaderVariables.viewerYaw += roundedSensetivity;
    }

    QVector3D tmp;
    tmp.setX(std::cos(qDegreesToRadians(shaderVariables.viewerPitch)) * std::cos(qDegreesToRadians(shaderVariables.viewerYaw)));
    tmp.setY(std::sin(qDegreesToRadians(shaderVariables.viewerPitch)));
    tmp.setZ(std::cos(qDegreesToRadians(shaderVariables.viewerPitch)) * std::sin(qDegreesToRadians(shaderVariables.viewerYaw)));
    tmp.normalize();
    shaderVariables.viewerFront = tmp;
}

