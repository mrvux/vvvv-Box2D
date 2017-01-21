
#include "Box2dCreateJointNode.h"

namespace VVVV 
{
	namespace Nodes 
	{
		Box2dCreateJointNode::Box2dCreateJointNode(void)
		{

		}

		void Box2dCreateJointNode::SetPluginHost(v4::IPluginHost^ Host)
		{
			this->FHost = Host;

			this->FHost->CreateNodeInput("World",v4::TSliceMode::Single,v4::TPinVisibility::True,this->vInWorld);
			this->vInWorld->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::WorldDataType::GUID), v4b2d::WorldDataType::FriendlyName);

			if (this->ForceBodyOneGround())
			{
				this->FHost->CreateNodeInput("Body",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInBody2);
				this->vInBody2->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::BodyDataType::GUID), v4b2d::BodyDataType::FriendlyName);
			}
			else
			{
				this->FHost->CreateNodeInput("Body 1",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInBody1);
				this->vInBody1->SetSubType(VVVV::Utils::ArrayUtils::DoubleGuidArray(v4b2d::BodyDataType::GUID, v4b2d::GroundDataType::GUID), v4b2d::BodyDataType::FriendlyName);

				this->FHost->CreateNodeInput("Body 2",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInBody2);
				this->vInBody2->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::BodyDataType::GUID), v4b2d::BodyDataType::FriendlyName);
			}


			this->FHost->CreateValueInput("Collide Connected",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInCollideConnected);
			this->vInCollideConnected->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,false,true,false);

		
			//Output
			this->OnPluginHostSet();

			this->FHost->CreateStringInput("Custom",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInCustom);
			this->vInCustom->SetSubType("",false);

			this->FHost->CreateValueInput("Do Create",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInDoCreate);
			this->vInDoCreate->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,true,false,false);


			this->FHost->CreateValueOutput("Joint Id", 1, nullptr, v4::TSliceMode::Dynamic, v4::TPinVisibility::True, this->vOutId);
			this->vOutId->SetSubType(System::Double::MinValue, System::Double::MaxValue, 1, 0, false,false, true);


		}

		void Box2dCreateJointNode::Configurate(v4::IPluginConfig^ Input) 
		{
		}

		void Box2dCreateJointNode::ConnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInWorld)
        	{
				v4::INodeIOBase^ usI;
				this->vInWorld->GetUpstreamInterface(usI);
				this->mWorld = (v4b2d::WorldDataType^)usI;
        	}
			if (Pin == this->vInBody1) 
			{
				v4::INodeIOBase^ usI;
				try 
				{
					this->vInBody1->GetUpstreamInterface(usI);
					this->m_body1 = (v4b2d::BodyDataType^)usI;
					this->isbody = true;
				} 
				catch (System::Exception^ ex)
				{
					this->vInBody1->GetUpstreamInterface(usI);
					this->m_ground1 = (v4b2d::GroundDataType^)usI;
					this->isbody = false;
				}
			}

			if (Pin == this->vInBody2) 
			{
				v4::INodeIOBase^ usI;
				this->vInBody2->GetUpstreamInterface(usI);
				this->m_body2 = (v4b2d::BodyDataType^)usI;
			}
		}




		void Box2dCreateJointNode::DisconnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInWorld)
        	{
        		this->mWorld = nullptr;
        	}
			if (Pin == this->vInBody1)
        	{
				if (this->isbody) 
				{
        			this->m_body1 = nullptr;
				} 
				else 
				{
					this->m_ground1 = nullptr;
				}
        	}
			if (Pin == this->vInBody2)
        	{
        		this->m_body2 = nullptr;
        	}
		}

	}
}

