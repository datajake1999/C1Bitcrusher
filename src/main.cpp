#ifndef __AWGNDither__
#include "AWGNDither.h"
#endif

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new AWGNDither (audioMaster);
}
