#include "qattributedisplay.h"
#include <QDebug>
#include "abool.h"
#include "aenumerator.h"
#include "aatexture.h"


QColor QAttributeDisplay::defaultColor = QColorConstants::White;
QColor QAttributeDisplay::nullColor = QColor(233, 218, 103);
QColor QAttributeDisplay::invalidColor = QColor(241, 136, 136);
QVariant QAttributeDisplay::noneCheckData = QVariant();

QAttributeDisplay::QAttributeDisplay()
{
    setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

bool QAttributeDisplay::IsRepresentingABool() const
{
    return myCurrentType == AttributeTypeHelper::Type::Bool;
}

bool QAttributeDisplay::AskUpdateContent(const Attribute* _attribute)
{
    if (_attribute == nullptr)
    {
        myCurrentType = AttributeTypeHelper::Type::Invalid;
        myCurrentValue = "";
        return true;
    }

    const AttributeTypeHelper::Type type = _attribute->GetType();
    const QString& value = _attribute->GetValueAsText();
    if (type == myCurrentType && value == myCurrentValue)
    {
        return false;
    }

    myCurrentType = type;
    myCurrentValue = value;
    return true;
}

void QAttributeDisplay::SetContentFromAttribute(const Attribute* _attribute)
{
    if (!AskUpdateContent(_attribute))
    {
        return;
    }
    UpdateContent(_attribute);
}

void QAttributeDisplay::UpdateContent(const Attribute* _attribute)
{
    setBackground(defaultColor);
    setForeground(QColorConstants::Black);
    setIcon(QIcon());
    setData(Qt::CheckStateRole, noneCheckData);

    QString textDisplayed = myCurrentValue;

    switch (myCurrentType)
    {
        case AttributeTypeHelper::Type::Bool:
        {
            const ABool* castedAttribute = dynamic_cast<const ABool*>(_attribute);
            Q_ASSERT(castedAttribute);
            setText(QString());
            setCheckState(castedAttribute->GetValue() ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);


            // End of process for Bool type : no text
            return;
        }

        case AttributeTypeHelper::Type::Enum:
        {
            const AEnumerator* castedAttribute = dynamic_cast<const AEnumerator*>(_attribute);
            Q_ASSERT(castedAttribute);

            const auto* attrEnum = castedAttribute->GetEnum();
            Q_ASSERT(attrEnum);

            if (attrEnum->GetUseColor())
            {
                int enumValue = castedAttribute->GetEnumValue();
                setForeground(attrEnum->GetColor(enumValue));
            }
            break;
        }

        case AttributeTypeHelper::Type::Texture:
        {
            const AATexture* castedAttribute = dynamic_cast<const AATexture*>(_attribute);
            Q_ASSERT(castedAttribute);
            setIcon(QIcon(AATexture::GetPixmapFromAssetPath(castedAttribute->GetValueAsText())));
        }
        case AttributeTypeHelper::Type::Mesh:
        case AttributeTypeHelper::Type::Niagara:
        case AttributeTypeHelper::Type::Sound:
        case AttributeTypeHelper::Type::UAsset:
        {
            const AAsset* castedAttribute = dynamic_cast<const AAsset*>(_attribute);
            Q_ASSERT(castedAttribute);
            QColor colorToUse = defaultColor;
            if (castedAttribute->IsEmpty())
            {
                colorToUse = nullColor;
            }
            else if (castedAttribute->IsDirty())
            {
                colorToUse = invalidColor;
                textDisplayed.remove(0,1);
            }
            setBackground(colorToUse);
            break;
        }
        default:
        {}
    }
    setText(textDisplayed);
}

QTableWidgetItem* QAttributeDisplay::clone() const
{
    return new QAttributeDisplay();    // QTableWidgetItem::clone();//
}

