#include "LisTCPConnect.h"
#include <QHostAddress>
#include <QTextCodec>

CLisTCPConnect::CLisTCPConnect()
    :CLisConnect()
{
    m_TcpSocket = new QTcpSocket();
    m_TcpServer = new QTcpServer();
    QObject::connect(m_TcpServer, &QTcpServer::newConnection, this, &CLisTCPConnect::SlotNewConnection);
}

CLisTCPConnect::~CLisTCPConnect()
{
    if (m_TcpSocket != nullptr)
    {
        delete m_TcpSocket;
        m_TcpSocket = nullptr;
    }

    if (m_TcpServer != nullptr)
    {
        delete m_TcpServer;
        m_TcpServer = nullptr;
    }
}

bool CLisTCPConnect::LisConnect(const vector<QString>& vecParam)
{
    if (vecParam.size() != 2 || m_TcpSocket == nullptr)
        return false;
    
    // 监听
    QHostAddress HostAddr;
    HostAddr.setAddress(vecParam.at(0));
    bool bRes = m_TcpServer->listen(HostAddr, vecParam.at(1).toInt());

    return bRes;
}

bool CLisTCPConnect::LisDisconnect()
{
    m_TcpServer->close();
    return true;
}

bool CLisTCPConnect::IsConnect()
{
    return m_TcpServer->isListening();
}

bool CLisTCPConnect::WriteData(QString sWriteData)
{
    if (!m_TcpSocket->isOpen())
        return false;

    QByteArray ByteData = sWriteData.toLocal8Bit();
    qint64 Num = m_TcpSocket->write(ByteData);
    return true;
}

void CLisTCPConnect::SlotNewConnection()
{
    m_TcpSocket = m_TcpServer->nextPendingConnection();//设置监听  
    QObject::connect(m_TcpSocket, &QTcpSocket::readyRead, this, &CLisTCPConnect::SlotReadData); //服务器接收客户端的消息  
    connect(m_TcpSocket, SIGNAL(disconnected()), m_TcpSocket, SLOT(deleteLater()));
}

void CLisTCPConnect::SlotReadData()
{
    if (m_bIsAllRead)
        m_bIsAllRead = false;
    else
        return;

    QByteArray ByteArr = m_TcpSocket->readAll();
    while (m_TcpSocket->waitForReadyRead(10))              // 等待函数在串口有数据时触发
        ByteArr += m_TcpSocket->readAll();

    QTextCodec *codec = QTextCodec::codecForName("System");
    QString sRecData = codec->toUnicode(ByteArr);
    m_bIsAllRead = true;
    emit SignalReceiveData(sRecData);
}
