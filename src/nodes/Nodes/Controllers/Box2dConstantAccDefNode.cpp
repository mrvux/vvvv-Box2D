
#include "Box2dConstantAccDefNode.h"

namespace VVVV 
{
	namespace Nodes 
	{
		Box2dConstantAccDefNode::Box2dConstantAccDefNode(void)
		{
			this->m_controller = gcnew v4b2d::ControllerDataType();

		}

		void Box2dConstantAccDefNode::OnEvaluate(int SpreadMax, bool reset)
		{
			if (this->vInAcceleration->PinIsChanged
			|| reset)
			{
				for (int i = 0; i <SpreadMax; i++)
				{
					double x,y;
					this->vInAcceleration->GetValue2D(i, x,y);


					if (reset)
					{
						b2ConstantAccelControllerDef ctrldef;
						ctrldef.A.x = (float)x;
						ctrldef.A.y = (float)y;

						this->ctrl->push_back(this->m_world->GetWorld()->CreateController(&ctrldef));
						this->m_controller->Add(this->ctrl->at(i));
					}
					else
					{
						b2ConstantAccelController* ac = (b2ConstantAccelController*) this->ctrl->at(i);
						ac->A.x = (float)x;
						ac->A.y = (float)y;
					}
				}
			}
		}
		
		void Box2dConstantAccDefNode::OnPluginHostSet()
		{
			this->FHost->CreateValueInput("Acceleration",2,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInAcceleration);
			this->vInAcceleration->SetSubType2D(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,0.0,false,false,false);
		}

	}
}