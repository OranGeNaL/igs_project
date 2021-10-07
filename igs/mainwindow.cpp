#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGLFunctions>
#include <QOpenGLFunctions_3_3_Core>

MainWindow::MainWindow() : QGLWidget(){}

float angle = 0;
int deltaTime = 1;

bool showModelInfo(const aiScene* model){
    qDebug() << "\n\n************************";
    qDebug() << "  Imported model info";
    qDebug() << "************************";

    if(model->HasMeshes()){
        qDebug() << "Meshes amount: " << model->mNumMeshes;
    }else{
        qDebug() << "Meshes amount: 0";
        qDebug() << "Model invalid!";
        //return false;
    }

    qDebug() << "-------------";
    qDebug() << "Info per mash:";
    for(unsigned int meshNum = 0; meshNum < model->mNumMeshes; ++meshNum){
        qDebug() << "    "  << meshNum + 1 << ". Name:" << model->mMeshes[meshNum]->mName.C_Str();
        qDebug() << "         Faces:" << model->mMeshes[meshNum]->mNumFaces << ";";
        qDebug() << "         Primitive type:" << QString("0x" + QString::number(model->mMeshes[meshNum]->mPrimitiveTypes, 16)) << ";";
        qDebug() << "         Vertices:" << model->mMeshes[meshNum]->mNumVertices << ";";

        const unsigned short UVChanels = model->mMeshes[meshNum]->GetNumUVChannels();
        qDebug() << "         UVChanels:" << UVChanels << ";";
        for(unsigned short currentUVChanel = 0; currentUVChanel < UVChanels; ++currentUVChanel){
            qDebug() << "           " << currentUVChanel + 1 << "chanel:" << model->mMeshes[meshNum]->mNumUVComponents[currentUVChanel] << "components ;";
        }
    }

    return true;
}

void MainWindow::initializeGL(){
    const aiScene* scene = nullptr;
    Assimp::Importer importer;
    scene = importer.ReadFile("/home/orangeanl/Документы/griffin_animated.dae", NULL);
    if(scene == nullptr){
        qDebug() << "initializeGL::Reading file failed!\n";
        exit(EXIT_FAILURE);
    }

    if(!showModelInfo(scene)){
        exit(EXIT_FAILURE);
    }

    m_VertexShader = new QGLShader(QGLShader::Vertex);
    m_FragmentShader = new QGLShader(QGLShader::Fragment);

    if(!(m_VertexShader->compileSourceFile(/*"../GLSL/vertex.glsl"*/ "/home/orangeanl/Документы/igs_project/vertex.glsl") &&
         m_FragmentShader->compileSourceFile(/*"../GLSL/fragment.glsl"*/ "/home/orangeanl/Документы/igs_project/fragment.glsl")))
    {
        qDebug() << "initializeGL::Source files not found!";
        exit(EXIT_FAILURE);
    }

    m_program.addShader(m_VertexShader);
    m_program.addShader(m_FragmentShader);
    m_program.link();

    QGLFunctions glFunctions(context());

    //unsigned short currentMesh = 5;
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        if(scene->mNumMeshes != 0 && scene->mMeshes[i]->HasNormals()){
            //----- tmp -----------------
            MyVertex tmpVertex;
            aiVector3D* verteces = scene->mMeshes[i]->mVertices;
            aiVector3D* normals = scene->mMeshes[i]->mNormals;
            aiVector3D* texture_coords = scene->mMeshes[i]->mTextureCoords[0];
            //---------------------------
            for(unsigned int currentVertex = 0; currentVertex < scene->mMeshes[i]->mNumVertices; ++currentVertex){
                tmpVertex.x = verteces[currentVertex].x;
                tmpVertex.y = verteces[currentVertex].y;
                tmpVertex.z = verteces[currentVertex].z;

                tmpVertex.nx = normals[currentVertex].x;
                tmpVertex.ny = normals[currentVertex].y;
                tmpVertex.nz = normals[currentVertex].z;

                tmpVertex.s0 = texture_coords[currentVertex].x;
                tmpVertex.t0 = texture_coords[currentVertex].y;

                //tmpVertex.s0 = verteces[currentVertex].x;
                //tmpVertex.t0 = verteces[currentVertex].y;

                //qDebug() << verteces[currentVertex].x << verteces[currentVertex].y << verteces[currentVertex].z;


                m_vertexes.append(tmpVertex);
            }
        }

        aiFace* faces = scene->mMeshes[i]->mFaces;
        for(unsigned int currentFace = 0; currentFace < scene->mMeshes[i]->mNumFaces; ++currentFace){
            for(unsigned short currentFacePoint = 0; currentFacePoint < faces[currentFace].mNumIndices; ++currentFacePoint){
                m_indexes.append((GLuint)faces[currentFace].mIndices[currentFacePoint]);
            }
        }
    }

    qDebug() << "x: " << m_vertexes[1234].x << "y: " << m_vertexes[1234].y << "z: " << m_vertexes[1234].z;

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



    //uniform переменные в шейдере

    //цвет фона(uniform), блика(uniform) и диффузионный (vec3 + alpha)
    //позиция ист света через (uniform) и цвет фона(uniform), блика(uniform) и диффузионный (vec3 + alpha) для света
    //uniform позиция камеры
    //коэффициент фонга

    //реализовать фрагментный шейдер
    //
    //
    //
    //
    //

    QImage grifon_tex_image;

    grifon_tex_image.load("/home/orangeanl/Документы/igs_project/griffon_Diff.png");
    grifon_tex_image = QGLWidget::convertToGLFormat(grifon_tex_image);

    glGenTextures(1, grifon_tex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grifon_tex[0]);

    glTexImage2D(GL_TEXTURE_2D, 0, 3, (GLsizei)grifon_tex_image.width(), (GLsizei)grifon_tex_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, grifon_tex_image.bits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //glEnable(GL_TEXTURE_2D);
    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    //glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

//    glVertexPointer(3, GL_FLOAT, 0, cubeVertexArray);
//    glTexCoordPointer(2, GL_FLOAT, 0, cubeTextureArray);


    //glDrawElements(GL_TRIANGLES, m_vertexes.count(), GL_UNSIGNED_INT, 0);

}

void MainWindow::resizeGL(int width, int height){
    glViewport(0, 0, (GLint)width, (GLint)height);
}

void MainWindow::paintGL(){
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);


    m_program.bind();
    //установка юниформ переменных
    setUniformVariables();


    m_VAO->bind();
    glDrawElements(GL_TRIANGLES, m_indexes.size(), GL_UNSIGNED_INT, 0);
    m_VAO->release();

    glFlush();
    m_program.release();
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
     * МАНИПУЛЯЦИИ С МАТРИЦАМИ
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

