#include "qapmesh.h"

#include <QLabel>           // to remove later ?
#include <QOpenGLWidget>    //  ?
#include <QVBoxLayout>

#define RENDER_SIZE 512

QAPMesh::QAPMesh(QWidget *parent) :
    QAssetPreview(parent)
{
    auto* vLayout = layout();

    auto* myMeshRender = new QLabel("!!! DISPLAY IS ON TODOLIST FOR LATER !!!");
    myMeshRender->setMinimumSize(RENDER_SIZE, RENDER_SIZE);
    myMeshRender->setMaximumSize(RENDER_SIZE, RENDER_SIZE);
    myMeshRender->setFrameStyle(QFrame::Panel | QFrame::Plain);
    myMeshRender->setLineWidth(1);
    vLayout->addWidget(myMeshRender);
}

void QAPMesh::SetFilePath(const QString& _filePath)
{
    QAssetPreview::SetFilePath(_filePath);
}
