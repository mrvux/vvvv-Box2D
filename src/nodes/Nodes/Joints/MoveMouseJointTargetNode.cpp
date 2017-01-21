#include "StdAfx.h"
#include "MoveMouseJointTargetNode.h"

#include "../../Internals/Data/JointCustomData.h"

namespace VVVV 
{
	namespace Nodes 
	{

		MoveMouseJointTargetNode::MoveMouseJointTargetNode(void)
		{

		}

		void MoveMouseJointTargetNode::SetPluginHost(IPluginHost^ Host) 
		{
			this->FHost = Host;

			this->FHost->CreateNodeInput("Input",TSliceMode::Dynamic,TPinVisibility::True,this->vInJoints);
			this->vInJoints->SetSubType(ArrayUtils::SingleGuidArray(JointDataType::GUID),JointDataType::FriendlyName);

			this->FHost->CreateValueInput("Target", 2, nullptr , TSliceMode::Dynamic, TPinVisibility::True, this->vInTarget);
			this->vInTarget->SetSubType2D(Double::MinValue, Double::MaxValue, 0.01, 0,0, false,false, false);

			this->FHost->CreateValueInput("Apply", 1, nullptr, TSliceMode::Dynamic, TPinVisibility::True, this->vInApply);
			this->vInApply->SetSubType(0, 1, 1, 0, true,false, false);
		}


		void MoveMouseJointTargetNode::Configurate(IPluginConfig^ Input)
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
							bm->SetTarget(b2Vec2(Convert::ToSingle(tx),Convert::ToSingle(ty)));

							
						}
					}

				}
			}
		}


		void MoveMouseJointTargetNode::ConnectPin(IPluginIO^ Pin)
		{
			if (Pin == this->vInJoints)
        	{
				INodeIOBase^ usI;
				this->vInJoints->GetUpstreamInterface(usI);
				this->m_joints = (JointDataType^)usI;
        	}
		}

		void MoveMouseJointTargetNode::DisconnectPin(IPluginIO^ Pin)
		{
			if (Pin == this->vInJoints)
        	{
        		this->m_joints = nullptr;
        	}
		}
	}
}
