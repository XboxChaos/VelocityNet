#ifndef XECRYPT_H
#define XECRYPT_H

#include "botan_all.h"

#include "winnames.h"
#include <iostream>
#include <string.h>

#include "XboxInternals_global.h"

class XBOXINTERNALSSHARED_EXPORT XeCrypt
{
public:
    static void BnQw_SwapDwQwLeBe(BYTE *data, DWORD length);

    static bool Pkcs1Verify(BYTE *pbMessage, DWORD cbMessage, BYTE *pbSignature, DWORD cbSignature, UINT64 publicExponent, BYTE *modulus);
};

#endif // XECRYPT_H
