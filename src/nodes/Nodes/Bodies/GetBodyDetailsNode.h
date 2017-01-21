#pragma once
#include "../../DataTypes/BodyDataType.h"
#include "../../DataTypes/GroundDataType.h"
#include "../../DataTypes/Shapes/ShapeDataType.h"

#include "../../Utils/ArrayUtils.h"

namespace v4 = VVVV::PluginInterfaces::V1;
namespace gen = System::Collections::Generic;
namespace v4b2d = VVVV::DataTypes;
namespace v4math = VVVV::Utils::VMath;

namespace VVVV 
{
	namespace Nodes 
	{
		public ref class GetBodyDetailsNode : v4::IPlugin,v4::IPluginConnections
		{
		public:
			GetBodyDetailsNode(void);

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
						Info->Name = "GetBodyDetails";
						Info->Category = "Box2d";
						Info->Version = "";
						Info->Help = "Get details about a created box2d body";
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

			v4::INodeIn^ vInBodies;
			v4::IValueIn^ vInFilterPersisted;
			v4b2d::BodyDataType^ m_bodies;
			v4b2d::GroundDataType^ mGround;
			bool isbody;

			v4::IValueOut^ vOutPosition;
			v4::IValueOut^ vOutRotation;
			v4::IValueOut^ vOutVelocity;
			v4::IValueOut^ vOutIsDynamic;
			v4::IValueOut^ vOutIsSleeping;
			v4::IValueOut^ vOutIsFrozen;
			v4::IValueOut^ vOutMass;
			v4::IValueOut^ vOutInertia;
			v4::IStringOut^ vOutCustom;
			v4::INodeOut^ vOutShapes;
			v4::IStringOut^ vOutShapeType;
			v4::IValueOut^ vOutShapeCount;
			v4::IValueOut^ vOutLifeTime;
			v4::IValueOut^ vOutTTL;
			v4::IValueOut^ vOutHasTTL;

			v4::IValueOut^ vOutId;
			v4b2d::ShapeDataType^ m_shapes;

		};
	}
}
