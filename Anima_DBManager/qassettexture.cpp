#include "qassettexture.h"

#include <QHBoxLayout>
#include "aatexture.h"
#include "db_manager.h"

#define PREVIEW_SIZE 22


QAssetTexture::QAssetTexture(QWidget *parent) :
    QWidget(parent)
{
    auto* myLayout = new QHBoxLayout();
    //myLayout->setAlignment(Qt::AlignCenter);
    myLayout->setContentsMargins(0,0,0,0);
    setLayout(myLayout);

    myTexturePreview = new QLabel();
    myTexturePreview->setMaximumSize(PREVIEW_SIZE, PREVIEW_SIZE);
    myTexturePreview->setMinimumSize(PREVIEW_SIZE, PREVIEW_SIZE);

    myAssetLabel = new QAssetLabel(AttributeTypeHelper::Type::Texture, "Texture", "Images (" + DB_Manager::GetDB_Manager().GetAAssetRegex(AttributeTypeHelper::Type::Texture) + "." + AATexture::GetStaticAssetFileExtension() +")", this);
    QObject::connect(myAssetLabel, &QAssetLabel::OnValueEdited, this, &QAssetTexture::RelayAssetLabelSignal);

    layout()->addWidget(myTexturePreview);
    layout()->addItem(new QSpacerItem(5,5));
    layout()->addWidget(myAssetLabel);

    setMinimumWidth(140);
}


void QAssetTexture::SetValue(const QString& _texturePath)
{
    myAssetLabel->SetValue(_texturePath);
    myTexturePreview->setPixmap(AATexture::GetPixmapFromAssetPath(_texturePath).scaled(32,32, Qt::AspectRatioMode::KeepAspectRatioByExpanding));
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

#undef PREVIEW_SIZE
