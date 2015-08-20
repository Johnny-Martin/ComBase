// dllmain.h : Declaration of module class.

class CATLSimpleAModule : public CAtlDllModuleT< CATLSimpleAModule >
{
public :
	DECLARE_LIBID(LIBID_ATLSimpleALib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ATLSIMPLEA, "{C322C68D-748F-46CC-8EB4-E9A11C4D4895}")
};

extern class CATLSimpleAModule _AtlModule;
