#ifndef __C1Bitcrusher__
#include "C1Bitcrusher.h"
#endif

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new C1Bitcrusher (audioMaster);
}
