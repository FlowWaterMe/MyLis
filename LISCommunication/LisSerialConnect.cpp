#include "LisSerialConnect.h"
#include "LisDef.h"
#include <QTextCodec>

CLisSerialConnect::CLisSerialConnect()
    :CLisConnect()
{
    m_bIsAllRead = true;
    m_Serial = new QSerialPort();
    QObject::connect(m_Serial, &QSerialPort::readyRead, this, &CLisSerialConnect::SlotReadData, Qt::UniqueConnection);
}

CLisSerialConnect::~CLisSerialConnect()
{
    if (m_Serial != nullptr)
    {
        delete m_Serial;
        m_Serial = nullptr;
    }
}

bool CLisSerialConnect::LisConnect(const vector<QString>& vecParam)
{
    if (vecParam.size() < 8)
        return false;

    if (m_Serial == nullptr)
        return false;

    // 设置端口
    m_Serial->setPortName(vecParam.at(0));
    // 波特率
    m_Serial->setBaudRate(vecParam.at(1).toInt());
    // 数据位
    m_Serial->setDataBits(ConvToQtDataBit(vecParam.at(2).toInt()));
    // 检验位
    m_Serial->setParity(ConvToQtParity(vecParam.at(3).toInt()));
    // 停止位
    m_Serial->setStopBits(ConvToQtStopBit(vecParam.at(4).toInt()));
    // 流控制 
    m_Serial->setFlowControl(ConvToQtFlowControl(vecParam.at(5).toInt()));
    // XON
    // XOFF

    m_Serial->open(QIODevice::ReadWrite);
    if (!m_Serial->isOpen())
        return false;

    return true;
}

bool CLisSerialConnect::LisDisconnect()
{
    if (!m_Serial->isOpen())
        return true;

    m_Serial->close();
    if (m_Serial->isOpen())
        return false;

    return true;
}

bool CLisSerialConnect::IsConnect()
{
    return m_Serial->isOpen();
}

bool CLisSerialConnect::WriteData(QString sWriteData)
{
    m_Serial->write(sWriteData.toLocal8Bit());
    return true;
}

void CLisSerialConnect::SlotReadData()
{
    if (m_bIsAllRead)
        m_bIsAllRead = false;
    else
        return;

    QByteArray ByteArr = m_Serial->readAll();
    while (m_Serial->waitForReadyRead(10))              // 等待函数在串口有数据时触发
        ByteArr += m_Serial->readAll();

    QTextCodec *codec = QTextCodec::codecForName("System");
    QString sRecData = codec->toUnicode(ByteArr);
    m_bIsAllRead = true;
    emit SignalReceiveData(sRecData);
}
