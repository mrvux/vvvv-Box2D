
#include "Box2dBaseShapeDefNode.h"

namespace VVVV 
{
	namespace Nodes 
	{
		Box2dBaseShapeDefNode::Box2dBaseShapeDefNode(void)
		{
		}

		void Box2dBaseShapeDefNode::SetPluginHost(v4::IPluginHost^ Host)
		{
			this->FHost = Host;

			this->m_shapes = gcnew v4b2d::ShapeDefDataType();

			//Output
			this->OnPluginHostSet();

			//Generic shape information
			this->FHost->CreateValueInput("Density",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInDensity);
			this->vInDensity->SetSubType(0,System::Double::MaxValue,0.01,1.0,false,false,false);

			this->FHost->CreateValueInput("Friction",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInFriction);
			this->vInFriction->SetSubType(0,System::Double::MaxValue,0.01,0.3,false,false,false);

			this->FHost->CreateValueInput("Restitution",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInRestitution);
			this->vInRestitution->SetSubType(0,System::Double::MaxValue,0.01,1.0,false,false,false);

			this->FHost->CreateValueInput("Is Sensor",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInIsSensor);
			this->vInIsSensor->SetSubType(0,1,1,0.0,false,true,false);

			this->FHost->CreateValueInput("Group Index", 1, nullptr, v4::TSliceMode::Dynamic, v4::TPinVisibility::True, this->vInGroupIndex);
			this->vInGroupIndex->SetSubType(System::Double::MinValue, System::Double::MaxValue, 1, 0, false,false, true);

			this->FHost->CreateStringInput("Custom",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInCustom);
			this->vInCustom->SetSubType("",false);

			this->FHost->CreateNodeOutput("Output",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutShapes);
			this->vOutShapes->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::ShapeDefDataType::GUID), v4b2d::ShapeDefDataType::FriendlyName);
			this->vOutShapes->SetInterface(this->m_shapes);


		}

		void Box2dBaseShapeDefNode::Configurate(v4::IPluginConfig^ Input) {}

	}
}
