
#include "Box2dPolygonDefNode.h"

namespace VVVV 
{
	namespace Nodes 
	{
		Box2dPolygonDefNode::Box2dPolygonDefNode(void) 
		{

		}

		void Box2dPolygonDefNode::Evaluate(int SpreadMax)
		{
			if (this->vInVertices->PinIsChanged
				|| this->vInVerticesCount->PinIsChanged
				|| this->vInFriction->PinIsChanged
				|| this->vInDensity->PinIsChanged
				|| this->vInRestitution->PinIsChanged
				|| this->vInIsSensor->PinIsChanged
				|| this->vInCustom->PinIsChanged
				|| this->vInGroupIndex->PinIsChanged) 
			{

				double x,y,count,friction,restitution,density,issensor,group;
				System::String^ custom;
				int max = System::Math::Max(this->vInVerticesCount->SliceCount,this->vInFriction->SliceCount);
				max = System::Math::Max(max,this->vInDensity->SliceCount);
				max = System::Math::Max(max,this->vInRestitution->SliceCount);

				this->vOutShapes->SliceCount = max;

				this->m_shapes->Reset();
				
				int vertidx = 0;
				for (int i = 0; i < max;i++) 
				{		
					this->vInVerticesCount->GetValue(i,count);
					this->vInFriction->GetValue(i,friction);
					this->vInDensity->GetValue(i,density);
					this->vInRestitution->GetValue(i,restitution);
					this->vInIsSensor->GetValue(i,issensor);
					this->vInCustom->GetString(i, custom);
					this->vInGroupIndex->GetValue(i, group);

					b2PolygonDef* shapeDef = this->m_shapes->AddPolygon();

					count = System::Math::Min(count,24.0);
					count = System::Math::Max(count,3.0);

					shapeDef->vertexCount = count;

					for (int j = 0; j < count; j++) 
					{
						this->vInVertices->GetValue2D(vertidx,x,y);
						shapeDef->vertices[j].Set(x,y);
						vertidx++;
						if (vertidx == this->vInVertices->SliceCount) 
						{
							vertidx = 0;
						}
					}
					shapeDef->density = (float)density;
					shapeDef->friction = (float)friction;
					shapeDef->restitution = (float)restitution;
					shapeDef->isSensor = (float)issensor >= 0.5;
					shapeDef->filter.groupIndex = System::Convert::ToInt32(group);
					this->m_shapes->AddCustom(custom);
				}

				this->vOutShapes->MarkPinAsChanged();
			}

		}
		
		void Box2dPolygonDefNode::OnPluginHostSet() 
		{
			this->FHost->CreateValueInput("Vertices",2,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInVertices);
			this->vInVertices->SetSubType2D(0,System::Double::MaxValue,0.01,0.0,0.0,false,false,false);

			this->FHost->CreateValueInput("Vertices Count",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInVerticesCount);
			this->vInVerticesCount->SetSubType(3,24,1,1.0,false,false,true);

		}
	}
}
