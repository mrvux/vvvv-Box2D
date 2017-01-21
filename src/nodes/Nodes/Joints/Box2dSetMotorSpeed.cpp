
#include "Box2dSetMotorSpeed.h"

#include "../../Internals/Data/JointCustomData.h"

namespace VVVV 
{
	namespace Nodes 
	{
		void Box2dSetMotorSpeedNode::SetPluginHost(v4::IPluginHost^ Host) 
		{
			this->FHost = Host;

			this->FHost->CreateNodeInput("Joints",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInJoints);
			this->vInJoints->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::JointDataType::GUID),v4b2d::JointDataType::FriendlyName);

			this->FHost->CreateValueInput("Speed",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInSpeed);
			this->vInSpeed->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,false,false,false);	

			this->FHost->CreateValueInput("Update", 1, nullptr, v4::TSliceMode::Dynamic, v4::TPinVisibility::True, this->vInUpdate);
			this->vInUpdate->SetSubType(0, 1, 1, 0, true,false, false);

		}

		void Box2dSetMotorSpeedNode::Configurate(v4::IPluginConfig^ Input)
		{

		}

		void Box2dSetMotorSpeedNode::Evaluate(int SpreadMax)
		{
			this->vInJoints->PinIsChanged;
			if (this->vInJoints->IsConnected) 
			{
				double dblupdate;
				

					//double dblsp,dblsv,dblsav;
					for (int i = 0; i < this->vInJoints->SliceCount; i++) 
					{
						this->vInUpdate->GetValue(i, dblupdate);
						if (dblupdate >= 0.5)
						{
							int realslice;
							this->vInJoints->GetUpsreamSlice(i % this->vInJoints->SliceCount,realslice);
							b2Joint* joint = this->m_joints->GetSlice(realslice);

							if (joint->GetType() == e_revoluteJoint)
							{
								b2RevoluteJoint* rev = (b2RevoluteJoint*)joint;

								if (rev->IsMotorEnabled())
								{
									double dblspeed;
									this->vInSpeed->GetValue(i, dblspeed);
									rev->SetMotorSpeed(System::Convert::ToInt32(dblspeed));
								}
							}

							if (joint->GetType() == e_prismaticJoint)
							{
								b2PrismaticJoint* pri = (b2PrismaticJoint*)joint;

								if (pri->IsMotorEnabled())
								{
									double dblspeed;
									this->vInSpeed->GetValue(i, dblspeed);
									pri->SetMotorSpeed(System::Convert::ToInt32(dblspeed));
								}
							}

							//JointCustomData* bdata = (JointCustomData*)joint->GetUserData();
							//bdata->MarkedForDeletion = true;
						}
					}
				
			}
		}



		void Box2dSetMotorSpeedNode::ConnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInJoints) 
			{
				v4::INodeIOBase^ usI;
				this->vInJoints->GetUpstreamInterface(usI);
				this->m_joints = (v4b2d::JointDataType^)usI;
			}
		}


		void Box2dSetMotorSpeedNode::DisconnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInJoints)
        	{
        		this->m_joints = nullptr;
        	}
		}

	}
}
