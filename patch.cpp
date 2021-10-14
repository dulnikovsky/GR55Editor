#include "patch.h"

Patch::Patch(QObject *parent) : QObject(parent)
{


}

bool Patch::setData(unsigned int addressOffset, const QByteArray &newDataArr, bool byUserControl)
{
    unsigned int dataOffs = addrOffsetToDataOffset(addressOffset);

    if(dataOffs == static_cast<unsigned int>(dataArr.size()))
        dataArr.append(newDataArr);
    else if(dataOffs < static_cast<unsigned int>(dataArr.size()))
        dataArr.replace(dataOffs, newDataArr.size(), newDataArr);

    emit dataChanged(addressOffset, newDataArr.size(), byUserControl);
    return true;
}

bool Patch::setData(unsigned int addressOffset, quint16 val, bool byUserControl)
{
    unsigned int dataOffs = addrOffsetToDataOffset(addressOffset);

    char valArr[2];
    valArr[0] = (val >> 7) & 0x7f;
    valArr[1] = (val & 0x7f);

    if(dataOffs == static_cast<unsigned int>(dataArr.size()))
        dataArr.append(valArr,2);
    else if(dataOffs < static_cast<unsigned int>(dataArr.size()))
        dataArr.replace(dataOffs, 2, &(valArr[0]), 2);

    emit dataChanged(addressOffset, 2, byUserControl);
    return true;
}

bool Patch::setData(unsigned int addressOffset, quint8 val, bool byUserControl)
{
    unsigned int dataOffs = addrOffsetToDataOffset(addressOffset);

    char varchar = val & 0x7f;

    if(dataOffs == static_cast<unsigned int>(dataArr.size()))
        dataArr.append(val);
    else if(dataOffs < static_cast<unsigned int>(dataArr.size()))
        dataArr.replace(dataOffs, 1, &varchar, 1);

    emit dataChanged(addressOffset, 1, byUserControl);
    return true;
}

char * Patch::data(unsigned int addressOffset, int length)
{
    unsigned int dataOffs = addrOffsetToDataOffset(addressOffset);

    if( ! (((int)dataOffs)+length < dataArr.size()))
        return nullptr;

    return dataArr.data() + dataOffs;
}

QList<QPair<quint32, quint32>> Patch::getOffsetLenghtList()
{
    QList<QPair<quint32, quint32>> list;

    QMap<Patch::SysExDataOffset, Patch::SysExDataType>::const_iterator iter = dataDefMap.constBegin();
    while (iter != dataDefMap.constEnd()) {
        list.append(QPair<quint32, quint32>(static_cast<quint32>(iter.key()), static_cast<quint32>(iter.value().length)));
        ++iter;
    }
    return list;
}

unsigned int Patch::addrOffsetToDataOffset(unsigned int addrOffset)
{
    unsigned int ret = 0;

    QMap<Patch::SysExDataOffset, Patch::SysExDataType>::const_iterator iter = dataDefMap.upperBound(static_cast<Patch::SysExDataOffset>(addrOffset));
    QMap<Patch::SysExDataOffset, Patch::SysExDataType>::const_iterator iter2 = dataDefMap.constBegin();

    unsigned lastLength = 0;
    while(iter2 != iter)
    {
        ret+= lastLength;
        lastLength =  static_cast<unsigned int>(iter2.value().length);

        iter2++;
    }
    ret = ret + (addrOffset - static_cast<unsigned int>((--iter2).key()));
    return ret;
}

const QMap<Patch::SysExDataOffset, Patch::SysExDataType> Patch::dataDefMap =
{
    {Patch::SysExDataOffset::Common, {Patch::SysExDataTypeID::Common, Patch::SysExDataLength::Common}},
    {Patch::SysExDataOffset::PcmTone1, {Patch::SysExDataTypeID::PCMTone, Patch::SysExDataLength::PCMTone}},
    {Patch::SysExDataOffset::PcmTone2, {Patch::SysExDataTypeID::PCMTone, Patch::SysExDataLength::PCMTone}}
};

