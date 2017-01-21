
#include "Box2dDestroyShape.h"

#include "../../Internals/Data/ShapeCustomData.h"

namespace VVVV
{
	namespace Nodes
	{
		Box2dDestroyShape::Box2dDestroyShape(void)
		{
		}

		void Box2dDestroyShape::SetPluginHost(v4::IPluginHost^ Host) 
		{
			this->FHost = Host;

			this->FHost->CreateNodeInput("Shapes",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInShapes);
			this->vInShapes->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::ShapeDataType::GUID), v4b2d::ShapeDataType::FriendlyName);

			this->FHost->CreateValueInput("Do Destroy",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInDoDestroy);
			this->vInDoDestroy->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,true,false,false);	

		}


		void Box2dDestroyShape::Configurate(v4::IPluginConfig^ Input)
		{

		}

		
		void Box2dDestroyShape::Evaluate(int SpreadMax)
		{
			this->vInShapes->PinIsChanged;
			if (this->vInShapes->IsConnected) 
			{
				double dbldelete;

				for (int i = 0; i < this->vInShapes->SliceCount; i++) 
				{
					this->vInDoDestroy->GetValue(i, dbldelete);
					if (dbldelete >= 0.5)
					{
						int realslice;
						this->vInShapes->GetUpsreamSlice(i % this->vInShapes->SliceCount,realslice);
						b2Shape* shape = this->m_shapes->GetSlice(realslice);

						if ((shape->GetType() == e_polygonShape) || (shape->GetType() == e_circleShape))
						{
							ShapeCustomData* sdata = (ShapeCustomData*)shape->GetUserData();
							sdata->MarkedForDeletion = true;
						}

						
						
					}
				}
			}
		}

		void Box2dDestroyShape::ConnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInShapes) 
			{
				v4::INodeIOBase^ usI;
				this->vInShapes->GetUpstreamInterface(usI);
				this->m_shapes = (v4b2d::ShapeDataType^)usI;
			}
		}

		void Box2dDestroyShape::DisconnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInShapes)
        	{
        		this->m_shapes = nullptr;
        	}
		}
	}

}
