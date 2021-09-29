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

private:
  GLuint grifon_tex[1];
  QGLShader *m_VertexShader;
  QGLShader *m_FragmentShader;
  QGLShaderProgram m_program;

  QVector<MyVertex> m_vertexes;
  QVector<GLuint> m_indexes;

  QOpenGLVertexArrayObject* m_VAO;
  QOpenGLBuffer* m_VBO;
  QOpenGLBuffer* m_EBO;

};
