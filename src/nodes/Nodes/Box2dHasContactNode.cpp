
#include "Box2dHasContactNode.h"
#include "../Internals/Data/ShapeCustomData.h"
#include "../Internals/Data/BodyCustomData.h"

namespace VVVV 
{
	namespace Nodes 
	{
		Box2dHasContactNode::Box2dHasContactNode(void)
		{

		}

		void Box2dHasContactNode::SetPluginHost(v4::IPluginHost^ Host) 
		{
			this->FHost = Host;
	
			this->FHost->CreateNodeInput("World",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInWorld);
			this->vInWorld->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::WorldDataType::GUID), v4b2d::WorldDataType::FriendlyName);

			this->FHost->CreateValueInput("Id 1",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInId1);
			this->vInId1->SetSubType(System::Double::MinValue,System::Double::MaxValue,1,0.0,false,false,true);

			this->FHost->CreateValueInput("ShapeBody 1", 1, nullptr, v4::TSliceMode::Dynamic, v4::TPinVisibility::True, this->vInMode1);
			this->vInMode1->SetSubType(0, 1, 1, 0, false,true, false);

			this->FHost->CreateValueInput("Id 2",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInId2);
			this->vInId2->SetSubType(System::Double::MinValue,System::Double::MaxValue,1,0.0,false,false,true);

			this->FHost->CreateValueInput("ShapeBody 2", 1, nullptr, v4::TSliceMode::Dynamic, v4::TPinVisibility::True, this->vInMode2);
			this->vInMode2->SetSubType(0, 1, 1, 0, false,true, false);

			this->FHost->CreateValueOutput("Has Contact", 1, nullptr, v4::TSliceMode::Dynamic, v4::TPinVisibility::True, this->vOutContact);
			this->vOutContact->SetSubType(0, 1, 1, 0, false,true, false);

		}


		void Box2dHasContactNode::Configurate(v4::IPluginConfig^ Input)
		{

		}

		
		void Box2dHasContactNode::Evaluate(int SpreadMax)
		{
			if (this->vInWorld->IsConnected) 
			{		
				if (m_world->GetIsValid()) 
				{
					this->vOutContact->SliceCount = SpreadMax;
					for (int i = 0; i < SpreadMax ; i++)
					{
						bool found = false;

						double ds1,ds2;
						this->vInId1->GetValue(i, ds1);
						this->vInId2->GetValue(i, ds2);

						double dblmode1,dblmode2;
						this->vInMode1->GetValue(i, dblmode1);
						this->vInMode2->GetValue(i, dblmode2);

						bool b1,b2;
						b1 = dblmode1 > 0.5;
						b2 = dblmode2 > 0.5;

						int is1 = System::Convert::ToInt32(ds1);
						int is2 = System::Convert::ToInt32(ds2);

						for (int j = 0; j < this->m_world->Results->size() && !found; j++)
						{
							ContactResultData* result = this->m_world->Results->at(j);

							int id1;
							if (b1)
							{
								BodyCustomData* bdata = (BodyCustomData*)result->shape1->GetBody()->GetUserData();
								id1 = bdata->Id;
							}
							else
							{
								ShapeCustomData* sdata1 = (ShapeCustomData*)result->shape1->GetUserData();
								id1 = sdata1->Id;
							}

							int id2;
							if (b2)
							{
								BodyCustomData* bdata = (BodyCustomData*)result->shape2->GetBody()->GetUserData();
								id2 = bdata->Id;
							}
							else
							{
								ShapeCustomData* sdata2 = (ShapeCustomData*)result->shape2->GetUserData();
								id1 = sdata2->Id;
							}

							if (
								(id1 == is1 && id2 == is2)
								|| (id2 == is1 && id1 == is2)
								)
							{
								found = true;
							}
						}

						this->vOutContact->SetValue(i, System::Convert::ToDouble(found));

					}
				}
				else 
				{
					this->vOutContact->SliceCount = 0;
				}
			} 
			else 
			{
				this->vOutContact->SliceCount = 0;
			}
		}


		void Box2dHasContactNode::ConnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInWorld) 
			{
				v4::INodeIOBase^ usI;
				this->vInWorld->GetUpstreamInterface(usI);
				this->m_world = (v4b2d::WorldDataType^)usI;
			}
		}

		void Box2dHasContactNode::DisconnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInWorld)
        	{
        		this->m_world = nullptr;
        	}
		}
	}
}
