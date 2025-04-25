#include "structureimporthelper.h"

#include <QMap>
#include <QRegularExpression>
#include <QRegularExpressionMatch>


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

bool StructureImportHelper::DecomposeCPPFile(const QString& _cppFileContent)
{
    QString cleanCppFile = _cppFileContent;
    cleanCppFile.replace("\r\n", "\n");
    QStringList structSections = StructureImportHelper::RemoveCommentBlocks(cleanCppFile).split("USTRUCT(");
    if (structSections.isEmpty())
    {
        return false;
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


    // TODO : for each structTableSections, create a table and import (use the prefix/suffix of the project when deducing the attributes names)
    // Remove // comments (between // and line return)
    // Remove line returns between UPROPERTY( and ; -> each property is on one line
    // Split on line returns
    // Only onsider the one starting with UPROPERTY(
    //
    // Map :
    // When ref a struct if is a table that we have -> ARef
    // If its a struct present in the otherStructSections, parse it as a strcut attribute
    // TArray -> arrays
    // E... -> enums
    // FStringId -> StringTable
    // Asset (TSubClassOf or pointers) -> properly mapped
    // If undefined, set it to bool + make it inactive
    // use = if present to fill the default value if possible

    return true;
}
QString StructureImportHelper::RemoveCommentBlocks(const QString& _text)
{
    QString valuesWithouCommentBlocks = _text;
    static QRegularExpression commentBlockRegex = QRegularExpression(R"(/\*.*?\*/)", QRegularExpression::DotMatchesEverythingOption);
    valuesWithouCommentBlocks = valuesWithouCommentBlocks.replace(commentBlockRegex, "");

    int startOfCommentBlock = valuesWithouCommentBlocks.indexOf("/*");
    if (startOfCommentBlock != -1)
    {
        valuesWithouCommentBlocks = valuesWithouCommentBlocks.left(startOfCommentBlock);
    }

    return valuesWithouCommentBlocks;
}
QString StructureImportHelper::GetStructName(const QString& _text)
{
    static QRegularExpression regex(R"(struct\s(\S*\s)?(F\w+)(?=[\n{:]| \:| \{))");
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
