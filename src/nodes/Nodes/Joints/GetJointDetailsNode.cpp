
#include "GetJointDetailsNode.h"

#include "../../Internals/Data/JointCustomData.h"

namespace VVVV 
{
	namespace Nodes 
	{

		GetJointDetailsNode::GetJointDetailsNode(void)
		{
			this->m_bodies1 = gcnew v4b2d::BodyDataType();
			this->m_bodies2 = gcnew v4b2d::BodyDataType();
		}

		void GetJointDetailsNode::SetPluginHost(v4::IPluginHost^ Host)
		{
			this->FHost = Host;

			this->FHost->CreateNodeInput("Input",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInJoints);
			this->vInJoints->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::JointDataType::GUID), v4b2d::JointDataType::FriendlyName);

			this->FHost->CreateStringOutput("Type",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutType);
			this->vOutType->SetSubType("",false);

			this->FHost->CreateNodeOutput("Body 1",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutBody1);
			this->vOutBody1->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::BodyDataType::GUID), v4b2d::BodyDataType::FriendlyName);
			this->vOutBody1->SetInterface(this->m_bodies1);

			this->FHost->CreateValueOutput("Anchor 1",2,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutPosition1);
			this->vOutPosition1->SetSubType2D(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,0.0,false,false,false);

			this->FHost->CreateNodeOutput("Body 2",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutBody2);
			this->vOutBody2->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::BodyDataType::GUID), v4b2d::BodyDataType::FriendlyName);
			this->vOutBody2->SetInterface(this->m_bodies2);

			this->FHost->CreateValueOutput("Anchor 2",2,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutPosition2);
			this->vOutPosition2->SetSubType2D(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,0.0,false,false,false);

			//this->FHost->CreateValueOutput("Force",2,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutPosition1);
			//this->vOutPosition1->SetSubType2D(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,0.0,false,false,false);

			this->FHost->CreateStringOutput("Custom",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutCustom);
			this->vOutCustom->SetSubType("",false);

			this->FHost->CreateValueOutput("Joint Id",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutId);
			this->vOutId->SetSubType(System::Double::MinValue,System::Double::MaxValue,1,0.0,false,false,true);

			this->FHost->CreateValueOutput("LifeTime",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vOutLifeTime);
			this->vOutLifeTime->SetSubType(System::Double::MinValue,System::Double::MaxValue,1,0.0,false,false,false);

		}


		void GetJointDetailsNode::Configurate(v4::IPluginConfig^ Input)
		{

		}

		
		void GetJointDetailsNode::Evaluate(int SpreadMax)
		{
			if (this->vInJoints->IsConnected) 
			{
				
				this->m_bodies1->Reset();
				this->m_bodies2->Reset();

				int cnt = this->vInJoints->SliceCount;
				this->vOutBody1->SliceCount = cnt;
				this->vOutPosition1->SliceCount = cnt;
				this->vOutBody2->SliceCount = cnt;
				this->vOutPosition2->SliceCount = cnt;
				this->vOutId->SliceCount = cnt;
				this->vOutCustom->SliceCount = cnt;
				this->vOutType->SliceCount = cnt;
				this->vOutLifeTime->SliceCount = cnt;

				for (int i = 0; i < this->vInJoints->SliceCount; i++) 
				{
					b2Joint* joint;

					int realslice;
					this->vInJoints->GetUpsreamSlice(i % this->vInJoints->SliceCount,realslice);
					joint = this->m_joints->GetSlice(realslice);

					JointCustomData* jdata = (JointCustomData*)joint->GetUserData();
					System::String^ str = gcnew System::String(jdata->Custom);
					this->vOutId->SetValue(i, jdata->Id);
					this->vOutLifeTime->SetValue(i,jdata->LifeTime);
					this->vOutCustom->SetString(i,str);

					this->m_bodies1->Add(joint->GetBody1());
					this->m_bodies2->Add(joint->GetBody2());

					b2Vec2 pos1 = joint->GetAnchor1();
					this->vOutPosition1->SetValue2D(i, pos1.x,pos1.y);

					b2Vec2 pos2 = joint->GetAnchor2();
					this->vOutPosition2->SetValue2D(i, pos2.x,pos2.y);

					System::String^ type;
					switch (joint->GetType())
					{
					case e_distanceJoint:
						type = "Distance";
						break;	
					case e_revoluteJoint:
						type = "Revolute";
						break;
					case e_prismaticJoint:
						type = "Prismatic";
						break;
					case e_pulleyJoint:
						type = "Pulley";
						break;
					case e_mouseJoint:
						type = "Mouse";
						break;
					case e_lineJoint:
						type = "Line";
						break;
					case e_gearJoint:
						type = "Gear";
						break;
					default:
						type = "Unknown";
						break;
					}

					this->vOutType->SetString(i, type);

				}
			}
		}


		void GetJointDetailsNode::ConnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInJoints)
        	{
				v4::INodeIOBase^ usI;
				this->vInJoints->GetUpstreamInterface(usI);
				this->m_joints = (v4b2d::JointDataType^)usI;
        	}
		}

		void GetJointDetailsNode::DisconnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInJoints)
        	{
        		this->m_joints = nullptr;
        	}
		}
	}
}
