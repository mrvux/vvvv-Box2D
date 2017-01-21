#pragma once

#include "../../DataTypes/BodyDataType.h"
#include "../../DataTypes/JointDataType.h"

#include "../../Utils/ArrayUtils.h"

namespace v4 = VVVV::PluginInterfaces::V1;
namespace gen = System::Collections::Generic;
namespace v4b2d = VVVV::DataTypes;

namespace VVVV 
{
	namespace Nodes 
	{
		public ref class GetJointDetailsNode : v4::IPlugin,v4::IPluginConnections
		{
		public:
			GetJointDetailsNode(void);

			virtual void SetPluginHost(v4::IPluginHost^ Host);
			virtual void Configurate(v4::IPluginConfig^ Input);
			virtual void Evaluate(int SpreadMax);
			virtual void ConnectPin(v4::IPluginIO^ Pin);
			virtual void DisconnectPin(v4::IPluginIO^ Pin);
			
			virtual property bool AutoEvaluate 
			{
				bool get() { return false; }
			}

			static property v4::IPluginInfo^ PluginInfo 
			{
				v4::IPluginInfo^ get() 
					{
						//v4::IPluginInfo^ Info;
						v4::IPluginInfo^ Info = gcnew VVVV::PluginInterfaces::V1::PluginInfo();
						Info->Name = "GetJointDetails";
						Info->Category = "Box2d";
						Info->Version = "";
						Info->Help = "Get details about a created box2d joint";
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



		private:
			v4::IPluginHost^ FHost;

			v4::INodeIn^ vInJoints;
			v4b2d::JointDataType^ m_joints;


			v4::INodeOut^ vOutBody1;
			v4b2d::BodyDataType^ m_bodies1;
			v4::IValueOut^ vOutPosition1;

			v4::INodeOut^ vOutBody2;
			v4b2d::BodyDataType^ m_bodies2;
			v4::IValueOut^ vOutPosition2;

			v4::IStringOut^ vOutType;

			v4::IStringOut^ vOutCustom;
			v4::IValueOut^ vOutId;
			v4::IValueOut^ vOutLifeTime;

		};
	}
}

