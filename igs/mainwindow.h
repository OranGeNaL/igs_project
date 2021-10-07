#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#endif

#include <QMainWindow>
#include <QGLWidget>
#include <QGLShader>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
//#include <GL/gl.h>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>

typedef struct MyVertex
  {
    float x, y, z;
    float nx, ny, nz;
    float s0, t0;
  } TMyVertex;



struct ShaderVariables
{
    QVector3D lightSourcePosition = QVector3D(0.0, 0.0, 10.0);

    QVector3D ambientLightColor = QVector3D(1.0, 1.0, 1.0);
    QVector3D ambientMaterialColor = QVector3D(0.2, 0.2, 0.2);
    GLfloat ambientLightIntensivity = 0.1;

    QVector3D diffuseLightColor = QVector3D(0.5, 0.5, 0.5);
    QVector3D diffuseMaterialColor = QVector3D(0.8, 0.8, 0.8);

    QVector3D specularLightColor = QVector3D(0.5, 0.5, 0.5);
    QVector3D specularMaterialColor = QVector3D(0.5, 0.5, 0.5);

    GLfloat materialShiness = 10.0;

    QVector3D viewerPosition = QVector3D(0, 0, 1.0);
    QVector3D viewerFront = QVector3D(0, 0, -1.0);
    QVector3D viewerTarget = QVector3D(0, 0, 0);
    QVector3D viewerUp = QVector3D(0, 1.0, 0);

    GLfloat viewerYaw = -90;
    GLfloat viewerPitch = 0;


//    QVector3D cameraPosition = QVector3D(1.0, 0, 0);
//    QVector3D cameraFront = QVector3D(0, 0, -1.0);
//    QVector3D cameraUp = QVector3D(0, 1.0, 0);

    QMatrix4x4 modelMatrix;
    QMatrix4x4 viewMatrix;
    QMatrix4x4 projectionMatrix;
    QMatrix4x4 lookAtMatrix;

//    QMatrix4x4 xRotationMatrix = QMatrix4x4(1.0, 0.0, 0.0, 0.0,
//                                            0.0, 1.0, 0.0, 0.0,
//                                            0.0, 0.0, 1.0, 0.0,
//                                            0.0, 0.0, 0.0, 1.0);

//    QMatrix4x4 zRotationMatrix = QMatrix4x4(1.0, 0.0, 0.0, 0.0,
//                                            0.0, 1.0, 0.0, 0.0,
//                                            0.0, 0.0, 1.0, 0.0,
//                                            0.0, 0.0, 0.0, 1.0);
};



#define BUFFER_OFFSET(i) ((char *)NULL + (i))

class MainWindow : public QGLWidget
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

protected:
  void initializeGL();
  void resizeGL(int, int);
  void paintGL();
  void setUniformVariables();

private:
  GLuint grifon_tex[1];

  QGLShader *m_VertexShader;
  QGLShader *m_FragmentShader;
  QGLShaderProgram m_program;
  ShaderVariables shaderVariables;

  QVector<MyVertex> m_vertexes;
  QVector<GLuint> m_indexes;

  QOpenGLVertexArrayObject* m_VAO;
  QOpenGLBuffer* m_VBO;
  QOpenGLBuffer* m_EBO;

};
