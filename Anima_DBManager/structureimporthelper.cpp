#include "structureimporthelper.h"

#include <QMap>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include "db_manager.h"


bool StructureImportHelper::DecomposeCSVString(QString _csvLineCopy, int _expectedCountWithKey, QStringList& _outList, bool _removeKey)
{
    int firstComma = _csvLineCopy.indexOf(',');
    if (firstComma == -1)
    {
        return false;
    }

    _csvLineCopy.remove(_csvLineCopy.length()-1,1);
    _csvLineCopy.replace(firstComma, 1, "\",");
    _outList = _csvLineCopy.split("\",\"");
    if (_outList.count() != _expectedCountWithKey)
    {
        return false;
    }

    if (_removeKey)
    {
        _outList.removeFirst();
    }

    return true;
}

QStringList StructureImportHelper::DecomposeCPPFile(const QString& _cppFileContent)
{
    QStringList addedStructNames = QStringList();

    QString cleanCppFile = _cppFileContent;
    cleanCppFile.replace("\r\n", "\n");
    cleanCppFile = StructureImportHelper::RemoveCommentBlocks(cleanCppFile);
    cleanCppFile = StructureImportHelper::RemoveCommentLines(cleanCppFile);
    QStringList structSections = cleanCppFile.split("USTRUCT(");
    if (structSections.isEmpty())
    {
        return addedStructNames;
    }

    structSections.removeFirst();
    QMap<QString,QString> structTableSections = QMap<QString,QString>();    // Key is the struct name
    QMap<QString,QString> otherStructSections = QMap<QString,QString>();
    for (const auto& structSection : structSections)
    {
        (structSection.contains(": public FTableRowBase") ? structTableSections : otherStructSections)
            .insert(
                StructureImportHelper::GetStructName(structSection),
                StructureImportHelper::GetBracketContent(structSection));
    }

    for (const auto& structContent : structTableSections)
    {
        QString structName = structTableSections.key(structContent);
        structName.removeFirst();
        TemplateStructure structureTemplate = TemplateStructure(structName);

        StructureImportHelper::GetStructureTemplateFromCppString(structContent, otherStructSections, structureTemplate);

        auto& db = DB_Manager::GetDB_Manager();
        db.AddStructureDB(structureTemplate);
        addedStructNames.append(db.GetStructureTable(db.GetStructuresCount() -1)->GetTemplateName());
    }

    return addedStructNames;
}
QString StructureImportHelper::RemoveCommentBlocks(const QString& _text)
{
    QString valuesWithouCommentBlocks = _text;
    static const QRegularExpression commentBlockRegex = QRegularExpression(R"(/\*.*?\*/)", QRegularExpression::DotMatchesEverythingOption);
    valuesWithouCommentBlocks = valuesWithouCommentBlocks.replace(commentBlockRegex, "");

    int startOfCommentBlock = valuesWithouCommentBlocks.indexOf("/*");
    if (startOfCommentBlock != -1)
    {
        valuesWithouCommentBlocks = valuesWithouCommentBlocks.left(startOfCommentBlock);
    }

    return valuesWithouCommentBlocks;
}
QString StructureImportHelper::RemoveCommentLines(const QString& _text)
{
    QString valuesWithouCommentBlocks = _text;
    static const QRegularExpression commentLineRegex = QRegularExpression(R"(\/\/.*)");
    return valuesWithouCommentBlocks.replace(commentLineRegex, "");
}
QString StructureImportHelper::GetStructName(const QString& _text)
{
    static const QRegularExpression regex(R"(struct\s(\S*\s)?(F\w+)(?=[\n{:]| \:| \{))");
    QRegularExpressionMatch match = regex.match(_text);

    if (match.hasMatch())
    {
        return  match.captured(2); // Group 2 contains "FStruct"
    }

    return QString(); // No match found
}
QString StructureImportHelper::GetBracketContent(const QString& _text)
{
    int openBracketIndex = _text.indexOf('{');
    if (openBracketIndex == -1)
    {
        return QString();
    }

    int bLevel = 1; // Track bracket levels
    int i = openBracketIndex + 1;
    while ((i < _text.length()) && (bLevel > 0))
    {
        if (_text[i] == '{') bLevel++;
        else if (_text[i] == '}') bLevel--;
        i++;
    }

    if (bLevel == 0)
    {
        // Successfully found matching closing '}'
        return _text.mid(openBracketIndex + 1, i - openBracketIndex - 2);
    }

    return QString(); // No matching closing bracket found
}


void StructureImportHelper::GetTemplateAttributeRepresentation(const QString& _structCppContent, const QList<QString>& _otherStructKeys, QList<AttributeTemplateRepresentation>& _outTemplRepresentations)
{
    _outTemplRepresentations.clear();

    // Remove line breaks between UPROPERTY definitions
    QString contentRealigned = _structCppContent;
    int startPos = 0;
    static const QString upropIndicator = "UPROPERTY(";
    while ((startPos = contentRealigned.indexOf(upropIndicator, startPos)) != -1) {
        int endPos = contentRealigned.indexOf(';', startPos);
        if (endPos != -1) {
            QString block = contentRealigned.mid(startPos, endPos - startPos + 1);
            block.replace("\n", " ");
            contentRealigned.replace(startPos, endPos - startPos + 1, block);

            startPos = endPos + 1;
        } else {
            break;
        }
    }

    // Handle each line individually
    QStringList lines = contentRealigned.split('\n', Qt::SkipEmptyParts);
    static const QRegularExpression upropRegex(R"(\bUPROPERTY\([^)]*\))");
    const QString& attPrefix = DB_Manager::GetDB_Manager().GetAttributePrefix();
    const QString& attSuffix = DB_Manager::GetDB_Manager().GetAttributeSuffix();
    for (const auto& line : lines)
    {
        if (!line.contains(upropIndicator) || line.last(1) != ";")
        {
            continue;
        }

        QString cleanedLine = line;
        cleanedLine.replace(upropRegex, "");
        cleanedLine.replace('\t', "");
        cleanedLine = cleanedLine.trimmed();
        cleanedLine.removeLast();

        QStringList defaultSeparatedLine = cleanedLine.split('=', Qt::SkipEmptyParts);
        QString defaultValueStr = "";
        switch (defaultSeparatedLine.count())
        {
        case 1:
            break;
        case 2:
            defaultValueStr = defaultSeparatedLine[1].trimmed();
            break;
        default:
            continue;
        }

        QStringList attributeDeclaration = defaultSeparatedLine[0].split(' ', Qt::SkipEmptyParts);
        if (attributeDeclaration.count() != 2)
        {
            continue;
        }

        AttributeTemplateRepresentation templateAtributeRep = AttributeTemplateRepresentation();
        templateAtributeRep.name = attributeDeclaration[1];
        if (templateAtributeRep.name.startsWith(attPrefix))
        {
            templateAtributeRep.name.remove(0, attPrefix.length());
        }
        if (templateAtributeRep.name.endsWith(attSuffix))
        {
            templateAtributeRep.name.chop(attSuffix.length());
        }
        templateAtributeRep.cppType = attributeDeclaration[0];
        StructureImportHelper::GetDbTypeFromCpp(_otherStructKeys, templateAtributeRep);


        // Handle the default value
        if (!defaultValueStr.isEmpty())
        {
            if (templateAtributeRep.dbType == AttributeTypeHelper::Type::Bool ||
                templateAtributeRep.dbType == AttributeTypeHelper::Type::Int ||
                templateAtributeRep.dbType == AttributeTypeHelper::Type::ShortString)
            {
                templateAtributeRep.defaultValue = defaultValueStr;
            }
            else if (templateAtributeRep.dbType == AttributeTypeHelper::Type::Enum)
            {
                QString defaultValStart = templateAtributeRep.cppType + "::";
                if (defaultValueStr.startsWith(defaultValStart))
                {
                    templateAtributeRep.defaultValue = defaultValueStr;
                    templateAtributeRep.defaultValue.remove(0, defaultValStart.length());
                }
            }
            else if (templateAtributeRep.dbType == AttributeTypeHelper::Type::Float)
            {
                templateAtributeRep.defaultValue = defaultValueStr;
                templateAtributeRep.defaultValue.replace("f", "");
            }
        }


        qDebug() << templateAtributeRep.name << '-'
                 << templateAtributeRep.cppType << '.' << AttributeTypeHelper::TypeToString(templateAtributeRep.dbType)
                 << '|' << templateAtributeRep.active
                 << '|' << defaultValueStr;

        _outTemplRepresentations.append(templateAtributeRep);
    }
}
void StructureImportHelper::GetDbTypeFromCpp(const QList<QString>& _otherStructKeys, AttributeTemplateRepresentation& _outAttrRep)
{
    _outAttrRep.dbType = StructureImportHelper::GetTypeFromCppString(_outAttrRep.cppType);
    if (_outAttrRep.dbType == AttributeTypeHelper::Type::Array)
    {
        int start = _outAttrRep.cppType.indexOf("<");
        int end = _outAttrRep.cppType.indexOf(">", start);

        if (start != -1 && end != -1 && start < end) {
            QString nestedType = _outAttrRep.cppType.mid(start + 1, end - start - 1);
            _outAttrRep.subArryDbType = StructureImportHelper::GetTypeFromCppString(nestedType);

            // if the nested type is invalid or an array abort
            if (_outAttrRep.subArryDbType == AttributeTypeHelper::Type::Invalid ||
                _outAttrRep.subArryDbType == AttributeTypeHelper::Type::Array)
            {
                _outAttrRep.dbType = AttributeTypeHelper::Type::Invalid;
            }
            else
            {
                QString nestedName = _outAttrRep.name;
                if (nestedName.endsWith("s"))
                {
                    nestedName.removeLast();
                }

                if (!StructureImportHelper::ValidateEnumStructStringTypeAndGetSubType(_otherStructKeys, _outAttrRep.subArryDbType, nestedType, nestedName, _outAttrRep.subArrySubRefStructOrEnumName))
                {
                    _outAttrRep.dbType = AttributeTypeHelper::Type::Invalid;
                }
            }
        }
        else
        {
            // Array without nested type -> invalid
            _outAttrRep.dbType = AttributeTypeHelper::Type::Invalid;
        }
    }
    if (!StructureImportHelper::ValidateEnumStructStringTypeAndGetSubType(_otherStructKeys, _outAttrRep.dbType, _outAttrRep.cppType, _outAttrRep.name, _outAttrRep.subRefStructOrEnumName))
    {
        _outAttrRep.dbType = AttributeTypeHelper::Type::Invalid;
    }


    // Make unidentified type inactie Boolean attributes
    if (_outAttrRep.dbType == AttributeTypeHelper::Type::Invalid)
    {
        _outAttrRep.dbType = AttributeTypeHelper::Type::Bool;
        _outAttrRep.active = false;
    }
}
AttributeTypeHelper::Type StructureImportHelper::GetTypeFromCppString(const QString& _typeCppString)
{
    static const QRegularExpression enumRegex(R"(^E[A-Z])");
    static const QRegularExpression structRegex(R"(^F[A-Z])");

    if (_typeCppString == "bool")
    {
        return AttributeTypeHelper::Type::Bool;
    }
    else if (_typeCppString == "int")
    {
        return AttributeTypeHelper::Type::Int;
    }
    else if (_typeCppString == "float")
    {
        return AttributeTypeHelper::Type::Float;
    }
    else if (_typeCppString == "FName" || _typeCppString == "FString" || _typeCppString == "FText")
    {
        return AttributeTypeHelper::Type::ShortString;
    }
    else if (_typeCppString == "FStringId") // TODO : make it a project property
    {
        return AttributeTypeHelper::Type::TableString;
    }
    else if (_typeCppString.startsWith("TArray<"))
    {
        return AttributeTypeHelper::Type::Array;
    }
    else if (_typeCppString.startsWith("TSubclassOf<"))
    {
        return AttributeTypeHelper::Type::Class;
    }
    else if (_typeCppString.endsWith("*"))
    {
        if (_typeCppString == "UTexture2D*")
        {
            return AttributeTypeHelper::Type::Texture;
        }
        else if (_typeCppString == "USkeletalMesh*")
        {
            return AttributeTypeHelper::Type::SkeletalMesh;
        }
        // TODO : handle other uasset types : StaticMesh, Niagara, Sound

        return AttributeTypeHelper::Type::UAsset;
    }
    else if (enumRegex.match(_typeCppString).hasMatch())
    {
        return AttributeTypeHelper::Type::Enum;
    }
    else if (structRegex.match(_typeCppString).hasMatch())
    {
        return AttributeTypeHelper::Type::Structure;
    }

    return AttributeTypeHelper::Type::Invalid;
}
bool StructureImportHelper::ValidateEnumStructStringTypeAndGetSubType(const QList<QString>& _otherStructKeys, AttributeTypeHelper::Type& _foundType, const QString& _cppType, const QString& _name, QString& _subType)
{
    if (_foundType == AttributeTypeHelper::Type::Structure)
    {
        if (_otherStructKeys.contains(_cppType))
        {
            // Struct present in the other struct list : can add struct
            _subType = _cppType;
        }
        else
        {
            // Else won't be albe to add the attribute as a struct
            return false;
        }
    }
    else if (_foundType == AttributeTypeHelper::Type::Enum)
    {
        _subType = _cppType;
        _subType.removeFirst();

        // The referenced enum is not present
        if (DB_Manager::GetDB_Manager().GetIndexOfFirstEnumWithName(_subType) < 0)
        {
            return false;
        }
    }
    else if (_foundType == AttributeTypeHelper::Type::ShortString)
    {
        QString possibleRef = _name;
        static const QString rowNameIndic = "RowName";
        static const QString rowIndic = "Row";
        static const QString nameIndic = "Name";
        if (_name.endsWith(rowNameIndic, Qt::CaseInsensitive))
        {
            possibleRef = _name.chopped(rowNameIndic.length());
        }
        else if (_name.endsWith(rowIndic, Qt::CaseInsensitive))
        {
            possibleRef = _name.chopped(rowIndic.length());
        }
        else if (_name.endsWith(nameIndic, Qt::CaseInsensitive))
        {
            possibleRef = _name.chopped(nameIndic.length());
        }


        if (!possibleRef.isEmpty() && DB_Manager::GetDB_Manager().GetStructureTableIndex(possibleRef) != -1)
        {
            _foundType = AttributeTypeHelper::Type::Reference;
            _subType = possibleRef;
        }
    }

    return true;
}

bool StructureImportHelper::GetStructureTemplateFromCppString(const QString& _structCppContent, const QMap<QString,QString>& _otherStructSections, TemplateStructure& _outTemplate)
{
    QList<AttributeTemplateRepresentation> templRepresentations = QList<AttributeTemplateRepresentation>();
    GetTemplateAttributeRepresentation(_structCppContent, _otherStructSections.keys(), templRepresentations);
    if (templRepresentations.isEmpty())
    {
        return false;
    }

    for (const auto& templAttrRep : templRepresentations)
    {
        TemplateAttribute* templAttr = StructureImportHelper::NewAttributeFromRepresentation(_otherStructSections, templAttrRep);
        _outTemplate.AddAttributeTemplateDirectly_UNSAFE(templAttr);
    }

    return true;
}
TemplateAttribute* StructureImportHelper::NewAttributeFromRepresentation(const QMap<QString,QString>& _otherStructSections, const AttributeTemplateRepresentation& _representation)
{
    return NewAttributeFromRepresentation(_otherStructSections,
                                          _representation.name,
                                          _representation.dbType,
                                          _representation.subRefStructOrEnumName,
                                          _representation.subArryDbType,
                                          _representation.subArrySubRefStructOrEnumName,
                                          _representation.active,
                                          _representation.defaultValue);
}
TemplateAttribute* StructureImportHelper::NewAttributeFromRepresentation(const QMap<QString,QString>& _otherStructSections, const QString& _name, AttributeTypeHelper::Type _type, const QString& _subRef, AttributeTypeHelper::Type _subType, const QString& _subSubRef, bool _active, const QString _defVal)
{
    AttributeParam param = AttributeParam();
    switch(_type)
    {
        default: break;
        case AttributeTypeHelper::Type::Enum:
        {
            param.enumeratorIndex = DB_Manager::GetDB_Manager().GetIndexOfFirstEnumWithName(_subRef);
            break;
        }
        case AttributeTypeHelper::Type::Array:
        {
            param.templateAtt = StructureImportHelper::NewAttributeFromRepresentation(_otherStructSections,
                                                                                      "",
                                                                                      _subType,
                                                                                      _subSubRef,
                                                                                      AttributeTypeHelper::Type::Invalid,
                                                                                      "",
                                                                                      true,
                                                                                      "");
            break;
        }
        case AttributeTypeHelper::Type::Structure:
        {
            QString structName = _subRef;
            structName.removeFirst();
            param.templateStruct = new TemplateStructure(structName);

            StructureImportHelper::GetStructureTemplateFromCppString(_otherStructSections.value(_subRef), _otherStructSections, *param.templateStruct);

            break;
        }
        case AttributeTypeHelper::Type::Reference:
        {
            param.structTable = DB_Manager::GetDB_Manager().GetStructureTable(_subRef);
        }
    }


    TemplateAttribute* templAttr = new TemplateAttribute(_name, _type, param, _active);

    if (!_defVal.isEmpty())
    {
        templAttr->GetDefaultAttributeW()->SetValue_CSV(_defVal);
    }

    return templAttr;
}
