#include "Controls.hpp"
GS::ClassInfo Controls::classInfo("Controls", GS::Guid("B45089A9-B372-460B-B145-80E6EBF107C3"), GS::ClassVersion(1, 0));

Controls::Controls() :Controls(0, 0, 0) {

}

Controls::Controls(UInt32 scale, UInt32 layer, UInt32 item):scale(scale), layer(layer), item(item)
{

}

GSErrCode Controls::Read(GS::IChannel& ic)
{
	GS::InputFrame frame(ic, classInfo);
	ic.Read(scale);
	ic.Read(layer);
	return ic.GetInputStatus();
}

GSErrCode Controls::Write(GS::OChannel & oc) const
{
	GS::OutputFrame frame(oc, classInfo);
	oc.Write(scale);
	oc.Write(layer);
	return oc.GetOutputStatus();
}