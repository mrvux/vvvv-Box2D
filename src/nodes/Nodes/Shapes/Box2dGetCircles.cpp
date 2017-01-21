
#include "Box2dGetCircles.h"
#include "../../Internals/Data/ShapeCustomData.h"
#include "../../Internals/Data/BodyCustomData.h"

using namespace System::Collections::Generic;
using namespace VVVV::Utils::VMath;

namespace VVVV 
{
	namespace Nodes 
	{
		Box2dGetCircles::Box2dGetCircles(void)
		{
		}

		void Box2dGetCircles::SetPluginHost(v4::IPluginHost^ Host) 
		{
			this->FHost = Host;

			this->FHost->CreateNodeInput("Shapes",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInShapes);
			this->vInShapes->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::ShapeDataType::GUID), v4b2d::ShapeDataType::FriendlyName);

			this->FHost->CreateValueInput("Local Coordinates",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInLocal);
			this->vInLocal->SetSubType(0,1,1,1,false,true,false);

			this->FHost->CreateValueOutput("Position",2,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutPosition);
			this->vOutPosition->SetSubType2D(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,0.0,false,false,false);

			this->FHost->CreateValueOutput("Radius",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutRadius);
			this->vOutRadius->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,false,false,false);

			this->FHost->CreateValueOutput("Is Sensor",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutIsSensor);
			this->vOutIsSensor->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,false,true,false);

			this->FHost->CreateStringOutput("Custom",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutCustom);
			this->vOutCustom->SetSubType("",false);

			this->FHost->CreateValueOutput("Shape Id",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutId);
			this->vOutId->SetSubType(System::Double::MinValue,System::Double::MaxValue,1,0.0,false,false,true);

			this->FHost->CreateValueOutput("Body Id",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutBodyId);
			this->vOutBodyId->SetSubType(System::Double::MinValue,System::Double::MaxValue,1,0.0,false,false,true);

			this->FHost->CreateValueOutput("LifeTime",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutLifeTime);
			this->vOutLifeTime->SetSubType(System::Double::MinValue,System::Double::MaxValue,1,0.0,false,false,false);
		}


		void Box2dGetCircles::Configurate(v4::IPluginConfig^ Input)
		{

		}

		
		void Box2dGetCircles::Evaluate(int SpreadMax)
		{
			this->vInShapes->PinIsChanged;
			
			if (this->vInLocal->PinIsChanged)
			{
				double dbllocal;
				this->vInLocal->GetValue(0,dbllocal);
				this->m_local = dbllocal >= 0.5;
			}

			if (this->vInShapes->IsConnected) 
			{
				gen::List<Vector2D> pos = gcnew gen::List<Vector2D>();
				gen::List<double>^ radius = gcnew gen::List<double>();
				gen::List<int>^ ids = gcnew gen::List<int>();
				gen::List<System::String^>^ custs = gcnew gen::List<System::String^>(); 
				std::vector<bool> issensor;
				List<int>^ bodyids = gcnew List<int>();
				List<double>^ lifetime = gcnew List<double>();

				int cnt = 0;
				for (int i = 0; i < this->vInShapes->SliceCount ; i++) 
				{
					int realslice;
					this->vInShapes->GetUpsreamSlice(i,realslice);
					b2Shape* shape = this->m_circles->GetSlice(realslice);
					if (shape->GetType() == e_circleShape) 
					{
						b2CircleShape* circle = (b2CircleShape*)shape;

						b2Vec2 local = circle->GetLocalPosition();
						if (this->m_local)
						{
							Vector2D vec = Vector2D(local.x,local.y);
							pos.Add(vec);
						}
						else
						{
							b2Vec2 world = circle->GetBody()->GetWorldPoint(local);
							Vector2D vec = Vector2D(world.x,world.y);
							pos.Add(vec);
						}

						radius->Add(circle->GetRadius());
						

						ShapeCustomData* sdata = (ShapeCustomData*)shape->GetUserData();
						ids->Add(sdata->Id);
						lifetime->Add(sdata->LifeTime);
						issensor.push_back(shape->IsSensor());
						System::String^ str = gcnew System::String(sdata->Custom);
						custs->Add(str);

						BodyCustomData* bdata = (BodyCustomData*)shape->GetBody()->GetUserData();
						bodyids->Add(bdata->Id);

						cnt++;
					}
				}

				this->vOutPosition->SliceCount = cnt;
				this->vOutRadius->SliceCount = cnt;
				this->vOutId->SliceCount = cnt;
				this->vOutIsSensor->SliceCount = issensor.size();
				this->vOutCustom->SliceCount = cnt;
				this->vOutBodyId->SliceCount = cnt;
				this->vOutLifeTime->SliceCount = cnt;

				for (int i = 0; i < cnt ; i++) 
				{	
					this->vOutPosition->SetValue2D(i, pos[i].x,pos[i].y);
					this->vOutRadius->SetValue(i,radius[i]);
					this->vOutId->SetValue(i,ids[i]);
					this->vOutIsSensor->SetValue(i, issensor.at(i));
					this->vOutCustom->SetString(i, custs[i]);
					this->vOutBodyId->SetValue(i, bodyids[i]);
					this->vOutLifeTime->SetValue(i, lifetime[i]);
				}
			}
		}

		void Box2dGetCircles::ConnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInShapes) 
			{
				v4::INodeIOBase^ usI;
				this->vInShapes->GetUpstreamInterface(usI);
				this->m_circles = (v4b2d::ShapeDataType^)usI;
			}
		}

		void Box2dGetCircles::DisconnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInShapes)
        	{
        		this->m_circles = nullptr;
        	}
		}
	}
}
