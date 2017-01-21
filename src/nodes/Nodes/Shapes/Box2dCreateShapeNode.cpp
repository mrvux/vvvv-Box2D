
#include "Box2dCreateShapeNode.h"

namespace VVVV 
{
	namespace Nodes 
	{
		Box2dCreateShapeNode::Box2dCreateShapeNode(void)
		{
		}

		void Box2dCreateShapeNode::SetPluginHost(v4::IPluginHost^ Host) 
		{
			this->FHost = Host;

			//World input
			this->FHost->CreateNodeInput("World",v4::TSliceMode::Single,v4::TPinVisibility::True,this->vInWorld);
			this->vInWorld->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::WorldDataType::GUID), v4b2d::WorldDataType::FriendlyName);

			this->FHost->CreateNodeInput("Bodies",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInBodies);
			this->vInBodies->SetSubType(VVVV::Utils::ArrayUtils::DoubleGuidArray(v4b2d::BodyDataType::GUID, v4b2d::GroundDataType::GUID), v4b2d::BodyDataType::FriendlyName);

			this->FHost->CreateNodeInput("Shapes",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInShapes);
			this->vInShapes->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::ShapeDefDataType::GUID), v4b2d::ShapeDefDataType::FriendlyName);

			this->FHost->CreateValueInput("Shape Count",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInShapeCount);
			this->vInShapeCount->SetSubType(1,System::Double::MaxValue,1,1,false,false,true);

			this->FHost->CreateValueInput("Do Create",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInDoCreate);
			this->vInDoCreate->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,true,false,false);

			//this->FHost->CreateValueOutput("Can Create",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutCanCreate);
			//this->vOutCanCreate->SetSubType(0,1,1,0,true,false,false);


			//this->FHost->CreateNodeOutput("Body",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutBodies);
			//this->vOutBodies->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(BodyDataType::GUID),BodyDataType::FriendlyName);
			//this->vOutBodies->SetInterface(this->mBodies);

			
		}

		void Box2dCreateShapeNode::Evaluate(int SpreadMax)
		{
			double dblcreate;
			this->vInDoCreate->GetValue(0,dblcreate);


			if (dblcreate >= 0.5 && this->vInBodies->IsConnected && this->vInShapes->IsConnected && this->vInWorld->IsConnected) 
			{
				if (this->mWorld->GetIsValid()) 
				{
					int cnt = 0;
					for (int i = 0; i < vInBodies->SliceCount; i++) 
					{
						double dblcount;

						this->vInShapeCount->GetValue(i, dblcount);
						int icount = System::Convert::ToInt32(dblcount);
						
						b2Body* body;

						if (this->isbody) 
						{
							int realslicebody;
							this->vInBodies->GetUpsreamSlice(i % this->vInBodies->SliceCount,realslicebody);
							body = this->mBodies->GetSlice(realslicebody);
						} 
						else
						{
							body = this->mGround->GetGround();
						}
								
						for (int j = 0; j < icount ; j++)
						{
							int realsliceshape;
							this->vInShapes->GetUpsreamSlice(cnt % this->vInShapes->SliceCount,realsliceshape);
							b2ShapeDef* shapedef = this->mShapes->GetSlice(realsliceshape);
							System::String^ shapecust = this->mShapes->GetCustom(realsliceshape);


							bool testcount;
							if (shapedef->type == e_edgeShape)
							{
								b2EdgeChainDef* chain = (b2EdgeChainDef*)shapedef;
								int vcount = chain->vertexCount;
								if (chain->isALoop)
								{
									vcount++;
								}

								testcount = this->mWorld->GetWorld()->GetProxyCount() + vcount <= b2_maxProxies;
	
							}
							else
							{
								testcount = this->mWorld->GetWorld()->GetProxyCount() < b2_maxProxies;
							}

							if (testcount)
							{
								b2Shape* shape = body->CreateShape(shapedef);
								ShapeCustomData* sdata = new ShapeCustomData();
								sdata->Id = this->mWorld->GetNewShapeId();
								sdata->Custom = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(shapecust);
								shape->SetUserData(sdata);
							}

							cnt++;
						}

						if (!body->IsStatic()) 
						{
							body->SetMassFromShapes();
						}
					}
				}

				
			} 

		}



		void Box2dCreateShapeNode::Configurate(v4::IPluginConfig^ Input)
		{

		}

		void Box2dCreateShapeNode::ConnectPin(v4::IPluginIO^ Pin)
		{
			//cache a reference to the upstream interface when the NodeInput pin is being connected
			if (Pin == this->vInWorld)
        	{
				v4::INodeIOBase^ usI;
				this->vInWorld->GetUpstreamInterface(usI);
				this->mWorld = (v4b2d::WorldDataType^)usI;
        	}
        	if (Pin == this->vInBodies)
        	{
				v4::INodeIOBase^ usI;
				try 
				{
					this->vInBodies->GetUpstreamInterface(usI);
					this->mBodies = (v4b2d::BodyDataType^)usI;
					this->isbody = true;
				} 
				catch (System::Exception^ ex)
				{
					this->vInBodies->GetUpstreamInterface(usI);
					this->mGround = (v4b2d::GroundDataType^)usI;
					this->isbody = false;
				}
        	}
			if (Pin == this->vInShapes) 
			{
				v4::INodeIOBase^ usI;
				this->vInShapes->GetUpstreamInterface(usI);
				this->mShapes = (v4b2d::ShapeDefDataType^)usI;
			}
		}

		void Box2dCreateShapeNode::DisconnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInWorld)
        	{
        		this->mWorld = nullptr;
        	}
			if (Pin == this->vInBodies)
        	{
        		if (this->isbody) 
				{
        			this->mBodies = nullptr;
				} 
				else 
				{
					this->mGround = nullptr;
				}
        	}
			if (Pin == this->vInShapes)
        	{
        		this->mShapes = nullptr;
        	}
		}
	}
}
