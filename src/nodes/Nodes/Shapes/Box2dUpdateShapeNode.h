#pragma once
#include "../../DataTypes/Shapes/ShapeDataType.h"

#include "../../Utils/ArrayUtils.h"

namespace v4 = VVVV::PluginInterfaces::V1;
namespace gen = System::Collections::Generic;
namespace v4b2d = VVVV::DataTypes;

namespace VVVV 
{
namespace Nodes 
{
	public ref class Box2dUpdateShapeNode : v4::IPlugin,v4::IPluginConnections
	{
	public:
		Box2dUpdateShapeNode(void);

		static property v4::IPluginInfo^ PluginInfo 
			{
				v4::IPluginInfo^ get() 
				{
					//v4::IPluginInfo^ Info;
					v4::IPluginInfo^ Info = gcnew VVVV::PluginInterfaces::V1::PluginInfo();
					Info->Name = "UpdateShape";
					Info->Category = "Box2d";
					Info->Version = "";
					Info->Help = "Update box2d shape";
					Info->Bugs = "";
					Info->Credits = "Box2d";
					Info->Warnings = "";
					Info->Author = "vux";
					Info->Tags="Physics,2d,Collision";

					//leave below as is
					System::Diagnostics::StackTrace^ st = gcnew System::Diagnostics::StackTrace(true);
					System::Diagnostics::StackFrame^ sf = st->GetFrame(0);
					System::Reflection::MethodBase^ method = sf->GetMethod();
					Info->Namespace = method->DeclaringType->Namespace;
					Info->Class = method->DeclaringType->Name;
					return Info;
				}
			}

		virtual void SetPluginHost(v4::IPluginHost^ Host);
		virtual void Configurate(v4::IPluginConfig^ Input);
		virtual void Evaluate(int SpreadMax);
		virtual void ConnectPin(v4::IPluginIO^ Pin);
		virtual void DisconnectPin(v4::IPluginIO^ Pin);
		
		virtual property bool AutoEvaluate 
		{
			bool get() { return true; }
		}

	private:
		v4::IPluginHost^ FHost;

		v4::INodeIn^ vInShapes;
		v4b2d::ShapeDataType^ m_shapes;

		v4::IValueIn^ vInDensity;
		v4::IValueIn^ vInSetDensity;

		v4::IValueIn^ vInFriction;
		v4::IValueIn^ vInSetFriction;

		v4::IValueIn^ vInRestitution;
		v4::IValueIn^ vInSetRestitution;

		v4::IValueIn^ vInGroup;
		v4::IValueIn^ vInSetGroup;

		v4::IStringIn^ vInCustom;
		v4::IValueIn^ vInSetCustom;

		v4::IValueIn^ vInSensor;
		v4::IValueIn^ vInSetSensor;

	};
}
}
