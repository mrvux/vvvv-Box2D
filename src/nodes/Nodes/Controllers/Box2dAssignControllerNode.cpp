
#include "Box2dAssignControllerNode.h"

namespace VVVV 
{
	namespace Nodes 
	{

		Box2dAssignControllerNode::Box2dAssignControllerNode(void)
		{
		}

		void Box2dAssignControllerNode::SetPluginHost(v4::IPluginHost^ Host)
		{
			this->FHost = Host;

			this->FHost->CreateNodeInput("Controller",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInController);
			this->vInController->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::ControllerDataType::GUID), v4b2d::ControllerDataType::FriendlyName);

			this->FHost->CreateNodeInput("Body",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInBody);
			this->vInBody->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::BodyDataType::GUID), v4b2d::BodyDataType::FriendlyName);

			this->FHost->CreateValueInput("Do Assign",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInDoCreate);
			this->vInDoCreate->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,true,false,false);



		}

		
		void Box2dAssignControllerNode::Configurate(v4::IPluginConfig^ Input) 
		{
		}

		
		void Box2dAssignControllerNode::ConnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInBody) 
			{
				v4::INodeIOBase^ usI;
				this->vInBody->GetUpstreamInterface(usI);
				this->m_body = (v4b2d::BodyDataType^)usI;
			}
			if (Pin == this->vInController) 
			{
				v4::INodeIOBase^ usI;
				this->vInController->GetUpstreamInterface(usI);
				this->m_controller = (v4b2d::ControllerDataType^)usI;
			}
		}
	
		void Box2dAssignControllerNode::DisconnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInBody)
        	{
        		this->m_body = nullptr;
        	}
			if (Pin == this->vInController)
			{
				this->m_controller = nullptr;
			}
		}

		void Box2dAssignControllerNode::Evaluate(int SpreadMax)
		{
			this->vInBody->PinIsChanged;
			this->vInController->PinIsChanged;

			if (this->vInBody->IsConnected && this->vInController->IsConnected)
			{
					for (int i = 0; i < SpreadMax; i++)
					{
						double dbldoassign;
						int realslice;
						this->vInDoCreate->GetValue(i, dbldoassign);

						if (dbldoassign >= 0.5)
						{
							b2Body* body;
							this->vInBody->GetUpsreamSlice(i % this->vInBody->SliceCount,realslice);
							body = this->m_body->GetSlice(realslice);

							int realslicectrl;
							this->vInController->GetUpsreamSlice(i % this->vInController->SliceCount,realslicectrl);
							b2Controller* ctrl = this->m_controller->GetSlice(realslicectrl);

							bool found = false;
							b2ControllerEdge* edge = body->GetControllerList();
							while(edge && (!found))
							{
								if (edge->controller == ctrl)
								{
									found = true;
								}
								else
								{
									edge = edge->nextController;
								}
							}
								
							if (!found)
							{
								ctrl->AddBody(body);
							}
						}
						
					
				}
			}
		}
	}
}