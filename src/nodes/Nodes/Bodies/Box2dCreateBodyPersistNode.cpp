#include "Box2dCreateBodyPersistNode.h"
#include "../../Internals/Data/BodyCustomData.h"
#include "../../Internals/Data/ShapeCustomData.h"

namespace VVVV 
{
	namespace Nodes 
	{
		Box2dCreateBodyPersistNode::Box2dCreateBodyPersistNode(void)
		{
			this->mBodies = gcnew v4b2d::BodyDataType();
			this->m_bodies = new std::map<int,b2Body*>();
		}

		void Box2dCreateBodyPersistNode::SetPluginHost(v4::IPluginHost^ Host) 
		{
			this->FHost = Host;

			//World input
			this->FHost->CreateNodeInput("World",v4::TSliceMode::Single,v4::TPinVisibility::True,this->vInWorld);
			this->vInWorld->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::WorldDataType::GUID), v4b2d::WorldDataType::FriendlyName);

			this->FHost->CreateNodeInput("Shapes",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInShapes);
			this->vInShapes->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::ShapeDefDataType::GUID), v4b2d::ShapeDefDataType::FriendlyName);

			//Position and velocity
			this->FHost->CreateValueInput("Position",2,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInPosition);
			this->vInPosition->SetSubType2D(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,0.0,false,false,false);

			this->FHost->CreateValueInput("Angle",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInAngle);
			this->vInAngle->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,false,false,false);

			this->FHost->CreateValueInput("Velocity",2,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInVelocity);
			this->vInVelocity->SetSubType2D(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,0.0,false,false,false);

			this->FHost->CreateValueInput("Angular Velocity",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInAngularVelocity);
			this->vInAngularVelocity->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,false,false,false);

			this->FHost->CreateValueInput("Linear Damping",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInLinearDamping);
			this->vInLinearDamping->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,false,false,false);

			this->FHost->CreateValueInput("Angular Damping",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInAngularDamping);
			this->vInAngularDamping->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,false,false,false);

			this->FHost->CreateValueInput("Fixed Rotation",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInFixedRotation);
			this->vInFixedRotation->SetSubType(0,1,1.0,0.0,false,true,false);

			this->FHost->CreateValueInput("Is Bullet",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInIsBullet);
			this->vInIsBullet->SetSubType(0,1,1.0,0.0,false,true,false);

			this->FHost->CreateStringInput("Custom",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInCustom);
			this->vInCustom->SetSubType("",false);

			this->FHost->CreateValueInput("TTL",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInTTL);
			this->vInTTL->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,1.0,false,false,false);

			this->FHost->CreateValueInput("Has TTL",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInHasTTL);
			this->vInHasTTL->SetSubType(0,1,1.0,0.0,false,true,false);

			this->FHost->CreateValueInput("Do Create",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInDoCreate);
			this->vInDoCreate->SetSubType(0,1,1.0,0.0,true,false,false);

			this->FHost->CreateNodeOutput("Body",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutBodies);
			this->vOutBodies->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::BodyDataType::GUID), v4b2d::BodyDataType::FriendlyName);
			this->vOutBodies->SetInterface(this->mBodies);

			this->FHost->CreateValueOutput("Body Id",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutId);
			this->vOutId->SetSubType(System::Double::MinValue,System::Double::MaxValue,1,0.0,false,false,true);

			this->FHost->CreateValueOutput("Is New", 1, nullptr, v4::TSliceMode::Dynamic, v4::TPinVisibility::True, this->vOutIsNew);
			this->vOutIsNew->SetSubType(0, 1, 1, 0, true,false, false);


			
		}

		void Box2dCreateBodyPersistNode::Configurate(v4::IPluginConfig^ Input)
		{

		}

		void Box2dCreateBodyPersistNode::Evaluate(int SpreadMax)
		{
			//Remove deleted bodies if applicable
			std::map<int,b2Body*>::iterator itbody;
			if (this->vInWorld->IsConnected)
			{
				if (this->mWorld->Reset)
				{
					this->m_bodies->clear();
				}
				else
				{
					for (int i = 0; i < this->mWorld->DeletedIds->size(); i++)
					{
						itbody=this->m_bodies->find(this->mWorld->DeletedIds->at(i));

						if (itbody != this->m_bodies->end())
						{
							this->m_bodies->erase(itbody); 
						}
					}
				}
			}

			double dblcreate;
			this->vInDoCreate->GetValue(0,dblcreate);

			this->mBodies->Reset();

			int shapeidx = 0;

			if (dblcreate >= 0.5 && this->vInWorld->IsConnected && this->vInShapes->IsConnected) 
			{
				if (this->mWorld->GetIsValid()) 
				{
					double x,y,a,vx,vy,va,bull,fr,ld,ad,dblhasttl,dblttl;
					System::String^ cust;
					
					for (int i = 0; i < SpreadMax; i++) 
					{
						this->vInPosition->GetValue2D(i,x,y);
						this->vInVelocity->GetValue2D(i,vx,vy);
						this->vInLinearDamping->GetValue(i,ld);
						this->vInAngularDamping->GetValue(i,ad);
						this->vInAngularVelocity->GetValue(i,va);
						this->vInIsBullet->GetValue(i,bull);
						this->vInFixedRotation->GetValue(i,fr);
						this->vInCustom->GetString(i,cust);
						this->vInAngle->GetValue(i,a);
						this->vInTTL->GetValue(i,dblttl);
						this->vInHasTTL->GetValue(i,dblhasttl);


						b2BodyDef bodydef;
						bodydef.position.Set(System::Convert::ToSingle(x), System::Convert::ToSingle(y));
						bodydef.isBullet = (bull >= 0.5);
						bodydef.fixedRotation = (fr >= 0.5);
						bodydef.angle = System::Convert::ToSingle(a * (System::Math::PI * 2.0));
						bodydef.linearDamping = System::Convert::ToSingle(ld);
						bodydef.angularDamping = System::Convert::ToSingle(ad);
					
						BodyCustomData* bdata = new BodyCustomData();
						
						bdata->Id = this->mWorld->GetNewBodyId();
						bdata->Custom = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(cust);
						bdata->Created = true;
						bdata->Persisted = true;
						bdata->HasTTL = dblhasttl > 0.5;
						bdata->TTL = dblttl;

						bool testcount;
						int vcount = 0;

						int realslice;
						this->vInShapes->GetUpsreamSlice(i % this->vInShapes->SliceCount,realslice);
						b2ShapeDef* shapedef = this->mShapes->GetSlice(realslice);

						if (shapedef->type == e_edgeShape)
						{
							b2EdgeChainDef* chain = (b2EdgeChainDef*)shapedef;
							vcount += chain->vertexCount;
							if (chain->isALoop)
							{
								vcount++;
							}
						}
						else
						{
							vcount++;
						}
						int prx = this->mWorld->GetWorld()->GetProxyCount();
						testcount = this->mWorld->GetWorld()->GetProxyCount() + vcount <= b2_maxProxies;
					
						

						if (testcount)
						{
							b2Body* body = this->mWorld->GetWorld()->CreateBody(&bodydef);
							body->SetLinearVelocity(b2Vec2(System::Convert::ToSingle(vx), System::Convert::ToSingle(vy)));
							body->SetAngularVelocity(System::Convert::ToSingle(va));
							body->SetUserData(bdata);

							float dens = 0.0f;

							int realslice;
							this->vInShapes->GetUpsreamSlice(i % this->vInShapes->SliceCount,realslice);
					
							b2ShapeDef* shapedef = this->mShapes->GetSlice(realslice);
							System::String^ shapecust = this->mShapes->GetCustom(realslice);

							dens += shapedef->density;


							b2Shape* shape = body->CreateShape(shapedef);
							
							if (shape->GetType() == e_edgeShape)
							{
								b2Shape* node = body->GetShapeList();
								while(node)
								{
									if (node->GetUserData() == nullptr)
									{
										ShapeCustomData* sdata = new ShapeCustomData();
										sdata->Id = this->mWorld->GetNewShapeId();
										sdata->Custom = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(shapecust);
										node->SetUserData(sdata);										
									}

									node = node->GetNext();
								}

							}
							else
							{
								ShapeCustomData* sdata = new ShapeCustomData();
								sdata->Id = this->mWorld->GetNewShapeId();
								sdata->Custom = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(shapecust);
								shape->SetUserData(sdata);
							}


							if (dens != 0.0) 
							{
								body->SetMassFromShapes();
							}

							this->m_bodies->insert(std::pair<int,b2Body*>(bdata->Id,body));
						}
						else
						{
							this->FHost->Log(v4::TLogType::Debug,"Test Count Failed");
						}
					}
				}

				
			} 

			this->vOutBodies->SliceCount = this->m_bodies->size();
			this->vOutBodies->MarkPinAsChanged();
			this->vOutId->SliceCount = m_bodies->size();
			this->vOutIsNew->SliceCount = this->m_bodies->size();

			std::map<int,b2Body*>::iterator it;


			int i = 0;
			for ( it=this->m_bodies->begin() ; it != this->m_bodies->end(); it++ )
			{
				b2Body* body = it->second;
				BodyCustomData* bdata = (BodyCustomData*)body->GetUserData();
				this->mBodies->Add(body);
				this->vOutIsNew->SetValue(i,System::Convert::ToDouble(bdata->Created));
				this->vOutId->SetValue(i,bdata->Id);
				i++;
			}

		}


		void Box2dCreateBodyPersistNode::ConnectPin(v4::IPluginIO^ Pin)
		{
			//cache a reference to the upstream interface when the NodeInput pin is being connected
        	if (Pin == this->vInWorld)
        	{
				v4::INodeIOBase^ usI;
				this->vInWorld->GetUpstreamInterface(usI);
				this->mWorld = (v4b2d::WorldDataType^)usI;
        	}
			if (Pin == this->vInShapes) 
			{
				v4::INodeIOBase^ usI;
				this->vInShapes->GetUpstreamInterface(usI);
				this->mShapes = (v4b2d::ShapeDefDataType^)usI;
			}
		}


		void Box2dCreateBodyPersistNode::DisconnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInWorld)
        	{
        		this->mWorld = nullptr;
				//First loop to unpersist them
				std::map<int,b2Body*>::iterator it;
				int i = 0;
				for ( it=this->m_bodies->begin() ; it != this->m_bodies->end(); it++ )
				{
					b2Body* body = it->second;
					BodyCustomData* bdata = (BodyCustomData*)body->GetUserData();
					bdata->Persisted = false;			
				}

				this->m_bodies->clear();
        	}
			if (Pin == this->vInShapes)
        	{
        		this->mShapes = nullptr;
        	}
		}
	}
}
