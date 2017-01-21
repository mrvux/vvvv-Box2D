
#include "Box2dUpdateShapeNode.h"

#include "../../Internals/Data/ShapeCustomData.h"

using namespace VVVV::DataTypes;

namespace VVVV 
{
	namespace Nodes 
	{

		Box2dUpdateShapeNode::Box2dUpdateShapeNode(void)
		{
		}

		void Box2dUpdateShapeNode::SetPluginHost(v4::IPluginHost^ Host) 
		{
			this->FHost = Host;

			this->FHost->CreateNodeInput("Shapes",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInShapes);
			this->vInShapes->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(ShapeDataType::GUID),ShapeDataType::FriendlyName);

			this->FHost->CreateValueInput("Density",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInDensity);
			this->vInDensity->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,false,false,false);

			this->FHost->CreateValueInput("Set Density",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInSetDensity);
			this->vInSetDensity->SetSubType(0,1,1,0.0,true,false,false);

			this->FHost->CreateValueInput("Friction",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInFriction);
			this->vInFriction->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,false,false,false);

			this->FHost->CreateValueInput("Set Friction",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInSetFriction);
			this->vInSetFriction->SetSubType(0,1,1,0.0,true,false,false);

			this->FHost->CreateValueInput("Restitution",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInRestitution);
			this->vInRestitution->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,false,false,false);

			this->FHost->CreateValueInput("Set Restitution",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInSetRestitution);
			this->vInSetRestitution->SetSubType(0,1,1,0.0,true,false,false);	

			this->FHost->CreateValueInput("Group Index", 1, nullptr, v4::TSliceMode::Dynamic, v4::TPinVisibility::True, this->vInGroup);
			this->vInGroup->SetSubType(System::Double::MinValue, System::Double::MaxValue, 1, 0, false,false, true);

			this->FHost->CreateValueInput("Set Group",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInSetGroup);
			this->vInSetGroup->SetSubType(0,1,1,0.0,true,false,false);

			this->FHost->CreateStringInput("Custom",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInCustom);
			this->vInCustom->SetSubType("",false);

			this->FHost->CreateValueInput("Set Custom",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInSetCustom);
			this->vInSetCustom->SetSubType(0,1,1,0.0,true,false,false);

			//this->FHost->CreateValueInput("Is Sensor",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInSensor);
			//this->vInSensor->SetSubType(0,1,1.0,0.0,false,true,false);

			//this->FHost->CreateValueInput("Set Sensor",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInSetSensor);
			//this->vInSetSensor->SetSubType(0,1,1.0,0.0,true,false,false);

		}

		
		void Box2dUpdateShapeNode::Configurate(v4::IPluginConfig^ Input)
		{

		}

				
		void Box2dUpdateShapeNode::ConnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInShapes) 
			{
				v4::INodeIOBase^ usI;
				this->vInShapes->GetUpstreamInterface(usI);
				this->m_shapes = (ShapeDataType^)usI;
			}
		}

		void Box2dUpdateShapeNode::Evaluate(int SpreadMax)
		{
			this->vInShapes->PinIsChanged;
			if (this->vInShapes->IsConnected) 
			{
				double dblsd,dblsf,dblsr,dblsc,dblsg,dblsensor,dblsetsensor;;
				for (int i = 0; i < this->vInShapes->SliceCount; i++) 
				{
					int realslice;
					this->vInShapes->GetUpsreamSlice(i % this->vInShapes->SliceCount,realslice);
					b2Shape* shape = this->m_shapes->GetSlice(realslice);
					ShapeCustomData* sdata = (ShapeCustomData*)shape->GetUserData();

					int id = sdata->Id;

					this->vInSetFriction->GetValue(i,dblsf);
					this->vInSetDensity->GetValue(i, dblsd);
					this->vInSetRestitution->GetValue(i,dblsr);
					this->vInSetGroup->GetValue(i, dblsg);
					
					if (dblsg >= 0.5)
					{
						double g;
						this->vInGroup->GetValue(i,g);
						b2FilterData filter = shape->GetFilterData();
						filter.groupIndex = System::Convert::ToInt32(g);
						shape->SetFilterData(filter);
						shape->GetBody()->GetWorld()->Refilter(shape);
					}

					if (dblsf >= 0.5) 
					{
						double f;
						this->vInFriction->GetValue(i, f);
						shape->SetFriction(f);
					}

					if (dblsd >= 0.5) 
					{
						double d;
						this->vInDensity->GetValue(i, d);

						shape->SetDensity(d);
						shape->GetBody()->SetMassFromShapes();
					}


					if (dblsr >= 0.5) 
					{
						double r;
						this->vInRestitution->GetValue(i, r);
						shape->SetRestitution(r);
					}

					this->vInSetCustom->GetValue(i,dblsc);
					if (dblsc >= 0.5)
					{
						System::String^ cust;
						this->vInCustom->GetString(i,cust);
						sdata->Custom = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(cust);
					}
					
					/*
					this->vInSetSensor->GetValue(i, dblsetsensor);
					if (dblsetsensor > 0.5)
					{
						this->vInSensor->GetValue(i,dblsensor);
					}*/

				}
			}
		}





	
		void Box2dUpdateShapeNode::DisconnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInShapes)
        	{
        		this->m_shapes = nullptr;
        	}
		}
	}
}