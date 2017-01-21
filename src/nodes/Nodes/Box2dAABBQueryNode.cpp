
#include "Box2dAABBQueryNode.h"



namespace VVVV 
{
	namespace Nodes 
	{
		Box2dAABBQueryNode::Box2dAABBQueryNode(void)
		{
			this->mBodies = gcnew v4b2d::BodyDataType();
			this->mShapes = gcnew v4b2d::ShapeDataType();
		}

		void Box2dAABBQueryNode::SetPluginHost(v4::IPluginHost^ Host) 
		{
			this->FHost = Host;
	
			this->FHost->CreateNodeInput("World",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInWorld);
			this->vInWorld->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::WorldDataType::GUID), v4b2d::WorldDataType::FriendlyName);

			this->FHost->CreateValueInput("Lower Bound",2,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInLowerBound);
			this->vInLowerBound->SetSubType2D(System::Double::MinValue,System::Double::MaxValue,0.01,-0.1,-0.1,false,false,false);

			this->FHost->CreateValueInput("Upper Bound",2,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInUpperBound);
			this->vInUpperBound->SetSubType2D(System::Double::MinValue,System::Double::MaxValue,0.01,0.1,0.1,false,false,false);

			this->FHost->CreateValueInput("Do Query",1,nullptr,v4::TSliceMode::Single,v4::TPinVisibility::True,this->vInDoQuery);
			this->vInDoQuery->SetSubType(0,1,1,0.0,true,false,false);

			this->FHost->CreateValueOutput("Query Index",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutQueryIndex);
			this->vOutQueryIndex->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,true,false,false);

			this->FHost->CreateNodeOutput("Shapes",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutShapes);
			this->vOutShapes->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::ShapeDataType::GUID), v4b2d::ShapeDataType::FriendlyName);
			this->vOutShapes->SetInterface(this->mShapes);

			this->FHost->CreateValueOutput("Shape Ids",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutShapeId);
			this->vOutShapeId->SetSubType(0,System::Double::MaxValue,1,0,false,false,true);

			this->FHost->CreateNodeOutput("Bodies",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutBodies);
			this->vOutBodies->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::BodyDataType::GUID), v4b2d::BodyDataType::FriendlyName);
			this->vOutBodies->SetInterface(this->mBodies);

			this->FHost->CreateValueOutput("Body Ids",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutBodyId);
			this->vOutBodyId->SetSubType(0,System::Double::MaxValue,1,0,false,false,true);
		}


		void Box2dAABBQueryNode::Configurate(v4::IPluginConfig^ Input)
		{

		}

		void Box2dAABBQueryNode::Evaluate(int SpreadMax)
		{
			this->vInWorld->PinIsChanged;
			this->mBodies->Reset();
			this->mShapes->Reset();

			if (this->vInWorld->IsConnected) 
			{		
				if (m_world->GetIsValid()) 
				{
					double dblquery;
					this->vInDoQuery->GetValue(0,dblquery);
					std::vector<b2Shape*> shapes;
					std::vector<b2Body*> bodies;
					std::vector<int> queryindex;
					std::vector<int> shapeids;
					std::vector<int> bodyids;

					if (dblquery >= 0.5) 
					{
						for (int i = 0; i < SpreadMax; i++) 
						{
							double lx,ly,ux,uy;
							this->vInLowerBound->GetValue2D(i,lx,ly);
							this->vInUpperBound->GetValue2D(i,ux,uy);

							b2AABB aabb;
							aabb.lowerBound.Set(lx,ly);
							aabb.upperBound.Set(ux,uy);
							
							const int32 k_bufferSize = 50;
							b2Shape *buffer[k_bufferSize];
							int32 count = m_world->GetWorld()->Query(aabb, buffer, k_bufferSize);

							for (int32 j = 0; j < count; ++j)
							{
								shapes.push_back(buffer[j]);		
								bodies.push_back(buffer[j]->GetBody());	
								queryindex.push_back(i);

								ShapeCustomData* sdata  = (ShapeCustomData*)buffer[j]->GetUserData();
								shapeids.push_back(sdata->Id);

								BodyCustomData* bdata = (BodyCustomData*)buffer[j]->GetBody()->GetUserData();
								bodyids.push_back(bdata->Id);
							}
						}
					}

					this->vOutShapes->SliceCount = queryindex.size();
					this->vOutQueryIndex->SliceCount = queryindex.size();
					this->vOutBodies->SliceCount = queryindex.size();
					this->vOutBodyId->SliceCount = queryindex.size();
					this->vOutShapeId->SliceCount = queryindex.size();
					
					for (int i = 0; i < queryindex.size();i++) 
					{
						this->vOutQueryIndex->SetValue(i,queryindex.at(i));
						this->mShapes->Add(shapes.at(i));
						this->mBodies->Add(bodies.at(i));
						this->vOutBodyId->SetValue(i,bodyids.at(i));
						this->vOutShapeId->SetValue(i,shapeids.at(i));
					}
				}
				else 
				{
					this->vOutQueryIndex->SliceCount = 0;
					this->vOutShapes->SliceCount = 0;
					this->vOutBodies->SliceCount = 0;
					this->vOutBodyId->SliceCount = 0;
					this->vOutShapeId->SliceCount = 0;
				}
			} 
			else 
			{
				this->vOutQueryIndex->SliceCount = 0;
				this->vOutShapes->SliceCount = 0;
				this->vOutBodies->SliceCount = 0;
				this->vOutBodyId->SliceCount = 0;
				this->vOutShapeId->SliceCount = 0;
			}

			this->vOutShapes->MarkPinAsChanged();
			this->vOutBodies->MarkPinAsChanged();
		
		}


		void Box2dAABBQueryNode::ConnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInWorld) 
			{
				v4::INodeIOBase^ usI;
				this->vInWorld->GetUpstreamInterface(usI);
				this->m_world = (v4b2d::WorldDataType^)usI;
			}
		}

		void Box2dAABBQueryNode::DisconnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInWorld)
        	{
        		this->m_world = nullptr;
        	}
		}
	}
}
