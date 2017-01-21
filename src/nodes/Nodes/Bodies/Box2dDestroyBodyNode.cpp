
#include "Box2dDestroyBodyNode.h"

#include "../../Internals/Data/BodyCustomData.h"

namespace VVVV 
{
	namespace Nodes 
	{
		void Box2dDestroyBodyNode::SetPluginHost(v4::IPluginHost^ Host) 
		{
			this->FHost = Host;

			this->FHost->CreateNodeInput("Bodies",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInBodies);
			this->vInBodies->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::BodyDataType::GUID), v4b2d::BodyDataType::FriendlyName);

			this->FHost->CreateValueInput("Do Destroy",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInDoDestroy);
			this->vInDoDestroy->SetSubType(System::Double::MinValue,System::Double::MaxValue,1.0,0.0,true,false,false);	

			

		}

		void Box2dDestroyBodyNode::Configurate(v4::IPluginConfig^ Input)
		{

		}

		void Box2dDestroyBodyNode::Evaluate(int SpreadMax)
		{
			this->vInBodies->PinIsChanged;
			if (this->vInBodies->IsConnected) 
			{
				double dbldelete;
				

					//double dblsp,dblsv,dblsav;
					for (int i = 0; i < this->vInBodies->SliceCount; i++) 
					{
						this->vInDoDestroy->GetValue(i, dbldelete);
						if (dbldelete >= 0.5)
						{
							int realslice;
							this->vInBodies->GetUpsreamSlice(i % this->vInBodies->SliceCount,realslice);
							b2Body* body = this->m_bodies->GetSlice(realslice);

							//Just In Case
							if (body != body->GetWorld()->GetGroundBody())
							{
								BodyCustomData* bdata = (BodyCustomData*)body->GetUserData();
								bdata->MarkedForDeletion = true;
							}
						}
					}
				
			}
		}



		void Box2dDestroyBodyNode::ConnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInBodies) 
			{
				v4::INodeIOBase^ usI;
				this->vInBodies->GetUpstreamInterface(usI);
				this->m_bodies = (v4b2d::BodyDataType^)usI;
			}
		}


		void Box2dDestroyBodyNode::DisconnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInBodies)
        	{
        		this->m_bodies = nullptr;
        	}
		}

	}
}
