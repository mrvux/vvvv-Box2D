#pragma once

#include "../../DataTypes/WorldDataType.h"
#include "../../DataTypes/Controllers/ControllerDataType.h"

#include "../../Utils/ArrayUtils.h"


namespace v4 = VVVV::PluginInterfaces::V1;
namespace gen = System::Collections::Generic;
namespace v4b2d = VVVV::DataTypes;

namespace VVVV 
{
	namespace Nodes 
	{
		public ref class Box2dBaseControllerDefNode
		{
		public:
			Box2dBaseControllerDefNode(void);
			~Box2dBaseControllerDefNode();

			virtual void SetPluginHost(v4::IPluginHost^ Host);
			virtual void Configurate(v4::IPluginConfig^ Input) {}
			virtual void ConnectPin(v4::IPluginIO^ Pin);
			virtual void DisconnectPin(v4::IPluginIO^ Pin);
			virtual void Evaluate(int SpreadMax);

			virtual property bool AutoEvaluate 
			{
				bool get() { return true; }
			}
		protected:
			virtual void OnPluginHostSet() abstract;
			virtual void OnEvaluate(int SpreadMax, bool reset) abstract;
		

			v4::IPluginHost^ FHost;
			v4::INodeIn^ vInWorld;
			v4b2d::WorldDataType^ m_world;
			v4::IValueIn^ vInClear;

			v4b2d::ControllerDataType^ m_controller;
			v4::INodeOut^ vOutController;
			std::vector<b2Controller*>* ctrl;
			//v4::IValueOut^ vOutBodyCount;
		};


	}
}
