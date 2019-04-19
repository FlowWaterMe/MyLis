#include "LisConnect.h"
#include "LisSerialConnect.h"
#include "LisTCPConnect.h"
#include <exception>


CLisConnect::CLisConnect()
{

}

CLisConnect::~CLisConnect()
{

}

CLisConnect* CLisConnect::GetInstance(ELisConType eType)
{
    if (eType == eLisConType_Serial)
        return GetSerialConInstance();
    else
        return GetTCPConInstance();
}

CLisConnect* CLisConnect::GetSerialConInstance()
{
    static CLisConnect *st_pLisCon = nullptr;
    if (st_pLisCon == nullptr)
        st_pLisCon = new CLisSerialConnect();

    if (st_pLisCon == nullptr)
        throw std::exception("LisConnect Init Error!");

    return st_pLisCon;
}

CLisConnect* CLisConnect::GetTCPConInstance()
{
    static CLisConnect *st_pLisCon = nullptr;
    if (st_pLisCon == nullptr)
        st_pLisCon = new CLisTCPConnect();

    if (st_pLisCon == nullptr)
        throw std::exception("LisConnect Init Error!");

    return st_pLisCon;
}