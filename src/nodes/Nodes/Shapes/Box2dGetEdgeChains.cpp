
#include "Box2dGetEdgeChains.h"
#include "../../Internals/Data/ShapeCustomData.h"
#include "../../Internals/Data/BodyCustomData.h"

using namespace System::Collections::Generic;
using namespace VVVV::Utils::VMath;

namespace VVVV 
{
	namespace Nodes 
	{
		Box2dGetEdgeChains::Box2dGetEdgeChains(void)
		{
		}

		void Box2dGetEdgeChains::SetPluginHost(v4::IPluginHost^ Host) 
		{
			this->FHost = Host;

			this->FHost->CreateNodeInput("Shapes",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInShapes);
			this->vInShapes->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::ShapeDataType::GUID), v4b2d::ShapeDataType::FriendlyName);

			this->FHost->CreateValueInput("Local Coordinates",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInLocal);
			this->vInLocal->SetSubType(0,1,1,1,false,true,false);


			//this->FHost->CreateValueOutput("Centers",2,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutCenters);
			//this->vOutCenters->SetSubType2D(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,0.0,false,false,false);

			this->FHost->CreateValueOutput("Vertices 1",2,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutVertices1);
			this->vOutVertices1->SetSubType2D(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,0.0,false,false,false);

			this->FHost->CreateValueOutput("Vertices 2",2,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutVertices2);
			this->vOutVertices2->SetSubType2D(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,0.0,false,false,false);

			this->FHost->CreateValueOutput("Is Sensor",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutIsSensor);
			this->vOutIsSensor->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,false,true,false);

			this->FHost->CreateStringOutput("Custom",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutCustom);
			this->vOutCustom->SetSubType("",false);

			this->FHost->CreateValueOutput("Body Id",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutBodyId);
			this->vOutBodyId->SetSubType(System::Double::MinValue,System::Double::MaxValue,1,0.0,false,false,true);

			this->FHost->CreateValueOutput("Shape Id",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutId);
			this->vOutId->SetSubType(System::Double::MinValue,System::Double::MaxValue,1,0.0,false,false,true);

			this->FHost->CreateValueOutput("LifeTime",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutLifeTime);
			this->vOutLifeTime->SetSubType(System::Double::MinValue,System::Double::MaxValue,1,0.0,false,false,false);
		}


		void Box2dGetEdgeChains::Configurate(v4::IPluginConfig^ Input)
		{

		}

		
		void Box2dGetEdgeChains::Evaluate(int SpreadMax)
		{
			if (this->vInLocal->PinIsChanged)
			{
				double dbllocal;
				this->vInLocal->GetValue(0,dbllocal);

				this->m_local = dbllocal >= 0.5;
			}

			if (this->vInShapes->IsConnected) 
			{
				std::vector<b2Vec2> vertices1;
				std::vector<b2Vec2> vertices2;
				std::vector<bool> issensor;
				std::vector<int> ids;
				gen::List<System::String^>^ custs = gcnew gen::List<System::String^>();
				List<double>^ lifetime = gcnew List<double>();

				List<int>^ bodyids = gcnew List<int>();
				int cnt = 0;
				for (int i = 0; i < this->vInShapes->SliceCount ; i++) 
				{
					int realslice;
					this->vInShapes->GetUpsreamSlice(i,realslice);
					b2Shape* shape = this->m_polygons->GetSlice(realslice);
					
					if (shape->GetType() == e_edgeShape) 
					{
						b2EdgeShape* poly = (b2EdgeShape*)shape;

						if (this->m_local)
						{
							vertices1.push_back(poly->GetVertex1());
							vertices2.push_back(poly->GetVertex2());
						}
						else
						{
							vertices1.push_back(poly->GetBody()->GetWorldPoint(poly->GetVertex1()));
							vertices2.push_back(poly->GetBody()->GetWorldPoint(poly->GetVertex2()));
						}
							
						issensor.push_back(shape->IsSensor());

						ShapeCustomData* sdata = (ShapeCustomData*)shape->GetUserData();
						ids.push_back(sdata->Id);
						lifetime->Add(sdata->LifeTime);
						System::String^ str = gcnew System::String(sdata->Custom);
						custs->Add(str);

						BodyCustomData* bdata = (BodyCustomData*)shape->GetBody()->GetUserData();
						bodyids->Add(bdata->Id);
					}
				}

				this->vOutVertices1->SliceCount = vertices1.size();
				this->vOutVertices2->SliceCount = vertices2.size();
				this->vOutIsSensor->SliceCount = issensor.size();
				this->vOutBodyId->SliceCount = issensor.size();
				this->vOutId->SliceCount = ids.size();
				this->vOutCustom->SliceCount = ids.size();
				this->vOutLifeTime->SliceCount = ids.size();

				for (int i = 0; i <  vertices1.size() ; i++) 
				{
					this->vOutVertices1->SetValue2D(i,vertices1.at(i).x,vertices1.at(i).y);
					this->vOutVertices2->SetValue2D(i,vertices2.at(i).x,vertices2.at(i).y);
					this->vOutIsSensor->SetValue(i, issensor.at(i));
					this->vOutBodyId->SetValue(i, bodyids[i]);
				}

				
				for (int i = 0; i < ids.size() ; i++) 
				{
					this->vOutId->SetValue(i,ids.at(i));
					this->vOutCustom->SetString(i, custs[i]);
					this->vOutLifeTime->SetValue(i, lifetime[i]);
				}
			}
		}

		void Box2dGetEdgeChains::ConnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInShapes) 
			{
				v4::INodeIOBase^ usI;
				this->vInShapes->GetUpstreamInterface(usI);
				this->m_polygons = (v4b2d::ShapeDataType^)usI;
			}
		}

		void Box2dGetEdgeChains::DisconnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInShapes)
        	{
        		this->m_polygons = nullptr;
        	}
		}
	}
}



