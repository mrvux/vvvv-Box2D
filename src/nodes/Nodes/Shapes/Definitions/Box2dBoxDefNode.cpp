
#include "Box2dBoxDefNode.h"

namespace VVVV 
{
	namespace Nodes 
	{
		Box2dBoxDefNode::Box2dBoxDefNode(void) 
		{

		}

		Box2dBoxDefNode::~Box2dBoxDefNode(void)
		{
			delete this->m_shapes;
		}

		void Box2dBoxDefNode::Evaluate(int SpreadMax)
		{
			if (this->vInLocalPosition->PinIsChanged
				|| this->vInSize->PinIsChanged
				|| this->vInAngle->PinIsChanged
				|| this->vInFriction->PinIsChanged
				|| this->vInDensity->PinIsChanged
				|| this->vInRestitution->PinIsChanged
				|| this->vInIsSensor->PinIsChanged
				|| this->vInCustom->PinIsChanged
				|| this->vInGroupIndex->PinIsChanged) 
			{

				double x,y,sx,sy,a,friction,restitution,density,issensor,group;
				System::String^ custom;
				this->vOutShapes->SliceCount = SpreadMax;

				this->m_shapes->Reset();
				
				for (int i = 0; i < SpreadMax;i++) 
				{		
					this->vInLocalPosition->GetValue2D(i,x,y);
					this->vInSize->GetValue2D(i,sx,sy);
					this->vInAngle->GetValue(i,a);
					this->vInFriction->GetValue(i,friction);
					this->vInDensity->GetValue(i,density);
					this->vInRestitution->GetValue(i,restitution);
					this->vInIsSensor->GetValue(i,issensor);
					this->vInCustom->GetString(i, custom);
					this->vInGroupIndex->GetValue(i, group);

					b2PolygonDef* shapeDef = this->m_shapes->AddPolygon();
					b2Vec2 center((float)x, (float)y);
					shapeDef->SetAsBox((float)sx / 2.0f, (float)sy / 2.0f,center, (float)(a * (System::Math::PI * 2.0)));
					shapeDef->density = (float)density;
					shapeDef->friction = (float)friction;
					shapeDef->restitution = (float)restitution;
					shapeDef->isSensor = issensor >= 0.5;
					shapeDef->filter.groupIndex = System::Convert::ToInt32(group);

					this->m_shapes->AddCustom(custom);


					
				}

				this->vOutShapes->MarkPinAsChanged();
			}

		}
		
		void Box2dBoxDefNode::OnPluginHostSet() 
		{
			this->FHost->CreateValueInput("Position",2,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInLocalPosition);
			this->vInLocalPosition->SetSubType2D(0,System::Double::MaxValue,0.01,0.0,0.0,false,false,false);

			this->FHost->CreateValueInput("Size",2,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInSize);
			this->vInSize->SetSubType2D(0,System::Double::MaxValue,0.01,1.0,1.0,false,false,false);

			this->FHost->CreateValueInput("Angle",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInAngle);
			this->vInAngle->SetSubType(0,System::Double::MaxValue,0.01,0.0,false,false,false);
		}
	}
}
