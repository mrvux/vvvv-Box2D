#pragma once
#include "../../DataTypes/BodyDataType.h"

#include "../../Utils/ArrayUtils.h"

namespace v4 = VVVV::PluginInterfaces::V1;
namespace gen = System::Collections::Generic;
namespace v4b2d = VVVV::DataTypes;

namespace VVVV 
{
	namespace Nodes 
	{
		public ref class Box2dApplyForceNode : v4::IPlugin,v4::IPluginConnections
		{
		public:
			Box2dApplyForceNode(void);

			static property v4::IPluginInfo^ PluginInfo 
				{
					v4::IPluginInfo^ get() 
					{
						//v4::IPluginInfo^ Info;
						v4::IPluginInfo^ Info = gcnew VVVV::PluginInterfaces::V1::PluginInfo();
						Info->Name = "ApplyForce";
						Info->Category = "Box2d";
						Info->Version = "";
						Info->Help = "Apply force on a body";
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

			v4::INodeIn^ vInBodies;
			v4b2d::BodyDataType^ m_bodies;

			v4::IValueIn^ vInForce;
			v4::IValueIn^ vInPosition;
			v4::IValueIn^ vInApply;
		};
	}
}
