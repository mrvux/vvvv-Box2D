#include "Box2dContactDetailsDualNode.h"
#include "../Internals/Data/ShapeCustomData.h"
#include "../Internals/Data/BodyCustomData.h"

namespace VVVV 
{
	namespace Nodes 
	{
		Box2dContactDetailsDualNode::Box2dContactDetailsDualNode(void)
		{
			this->m_shape = gcnew v4b2d::ShapeDataType();
			this->mBody = gcnew v4b2d::BodyDataType();
		}

		void Box2dContactDetailsDualNode::SetPluginHost(v4::IPluginHost^ Host) 
		{
			this->FHost = Host;
	
			this->FHost->CreateNodeInput("World",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInWorld);
			this->vInWorld->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::WorldDataType::GUID), v4b2d::WorldDataType::FriendlyName);

			this->FHost->CreateValueOutput("Contact Id",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutId);
			this->vOutId->SetSubType(System::Double::MinValue,System::Double::MaxValue,1,0.0,false,false,true);

			this->FHost->CreateValueOutput("Position",2,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutPosition);
			this->vOutPosition->SetSubType2D(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,0.0,false,false,false);

			this->FHost->CreateValueOutput("Normals",2,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutNormals);
			this->vOutNormals->SetSubType2D(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,0.0,false,false,false);

			this->FHost->CreateValueOutput("Normal Impulse", 1, nullptr, v4::TSliceMode::Dynamic, v4::TPinVisibility::True, this->vOutNormalImpulse);
			this->vOutNormalImpulse->SetSubType(System::Double::MinValue, System::Double::MaxValue, 0.01, 0, false,false, false);

			this->FHost->CreateValueOutput("Tangent Impulse", 1, nullptr, v4::TSliceMode::Dynamic, v4::TPinVisibility::True, this->vOutTangentImpulse);
			this->vOutTangentImpulse->SetSubType(System::Double::MinValue, System::Double::MaxValue, 0.01, 0, false,false, false);

			this->FHost->CreateNodeOutput("Shapes",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutShape);
			this->vOutShape->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::ShapeDataType::GUID), v4b2d::ShapeDataType::FriendlyName);
			this->vOutShape->SetInterface(this->m_shape);
	
			this->FHost->CreateNodeOutput("Bodies",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutBody);
			this->vOutBody->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::BodyDataType::GUID), v4b2d::BodyDataType::FriendlyName);
			this->vOutBody->SetInterface(this->mBody);

			this->FHost->CreateValueOutput("Bodies Id",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutBodyId);
			this->vOutBodyId->SetSubType(System::Double::MinValue,System::Double::MaxValue,1,0.0,false,false,true);

			this->FHost->CreateValueOutput("Shapes Id",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutShapes);
			this->vOutShapes->SetSubType(System::Double::MinValue,System::Double::MaxValue,1,0.0,false,false,true);

			this->FHost->CreateValueOutput("Is New",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutNew);
			this->vOutNew->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,true,false,false);

			this->FHost->CreateValueOutput("Contact Count",1,nullptr,v4::TSliceMode::Single,v4::TPinVisibility::True,this->vOutContactCount);
			this->vOutContactCount->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,true,false,false);

		}


		void Box2dContactDetailsDualNode::Configurate(v4::IPluginConfig^ Input)
		{

		}

		
		void Box2dContactDetailsDualNode::Evaluate(int SpreadMax)
		{
			if (this->vInWorld->IsConnected) 
			{		
				if (m_world->GetIsValid()) 
				{
					this->vOutContactCount->SetValue(0,this->m_world->GetWorld()->GetContactCount());

					this->vOutId->SliceCount = this->m_world->Results->size();
					this->vOutPosition->SliceCount = this->m_world->Results->size();
					this->vOutNormals->SliceCount = this->m_world->Results->size();
					this->vOutShapes->SliceCount = this->m_world->Results->size() * 2;
					this->vOutShape->SliceCount = this->m_world->Results->size() * 2;
					this->vOutBody->SliceCount = this->m_world->Results->size() * 2;
					this->vOutBodyId->SliceCount = this->m_world->Results->size() * 2;
					this->vOutNew->SliceCount = this->m_world->Results->size();
					this->vOutNormalImpulse->SliceCount = this->m_world->Results->size();
					this->vOutTangentImpulse->SliceCount = this->m_world->Results->size();
					this->m_shape->Reset();
					this->mBody->Reset();

					for (int i = 0; i < this->m_world->Results->size(); i++)
					{
						ContactResultData* result = this->m_world->Results->at(i);
						this->vOutId->SetValue(i, System::Convert::ToDouble(result->id.key));
						this->vOutPosition->SetValue2D(i,result->position.x,result->position.y);
						this->vOutNormals->SetValue2D(i,result->normal.x,result->normal.y);
						this->vOutNormalImpulse->SetValue(i, result->normalImpulse);
						this->vOutTangentImpulse->SetValue(i, result->tangentImpulse);
						

						ShapeCustomData* sdata1 = (ShapeCustomData*)result->shape1->GetUserData();
						this->vOutShapes->SetValue(i * 2,sdata1->Id);
						ShapeCustomData* sdata2 = (ShapeCustomData*)result->shape2->GetUserData();
						this->vOutShapes->SetValue(i * 2 + 1,sdata2->Id);

						bool found = false;
						for (int c = 0; c < this->m_world->Reports->size() && !found; c++)
						{
							ContactReportId* report = this->m_world->Reports->at(c);
							if (report->FeaturesId == result->id.key
								&& report->Shape1 == sdata1->Id
								&& report->Shape2 == sdata2->Id)
							{
								found = true;
								this->vOutNew->SetValue(i,report->IsNew);
							}
						}

						if (!found)
						{
							this->vOutNew->SetValue(i, 0);
						}

						BodyCustomData* bdata = (BodyCustomData*)result->shape1->GetBody()->GetUserData();
						this->vOutBodyId->SetValue(i * 2, bdata->Id);
						bdata = (BodyCustomData*)result->shape2->GetBody()->GetUserData();
						this->vOutBodyId->SetValue(i * 2  +1, bdata->Id);

						this->m_shape->Add(result->shape1);
						this->m_shape->Add(result->shape2);
						this->mBody->Add(result->shape1->GetBody());
						this->mBody->Add(result->shape2->GetBody());

					}
					this->vOutShape->MarkPinAsChanged();
					this->vOutBody->MarkPinAsChanged();
				}
				else 
				{
					this->vOutPosition->SliceCount = 0;
					this->vOutNormals->SliceCount = 0;
					this->vOutShapes->SliceCount = 0;
					this->vOutNew->SliceCount = 0;
					this->vOutBody->SliceCount = 0;
				}
			} 
			else 
			{
				this->vOutPosition->SliceCount = 0;
				this->vOutNormals->SliceCount = 0;
				this->vOutShapes->SliceCount = 0;
				this->vOutNew->SliceCount = 0;
				this->vOutBody->SliceCount = 0;
			}
		}


		void Box2dContactDetailsDualNode::ConnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInWorld) 
			{
				v4::INodeIOBase^ usI;
				this->vInWorld->GetUpstreamInterface(usI);
				this->m_world = (v4b2d::WorldDataType^)usI;
			}
		}

		void Box2dContactDetailsDualNode::DisconnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInWorld)
        	{
        		this->m_world = nullptr;
        	}
		}
	}
}
