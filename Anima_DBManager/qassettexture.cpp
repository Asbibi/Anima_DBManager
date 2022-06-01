#include "qassettexture.h"

#include <QHBoxLayout>
#include <QPixmap>

QAssetTexture::QAssetTexture(QWidget *parent) :
    QWidget(parent)
{
    auto* myLayout = new QHBoxLayout();
    //myLayout->setAlignment(Qt::AlignCenter);
    myLayout->setContentsMargins(0,0,0,0);
    setLayout(myLayout);

    myTexturePreview = new QLabel();
    myTexturePreview->setMaximumSize(32,32);
    myTexturePreview->setMinimumSize(32,32);

    myAssetLabel = new QAssetLabel(Attribute::Type::Texture, "Texture", "Images (*.png)", this);
    QObject::connect(myAssetLabel, &QAssetLabel::OnValueEdited, this, &QAssetTexture::RelayAssetLabelSignal);

    layout()->addWidget(myTexturePreview);
    layout()->addItem(new QSpacerItem(5,5));
    layout()->addWidget(myAssetLabel);

    setMinimumWidth(140);
}


void QAssetTexture::SetValue(const QString& _texturePath)
{
    myAssetLabel->SetValue(_texturePath);
    myTexturePreview->setPixmap(QPixmap(_texturePath).scaled(32,32, Qt::AspectRatioMode::KeepAspectRatioByExpanding));
}


const QString& QAssetTexture::GetValue() const
{
    return myAssetLabel->GetValue();
}

const QAssetLabel* QAssetTexture::GetLabel() const
{
    return myAssetLabel;
}



void QAssetTexture::OpenFileDialog()
{
    myAssetLabel->OpenFileDialog();
}

void QAssetTexture::RelayAssetLabelSignal()
{
    SetValue(myAssetLabel->GetValue());
    emit OnValueEdited();
}
