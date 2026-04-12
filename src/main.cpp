#include "vstplug.h"

AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new C1Bitcrusher (audioMaster);
}
