
#include "MoveMouseJointTargetNode.h"

#include "../../Internals/Data/JointCustomData.h"

namespace VVVV 
{
	namespace Nodes 
	{

		MoveMouseJointTargetNode::MoveMouseJointTargetNode(void)
		{

		}

		void MoveMouseJointTargetNode::SetPluginHost(v4::IPluginHost^ Host)
		{
			this->FHost = Host;

			this->FHost->CreateNodeInput("Input",v4::TSliceMode::Dynamic,v4::TPinVisibility::True,this->vInJoints);
			this->vInJoints->SetSubType(VVVV::Utils::ArrayUtils::SingleGuidArray(v4b2d::JointDataType::GUID), v4b2d::JointDataType::FriendlyName);

			this->FHost->CreateValueInput("Target", 2, nullptr , v4::TSliceMode::Dynamic, v4::TPinVisibility::True, this->vInTarget);
			this->vInTarget->SetSubType2D(System::Double::MinValue, System::Double::MaxValue, 0.01, 0,0, false,false, false);

			this->FHost->CreateValueInput("Apply", 1, nullptr, v4::TSliceMode::Dynamic, v4::TPinVisibility::True, this->vInApply);
			this->vInApply->SetSubType(0, 1, 1, 0, true,false, false);
		}


		void MoveMouseJointTargetNode::Configurate(v4::IPluginConfig^ Input)
		{

		}

		
		void MoveMouseJointTargetNode::Evaluate(int SpreadMax)
		{
			if (this->vInJoints->IsConnected) 
			{
				
				for (int i = 0; i < this->vInJoints->SliceCount; i++) 
				{
					b2Joint* joint;

					int realslice;
					this->vInJoints->GetUpsreamSlice(i % this->vInJoints->SliceCount,realslice);
					joint = this->m_joints->GetSlice(realslice);

					if (joint->GetType() == e_mouseJoint)
					{
						double dblapply;
						this->vInApply->GetValue(i, dblapply);

						if (dblapply >= 0.5)
						{
							double tx,ty;
							this->vInTarget->GetValue2D(i,tx,ty);

							b2MouseJoint* bm = (b2MouseJoint*) joint;
							bm->SetTarget(b2Vec2(System::Convert::ToSingle(tx), System::Convert::ToSingle(ty)));

							
						}
					}

				}
			}
		}


		void MoveMouseJointTargetNode::ConnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInJoints)
        	{
				v4::INodeIOBase^ usI;
				this->vInJoints->GetUpstreamInterface(usI);
				this->m_joints = (v4b2d::JointDataType^)usI;
        	}
		}

		void MoveMouseJointTargetNode::DisconnectPin(v4::IPluginIO^ Pin)
		{
			if (Pin == this->vInJoints)
        	{
        		this->m_joints = nullptr;
        	}
		}
	}
}
