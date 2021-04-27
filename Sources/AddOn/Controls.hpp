#ifndef CONTROLS_HPP
#define CONTROLS_HPP
#include "Object.hpp"

class Controls : public GS::Object
{
	DECLARE_CLASS_INFO;
public:
	Controls();
	Controls(UInt32 scale, UInt32 layer, UInt32 item);
	virtual	GSErrCode	Read(GS::IChannel& ic) override;
	virtual	GSErrCode	Write(GS::OChannel& oc) const override;

	UInt32 scale;
	UInt32 layer;
	UInt32 item;

};

#endif // !CONTROLS_HPP
