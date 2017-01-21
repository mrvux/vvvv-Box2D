#include "Box2dDestroyJointNode.h"

#include "../../Internals/Data/JointCustomData.h"

namespace VVVV 
{
	namespace Nodes 
	{
		void Box2dDestroyJointNode::SetPluginHost(v4::IPluginHost^ Host) 
		{
			this->FHost = Host;

			this->FHost->CreateNodeInput("Joints",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInJoints);
			this->vInJoints->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::JointDataType::GUID), v4b2d::JointDataType::FriendlyName);

			this->FHost->CreateValueInput("Do Destroy",1,nullptr,v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInDoDestroy);
			this->vInDoDestroy->SetSubType(System::Double::MinValue,System::Double::MaxValue,0.01,0.0,true,false,false);	

			

		}

		void Box2dDestroyJointNode::Configurate(v4::IPluginConfig^ Input)
		{

		}

		void Box2dDestroyJointNode::Evaluate(int SpreadMax)
		{
			this->vInJoints->PinIsChanged;
			if (this->vInJoints->IsConnected) 
			{
				double dbldelete;
				

					//double dblsp,dblsv,dblsav;
					for (int i = 0; i < this->vInJoints->SliceCount; i++) 
					{
						this->vInDoDestroy->GetValue(i, dbldelete);
						if (dbldelete >= 0.5)
						{
							int realslice;
							this->vInJoints->GetUpsreamSlice(i % this->vInJoints->SliceCount,realslice);
							b2Joint* joint = this->m_joints->GetSlice(realslice);

							JointCustomData* bdata = (JointCustomData*)joint->GetUserData();
							bdata->MarkedForDeletion = true;
						}
					}
				
			}
		}



		void Box2dDestroyJointNode::ConnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInJoints) 
			{
				v4::INodeIOBase^ usI;
				this->vInJoints->GetUpstreamInterface(usI);
				this->m_joints = (v4b2d::JointDataType^)usI;
			}
		}


		void Box2dDestroyJointNode::DisconnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInJoints)
        	{
        		this->m_joints = nullptr;
        	}
		}

	}
}
