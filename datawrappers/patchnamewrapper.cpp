#include "patchnamewrapper.h"

#include "patch.h"

QString PatchNameWrapper::getName() const
{
    return tmpName;
}

void PatchNameWrapper::setName(const QString &newName)
{
    if(newName == tmpName)
        return;

    QString newNamejustified = newName.leftJustified(Name16 - Name1, ' ');

    QByteArray nameArr = newNamejustified.toLatin1();
    for(int i=0; i<nameArr.size(); i++)
        nameArr[i] = nameArr[i] & 0x7F;

    patch->setData(Name1, nameArr, true);
}

void PatchNameWrapper::onDataChanged(unsigned int offset, int lenght, bool byUserControl)
{
    if(byUserControl)
        return;

    if((baseOffset+offset) > Name16 || baseOffset+offset+lenght < Name1)
        return;

    char *data = patch->data(Name1, Name16 - Name1);
    if( data == nullptr)
        return;

    tmpName = QString::fromLatin1(data, Name16 - Name1).trimmed();

    emit nameChanged();
}
