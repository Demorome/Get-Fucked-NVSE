#include "GetFucked.h"


#include "nvse/PluginAPI.h"
#include "nvse/GameData.h"
#include "nvse/GameForms.h"
#include "nvse/GameObjects.h"
#include "nvse/SafeWrite.h"
#include "nvse/Utilities.h"
#include "nvse/GameTiles.h"


#include "params.h"

 
// Plugin Stuff
IDebugLog g_Log("GetFucked.log");
UInt32 g_MyPluginVersion = 100;
const char* g_MyPluginName = "Get Fucked";


TESActorBaseData::FactionListData nephiFaction;

#if _DEBUG

float garyFaceGenFGGS[50] =
{
	-0.010735, -0.028206, 0.288134, -0.120254, -0.301667, 0.025682, 0.227829, -0.143569, 0.272299, -0.037882,
	-0.090595, 0.078671, -0.067672, -0.312650, -0.414043, -0.099347, 0.762563, -1.153824, 0.028661, 0.305484,
	0.122966, -0.386519, -0.208481, 0.561810, 0.338715, 1.287850, 0.992090, -1.086972, 0.581703, 0.072618,
	0.033602, -1.178049, 0.571492, 0.563518, 0.913136, 1.103169, -1.252815, -0.144436, -0.764418, 1.917684,
	2.060525, 2.580075, -0.350414, 1.576132, 0.745220, -1.090083, 1.565197, 0.953444, -1.502846, -2.429406
};

float garyFaceGenFGGA[30] =
{
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000
};

float garyFaceGenFGTS[50] =
{
	-2.119767, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
	0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000,
};

#endif

// Changes voicelines to "Get Fucked".
errno_t __cdecl Actor_DoSpeechLoadLipFiles_strcpy_s_Hook(char* Dst, rsize_t SizeInBytes, char* Src)
{
	const char* newSrc = "Data\\Sound\\Voice\\FalloutNV.esm\\MaleAdult03\\1EExtraFiends_GOODBYE_0014E012_1.ogg";
	return strcpy_s(Dst, SizeInBytes, newSrc);
}

// This is a message handler for nvse events
// With this, plugins can listen to messages such as whenever the game loads
void MessageHandler(NVSEMessagingInterface::Message* msg)
{
	switch (msg->type)
	{

	case NVSEMessagingInterface::kMessage_DeferredInit:

		UInt8 FNV = DataHandler::Get()->GetModIndex("FalloutNV.esm");
		for (auto iter = DataHandler::Get()->boundObjectList->first; iter; iter = iter->next)
		{
			if (iter->typeID == 42)  // NPC Type.
			{
				TESNPC* npc = (TESNPC*)iter;
				std::string name = "Driver Nephi";
				
				npc->fullName.name.Set(name.c_str());
				npc->baseData.flags &= ~1;  // idk about this****
				npc->race.race = (TESRace*)LookupFormByID(0x0004BB8D);  // CaucasianRaider
				npc->hair = (TESHair*)LookupFormByID(0x0002A185);  // HairBalding
				npc->hairColor = 0x00494126;  // 49, 41, 26 (RGB)
				npc->hairLength = 0.87;
				npc->eyes = (TESEyes*)LookupFormByID(0x00004253);  // None?
				npc->headPart.RemoveAll();
				nephiFaction.faction = (TESFaction*)LookupFormByID(0x000EF323);  // FiendsFactionNV
				npc->baseData.factionList.Append(&nephiFaction);
				npc->baseData.voiceType = (BGSVoiceType*)LookupFormByID(0x0013C8D6);  // MaleAdult03
#if _DEBUG
				for (int i = 0; i < 50; i++)	*(float*)&npc->faceGenData[0].values[i] = garyFaceGenFGGS[i];
				for (int i = 0; i < 30; i++)	*(float*)&npc->faceGenData[1].values[i] = garyFaceGenFGGA[i];
				for (int i = 0; i < 50; i++)	*(float*)&npc->faceGenData[2].values[i] = garyFaceGenFGTS[i];
#endif

			}
		}

		// Change voice lines to "Get Fucked"
		WriteRelCall(0x8A2200, (UInt32)Actor_DoSpeechLoadLipFiles_strcpy_s_Hook);   // strcpy_s_(Dst, 0x200u, Src);

		break;

	}
}


//No idea why the extern "C" is there.
extern "C"
{

	bool NVSEPlugin_Query(const NVSEInterface* nvse, PluginInfo* info)
	{
		// fill out the info structure
		info->infoVersion = PluginInfo::kInfoVersion;
		info->name = g_MyPluginName;
		info->version = g_MyPluginVersion;

		// version checks
		if (nvse->nvseVersion < PACKED_NVSE_VERSION)  //fixed version check thanks to c6
		{
			char buffer[100];
			sprintf_s(buffer, 100,"NVSE version too old (got %08X expected at least %08X)", nvse->nvseVersion, PACKED_NVSE_VERSION);
			MessageBoxA(nullptr, buffer, g_MyPluginName, MB_ICONEXCLAMATION);  //MessageBoxA usage style copied from lStewieAl.
			_ERROR("%s", buffer);
			return false;
		}
		
		if (!nvse->isEditor)
		{
			if (nvse->runtimeVersion < RUNTIME_VERSION_1_4_0_525)
			{
				char buffer[100];
				sprintf_s(buffer, 100, "Incorrect runtime version (got %08X need at least %08X)", nvse->runtimeVersion, RUNTIME_VERSION_1_4_0_525);
				MessageBoxA(nullptr, buffer, g_MyPluginName, MB_ICONEXCLAMATION);
				_ERROR("%s", buffer);
				return false;
			}

			if (nvse->isNogore)
			{
				char buffer[] = "NoGore is not supported";
				MessageBoxA(nullptr, buffer, g_MyPluginName, MB_ICONEXCLAMATION);
				_ERROR("%s", buffer);
				return false;
			}
		}

		// version checks pass
		// any version compatibility checks should be done here
		return true;
	}


	bool NVSEPlugin_Load(const NVSEInterface* nvse)
	{
		if (!nvse->isEditor)
		{
			_MESSAGE("%s loaded successfully (In-Game).\nPlugin version: %u\n", g_MyPluginName, g_MyPluginVersion);
		}
		
		// register to receive messages from NVSE
		((NVSEMessagingInterface*)nvse->QueryInterface(kInterface_Messaging))->RegisterListener(nvse->GetPluginHandle(), "NVSE", MessageHandler);
		
		return true;
	}

};
