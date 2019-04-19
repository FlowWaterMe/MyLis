#include "LisProtocol.h"
#include "LisASTMProtocol.h"
#include "LisHL7Protocol.h"

CLisProtocol* CLisProtocol::GetInstance(EProtocolType eProtocolType)
{
    static CLisProtocol* stProtocol;
    if (stProtocol == nullptr)
    {
        if (eProtocolType_HL7 == eProtocolType)
            stProtocol = new CLisHL7Protocol();
        else
            stProtocol = new CLisASTMProtocol();
    }

    if (stProtocol == nullptr)
        throw std::exception("LisProtocol Init Error!");

    return stProtocol;
}


