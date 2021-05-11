#include "C1Bitcrusher.h"

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new C1Bitcrusher (audioMaster);
}
