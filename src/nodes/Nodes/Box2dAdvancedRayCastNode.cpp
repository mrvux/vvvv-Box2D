
#include "Box2dAdvancedRayCastNode.h"


namespace VVVV 
{
	namespace Nodes 
	{
		Box2dAdvancedRayCastNode::Box2dAdvancedRayCastNode(void)
		{
			this->mBodies = gcnew v4b2d::BodyDataType();
			this->mShapes = gcnew v4b2d::ShapeDataType();
		}

		void Box2dAdvancedRayCastNode::SetPluginHost(v4::IPluginHost^ Host) 
		{
			this->FHost = Host;

			this->FHost->CreateNodeInput("World",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInWorld);
			this->vInWorld->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::WorldDataType::GUID), v4b2d::WorldDataType::FriendlyName);

			this->FHost->CreateValueInput("Origin",2,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInStartPos);
			this->vInStartPos->SetSubType2D(System::Double::MinValue,System::Double::MaxValue,0.01,-0.1,-0.1,false,false,false);

			this->FHost->CreateValueInput("Destination",2,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInEndPos);
			this->vInEndPos->SetSubType2D(System::Double::MinValue,System::Double::MaxValue,0.01,0.1,0.1,false,false,false);

			this->FHost->CreateValueInput("Bounces Count", 1, nullptr, v4::TSliceMode::Dynamic, v4::TPinVisibility::True, this->vInMaxCount);
			this->vInMaxCount->SetSubType(1, System::Double::MaxValue, 1, 1, false,false, true);

			this->FHost->CreateValueInput("Do Query",1,nullptr,v4::TSliceMode::Single,v4::TPinVisibility::True,this->vInDoQuery);
			this->vInDoQuery->SetSubType(0,1,1,0.0,true,false,false);

			this->FHost->CreateValueOutput("Shapes Count",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutShapeCount);
			this->vOutShapeCount->SetSubType(0,System::Double::MaxValue,1,0.0,true,false,true);

			this->FHost->CreateValueOutput("Segments Count",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutSegmentsCount);
			this->vOutSegmentsCount->SetSubType(0,System::Double::MaxValue,1,0.0,true,false,true);

			this->FHost->CreateValueOutput("Vertices",2,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutSegments);
			this->vOutSegments->SetSubType2D(System::Double::MinValue,System::Double::MaxValue,0.01,0.1,0.1,false,false,false);

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

		void Box2dAdvancedRayCastNode::Configurate(v4::IPluginConfig^ Input)
		{

		}


		void Box2dAdvancedRayCastNode::Evaluate(int SpreadMax)
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

					std::vector<int> shapeids;
					std::vector<int> bodyids;
					
					std::vector<b2Vec2> verts;

					std::vector<int> shapecount;
					std::vector<int> intercount;

					if (dblquery >= 0.5) 
					{
						for (int i = 0; i < SpreadMax; i++) 
						{
							double ox,oy,tx,ty;
							double max;
							this->vInStartPos->GetValue2D(i,ox,oy);
							this->vInEndPos->GetValue2D(i,tx,ty);
							this->vInMaxCount->GetValue(i,max);

							b2Segment segment;
							segment.p1 = b2Vec2(ox,oy);
							segment.p2 = b2Vec2(tx,ty);

							float segmentLength = b2Distance(segment.p1,segment.p2);

							verts.push_back(segment.p1);
							int vcnt = 1;
							int scnt = 0;

							for(int32 rebounds=0;rebounds<max;rebounds++)
							{
								float32 lambda=1;
								b2Vec2 normal;
								b2Shape* shape = m_world->GetWorld()->RaycastOne(segment,&lambda,&normal,false,NULL);

								if (shape)
								{
									vcnt++;
									scnt++;
									verts.push_back((1-lambda)*segment.p1+lambda*segment.p2);
									shapes.push_back(shape);

									bodies.push_back(shape->GetBody());	

									ShapeCustomData* sdata  = (ShapeCustomData*)shape->GetUserData();
									shapeids.push_back(sdata->Id);

									BodyCustomData* bdata = (BodyCustomData*)shape->GetBody()->GetUserData();
									bodyids.push_back(bdata->Id);
								}
								else
								{
									vcnt++;
									verts.push_back(segment.p2);
									break;
								}

								segmentLength *=(1-lambda);
								if(segmentLength<=B2_FLT_EPSILON)
								{
									break;
								}

								//b2Segment segment;
								b2Vec2 newStart = (1-lambda)*segment.p1+lambda*segment.p2;
								
								b2Vec2 newDir = segment.p2-segment.p1;
								newDir.Normalize();
								newDir = newDir -2 * b2Dot(newDir,normal) * normal;
								segment.p1 = newStart-0.1f*newDir;
								segment.p2 = newStart+segmentLength*newDir;

							}

							shapecount.push_back(scnt);
							intercount.push_back(vcnt);

						}

						this->vOutSegmentsCount->SliceCount = intercount.size();
						this->vOutSegments->SliceCount = verts.size();
						this->vOutShapeCount->SliceCount = shapecount.size();
						this->vOutShapes->SliceCount = shapes.size();
						this->vOutBodies->SliceCount = shapes.size();
						this->vOutBodyId->SliceCount = shapes.size();
						this->vOutShapeId->SliceCount = shapes.size();

						for (int i = 0; i < intercount.size();i++)
						{
							this->vOutSegmentsCount->SetValue(i,intercount.at(i));
						}

						for (int i = 0; i < shapecount.size();i++)
						{
							this->vOutShapeCount->SetValue(i,shapecount.at(i));
						}

						for (int i = 0; i < verts.size();i++) 
						{
							this->vOutSegments->SetValue2D(i,verts.at(i).x,verts.at(i).y);
						}

						for (int i = 0; i < shapes.size();i++) 
						{
							this->mShapes->Add(shapes.at(i));
							this->mBodies->Add(bodies.at(i));
							this->vOutBodyId->SetValue(i,bodyids.at(i));
							this->vOutShapeId->SetValue(i,shapeids.at(i));
						}
					}
					else
					{
						this->vOutSegmentsCount->SliceCount = 0;
						this->vOutShapeCount->SliceCount = 0;
						this->vOutSegments->SliceCount = 0;
						this->vOutShapes->SliceCount = 0;
						this->vOutBodies->SliceCount = 0;
						this->vOutBodyId->SliceCount = 0;
						this->vOutShapeId->SliceCount = 0;
					}


				}
				else 
				{
					this->vOutSegmentsCount->SliceCount = 0;
					this->vOutShapeCount->SliceCount = 0;
					this->vOutSegments->SliceCount = 0;
					this->vOutShapes->SliceCount = 0;
					this->vOutBodies->SliceCount = 0;
					this->vOutBodyId->SliceCount = 0;
					this->vOutShapeId->SliceCount = 0;
				}
			}
			else 
			{
				this->vOutSegmentsCount->SliceCount = 0;
				this->vOutShapeCount->SliceCount = 0;
				this->vOutSegments->SliceCount = 0;
				this->vOutShapes->SliceCount = 0;
				this->vOutBodies->SliceCount = 0;
				this->vOutBodyId->SliceCount = 0;
				this->vOutShapeId->SliceCount = 0;
			}
		}

		void Box2dAdvancedRayCastNode::ConnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInWorld) 
			{
				v4::INodeIOBase^ usI;
				this->vInWorld->GetUpstreamInterface(usI);
				this->m_world = (v4b2d::WorldDataType^)usI;
			}
		}

		void Box2dAdvancedRayCastNode::DisconnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInWorld)
        	{
        		this->m_world = nullptr;
        	}
		}

	}
}


/*

		for(int32 rebounds=0;rebounds<10;rebounds++){

			float32 lambda=1;
			b2Vec2 normal;
			b2Shape* shape = m_world->RaycastOne(segment,&lambda,&normal,false,NULL);

			b2Color laserColor(255,0,0);

			if(shape)
			{
				m_debugDraw.DrawSegment(segment.p1,(1-lambda)*segment.p1+lambda*segment.p2,laserColor);
			}
			else
			{
				m_debugDraw.DrawSegment(segment.p1,segment.p2,laserColor);
				break;
			}
			//Bounce
			segmentLength *=(1-lambda);
			if(segmentLength<=B2_FLT_EPSILON)
				break;
			laserStart = (1-lambda)*segment.p1+lambda*segment.p2;
			laserDir = segment.p2-segment.p1;
			laserDir.Normalize();
			laserDir = laserDir -2 * b2Dot(laserDir,normal) * normal;
			segment.p1 = laserStart-0.1f*laserDir;
			segment.p2 = laserStart+segmentLength*laserDir;
		}
*/