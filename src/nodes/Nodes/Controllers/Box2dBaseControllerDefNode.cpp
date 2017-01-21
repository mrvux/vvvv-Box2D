
#include "Box2dBaseControllerDefNode.h"

namespace VVVV 
{
	namespace Nodes 
	{
		Box2dBaseControllerDefNode::Box2dBaseControllerDefNode(void)
		{
			 this->m_controller = gcnew v4b2d::ControllerDataType();
			 this->ctrl = new std::vector<b2Controller*>;
			 //this->m_controller->SetController(nullptr);
		}

		Box2dBaseControllerDefNode::~Box2dBaseControllerDefNode() 
		{
			if (this->ctrl != 0) 
			{
				if (this->m_world->GetIsValid())
				{
					for (int i = 0; i < this->ctrl->size();i++)
					{
						this->ctrl->at(i)->Clear();
						this->m_world->GetWorld()->DestroyController(this->ctrl->at(i));
					}
				}
			}
		}

		void Box2dBaseControllerDefNode::SetPluginHost(v4::IPluginHost^ Host)
		{
			this->FHost = Host;

			this->FHost->CreateNodeInput("World",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInWorld);
			this->vInWorld->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::WorldDataType::GUID), v4b2d::WorldDataType::FriendlyName);

			this->OnPluginHostSet();

			this->FHost->CreateValueInput("Clear",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInClear);
			this->vInClear->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,true,false,false);

			this->FHost->CreateNodeOutput("Controller",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutController);
			this->vOutController->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::ControllerDataType::GUID), v4b2d::ControllerDataType::FriendlyName);
			this->vOutController->SetInterface(this->m_controller);

			//this->FHost->CreateValueOutput("Body Count",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutBodyCount);
			//this->vOutBodyCount->SetSubType(-1,System::Double::MaxValue,1,0.0,false,false,true);
			
		}

		void Box2dBaseControllerDefNode::Evaluate(int SpreadMax)
		{
			this->vInWorld->PinIsChanged;
			bool redo = false;
			if (this->vInWorld->IsConnected)
			{
				if (this->m_world->GetIsValid())
				{
					if (this->ctrl->size() != SpreadMax)
					{
						redo = true;
					}
					else
					{
						if (this->m_world->HasReset())
						{
							redo = true;
						}
					}

					if (redo)
					{
						if (!this->m_world->HasReset())
						{
							for (int i = 0; i < this->ctrl->size();i++)
							{
								this->ctrl->at(i)->Clear();
								this->m_world->GetWorld()->DestroyController(this->ctrl->at(i));
							}
						}

						this->ctrl->clear();
						this->m_controller->Reset();
					}

					this->OnEvaluate(SpreadMax,redo);

					for (int i = 0; i < this->ctrl->size(); i++)
					{
						double dblclear;
						this->vInClear->GetValue(i, dblclear);

						if (dblclear >= 0.5)
						{	
							this->ctrl->at(i)->Clear();
						}
					}

					


				}
			}

			this->vOutController->SliceCount = this->ctrl->size();
			this->vOutController->MarkPinAsChanged();
		}

		void Box2dBaseControllerDefNode::ConnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInWorld) 
			{
				v4::INodeIOBase^ usI;
				this->vInWorld->GetUpstreamInterface(usI);
				this->m_world = (v4b2d::WorldDataType^)usI;
			}
		}

		void Box2dBaseControllerDefNode::DisconnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInWorld)
        	{
				if (this->m_world != nullptr)
				{
					if (this->m_world->GetIsValid())
					{
						for (int i = 0; i < this->ctrl->size();i++)
						{
							this->ctrl->at(i)->Clear();
							this->m_world->GetWorld()->DestroyController(this->ctrl->at(i));
						}
					}
        			this->m_world = nullptr;
					this->ctrl->clear();
				}
				this->m_controller->Reset();
        	}
		}

	}
}
