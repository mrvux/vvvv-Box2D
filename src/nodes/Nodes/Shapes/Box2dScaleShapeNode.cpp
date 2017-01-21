
#include "Box2dScaleShapeNode.h"

#include "../../Internals/Data/ShapeCustomData.h"

using namespace VVVV::DataTypes;

namespace VVVV 
{
	namespace Nodes 
	{

		Box2dScaleShapeNode::Box2dScaleShapeNode(void)
		{
		}

		void Box2dScaleShapeNode::SetPluginHost(v4::IPluginHost^ Host)
		{
			this->FHost = Host;

			this->FHost->CreateNodeInput("Shapes",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInShapes);
			this->vInShapes->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(ShapeDataType::GUID),ShapeDataType::FriendlyName);

			this->FHost->CreateValueInput("Factor",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInFactor);
			this->vInFactor->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,false,false,false);

			this->FHost->CreateValueInput("Incremental", 1, nullptr, v4::TSliceMode::Dynamic, v4::TPinVisibility::True, this->vInMode);
			this->vInMode->SetSubType(0, 1, 1, 0, false,true, false);

			this->FHost->CreateValueInput("Apply", 1, nullptr, v4::TSliceMode::Dynamic, v4::TPinVisibility::True, this->vInApply);
			this->vInApply->SetSubType(0, 1, 1, 0, true,false, false);

		}

		
		void Box2dScaleShapeNode::Configurate(v4::IPluginConfig^ Input)
		{

		}

				
		void Box2dScaleShapeNode::ConnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInShapes) 
			{
				v4::INodeIOBase^ usI;
				this->vInShapes->GetUpstreamInterface(usI);
				this->m_shapes = (ShapeDataType^)usI;
			}
		}

		void Box2dScaleShapeNode::Evaluate(int SpreadMax)
		{
			this->vInShapes->PinIsChanged;
			if (this->vInShapes->IsConnected) 
			{
				double dblfactor,dblapply,dblmode;
				for (int i = 0; i < this->vInShapes->SliceCount; i++) 
				{
					int realslice;
					this->vInShapes->GetUpsreamSlice(i % this->vInShapes->SliceCount,realslice);
					b2Shape* shape = this->m_shapes->GetSlice(realslice);

					if (shape->GetType() != e_edgeShape)
					{
						double dblapply;
						this->vInApply->GetValue(i, dblapply);
						if (dblapply >= 0.5)
						{
							double factor,mode;
							this->vInFactor->GetValue(i, factor);
							this->vInMode->GetValue(i, mode);

							if (mode < 0.5)
							{
								factor = System::Math::Abs(factor);
							}

							if (shape->GetType() == e_circleShape)
							{
								b2CircleShape* circle = (b2CircleShape*)shape;

								b2CircleDef* replace = new b2CircleDef();

								replace->radius = circle->GetRadius();
								replace->friction = circle->GetFriction();
								replace->restitution = circle->GetRestitution();
								replace->density = circle->GetDensity();
								replace->isSensor = circle->IsSensor();

								if (mode >= 0.5)
								{
									replace->radius += factor;
								}
								else
								{
									replace->radius *= factor;
								}

								ShapeCustomData* sdata = (ShapeCustomData*)circle->GetUserData();
								sdata->MarkedForUpdate = true;
								sdata->NewShape = replace;

							}

							if (shape->GetType() == e_polygonShape)
							{
								b2PolygonShape* poly = (b2PolygonShape*)shape;
								b2PolygonDef* replace = new b2PolygonDef();
								
								replace->friction = poly->GetFriction();
								replace->restitution = poly->GetRestitution();
								replace->isSensor = poly->IsSensor();
								replace->density = poly->GetDensity();
								replace->vertexCount = poly->GetVertexCount();
								
								if (mode >= 0.5)
								{
									float cx,cy;

									cx = 0.0f;
									cy = 0.0f;
									
									//Need to get center if increment;
									for (int v = 0; v < poly->GetVertexCount(); v++)
									{
										cx += poly->GetVertices()[v].x;
										cy += poly->GetVertices()[v].y;
										
									}

									cx /= (float)poly->GetVertexCount();
									cy /= (float)poly->GetVertexCount();

									for (int v = 0; v < poly->GetVertexCount(); v++)
									{
										float x,y;
										if (poly->GetVertices()[v].x > cx)
										{
											x = poly->GetVertices()[v].x + factor;
										}
										else
										{
											x = poly->GetVertices()[v].x - factor;
										}

										if (poly->GetVertices()[v].y > cy)
										{
											y = poly->GetVertices()[v].y + factor;
										}
										else
										{
											y = poly->GetVertices()[v].y - factor;
										}

										replace->vertices[v].Set(x,y);
									}
									
								}
								else
								{
									for (int v = 0; v < poly->GetVertexCount(); v++)
									{
										replace->vertices[v].Set(poly->GetVertices()[v].x * factor,poly->GetVertices()[v].y * factor);
									}
								}

								ShapeCustomData* sdata = (ShapeCustomData*)poly->GetUserData();
								sdata->MarkedForUpdate = true;
								sdata->NewShape = replace;
							}


						}
					}


					

				}
			}
		}


		void Box2dScaleShapeNode::DisconnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInShapes)
        	{
        		this->m_shapes = nullptr;
        	}
		}
	}
}
