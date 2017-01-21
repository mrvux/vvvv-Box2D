#include "StdAfx.h"
#include "Box2dSetMotorSpeed.h"

#include "../../Internals/Data/JointCustomData.h"

namespace VVVV 
{
	namespace Nodes 
	{
		void Box2dSetMotorSpeedNode::SetPluginHost(IPluginHost^ Host) 
		{
			this->FHost = Host;

			this->FHost->CreateNodeInput("Joints",TSliceMode::Dynamic,TPinVisibility::True,this->vInJoints);
			this->vInJoints->SetSubType(ArrayUtils::SingleGuidArray(JointDataType::GUID),JointDataType::FriendlyName);

			this->FHost->CreateValueInput("Speed",1,nullptr,TSliceMode::Dynamic,TPinVisibility::True,this->vInSpeed);
			this->vInSpeed->SetSubType(Double::MinValue,Double::MaxValue,0.01,0.0,false,false,false);	

			this->FHost->CreateValueInput("Update", 1, nullptr, TSliceMode::Dynamic, TPinVisibility::True, this->vInUpdate);
			this->vInUpdate->SetSubType(0, 1, 1, 0, true,false, false);

		}

		void Box2dSetMotorSpeedNode::Configurate(IPluginConfig^ Input)
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
									rev->SetMotorSpeed(Convert::ToInt32(dblspeed));
								}
							}

							if (joint->GetType() == e_prismaticJoint)
							{
								b2PrismaticJoint* pri = (b2PrismaticJoint*)joint;

								if (pri->IsMotorEnabled())
								{
									double dblspeed;
									this->vInSpeed->GetValue(i, dblspeed);
									pri->SetMotorSpeed(Convert::ToInt32(dblspeed));
								}
							}

							//JointCustomData* bdata = (JointCustomData*)joint->GetUserData();
							//bdata->MarkedForDeletion = true;
						}
					}
				
			}
		}



		void Box2dSetMotorSpeedNode::ConnectPin(IPluginIO^ Pin)
		{
			if (Pin == this->vInJoints) 
			{
				INodeIOBase^ usI;
				this->vInJoints->GetUpstreamInterface(usI);
				this->m_joints = (JointDataType^)usI;
			}
		}


		void Box2dSetMotorSpeedNode::DisconnectPin(IPluginIO^ Pin)
		{
			if (Pin == this->vInJoints)
        	{
        		this->m_joints = nullptr;
        	}
		}

	}
}
