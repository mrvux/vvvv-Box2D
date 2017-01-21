#pragma once
#include "../../../DataTypes/BodyDataType.h"
#include "../../../DataTypes/GroundDataType.h"
#include "../../../DataTypes/WorldDataType.h"
#include "../../../Internals/Data/JointCustomData.h"

#include "../../../Utils/ArrayUtils.h"

namespace v4 = VVVV::PluginInterfaces::V1;
namespace gen = System::Collections::Generic;
namespace v4b2d = VVVV::DataTypes;

namespace VVVV 
{
	namespace Nodes 
	{
		public ref class Box2dCreateJointNode
		{
		public:
			Box2dCreateJointNode(void);

			virtual void SetPluginHost(v4::IPluginHost^ Host);
			virtual void Configurate(v4::IPluginConfig^ Input);
			virtual void ConnectPin(v4::IPluginIO^ Pin);
			virtual void DisconnectPin(v4::IPluginIO^ Pin);

			virtual void Evaluate(int SpreadMax) abstract;
			
			virtual property bool AutoEvaluate 
			{
				bool get() { return true; }
			}
	
		protected:
			v4::IPluginHost^ FHost;

			v4::INodeIn^ vInWorld;
			v4b2d::WorldDataType^ mWorld;

			v4::INodeIn^ vInBody1;
			v4b2d::BodyDataType^ m_body1;
			v4b2d::GroundDataType^ m_ground1;
			bool isbody;

			v4::INodeIn^ vInBody2;
			v4b2d::BodyDataType^ m_body2;

			v4::IValueIn^ vInCollideConnected;

			v4::IStringIn^ vInCustom;
			v4::IValueIn^ vInDoCreate;

			v4::IValueOut^ vOutId;

			virtual void OnPluginHostSet() abstract;
			virtual bool ForceBodyOneGround() abstract;

		private:
			

		};
	}
}
