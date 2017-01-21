
#include "Box2dApplyForceNode.h"

#include "../../Internals/Data/BodyCustomData.h"

namespace VVVV 
{
	namespace Nodes 
	{
		Box2dApplyForceNode::Box2dApplyForceNode(void)
		{
		}

		void Box2dApplyForceNode::SetPluginHost(v4::IPluginHost^ Host) 
		{
			this->FHost = Host;

			this->FHost->CreateNodeInput("Bodies",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInBodies);
			this->vInBodies->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::BodyDataType::GUID), v4b2d::BodyDataType::FriendlyName);

			this->FHost->CreateValueInput("Force",2,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInForce);
			this->vInForce->SetSubType2D(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,0.0,false,false,false);

			this->FHost->CreateValueInput("Position",2,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInPosition);
			this->vInPosition->SetSubType2D(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,0.0,false,false,false);

			this->FHost->CreateValueInput("Apply",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInApply);
			this->vInApply->SetSubType(0,1,1,0.0,true,false,false);	

		}

		void Box2dApplyForceNode::Configurate(v4::IPluginConfig^ Input)
		{

		}

		
		void Box2dApplyForceNode::Evaluate(int SpreadMax)
		{
			this->vInBodies->PinIsChanged;
			if (this->vInBodies->IsConnected) 
			{
				double dblapply,fx,fy,px,py;
				for (int i = 0; i < this->vInBodies->SliceCount; i++) 
				{
					int realslice;
					this->vInBodies->GetUpsreamSlice(i % this->vInBodies->SliceCount,realslice);
					b2Body* body = this->m_bodies->GetSlice(realslice);

					this->vInApply->GetValue(i,dblapply);
					if (dblapply >= 0.5) 
					{
						this->vInPosition->GetValue2D(i,px,py);
						this->vInForce->GetValue2D(i,fx,fy);
						body->ApplyForce(b2Vec2(fx,fy),b2Vec2(px,py));
					}
				}
			}
		}


		void Box2dApplyForceNode::ConnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInBodies) 
			{
				v4::INodeIOBase^ usI;
				this->vInBodies->GetUpstreamInterface(usI);
				this->m_bodies = (v4b2d::BodyDataType^)usI;
			}
		}


		void Box2dApplyForceNode::DisconnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInBodies)
        	{
        		this->m_bodies = nullptr;
        	}
		}
	}
}
