#pragma once

#include "../../../DataTypes/BodyDataType.h"
#include "Box2dCreateJointNode.h"

#include "../../../Utils/ArrayUtils.h"

namespace v4 = VVVV::PluginInterfaces::V1;
namespace gen = System::Collections::Generic;
namespace v4b2d = VVVV::DataTypes;

namespace VVVV 
{
	namespace Nodes 
	{
		public ref class Box2dCreateLineJointNode : Box2dCreateJointNode, v4::IPlugin, v4::IPluginConnections
		{
		public:
			static property v4::IPluginInfo^ PluginInfo 
				{
					v4::IPluginInfo^ get() 
					{
						//v4::IPluginInfo^ Info;
						v4::IPluginInfo^ Info = gcnew VVVV::PluginInterfaces::V1::PluginInfo();
						Info->Name = "CreateLineJoint";
						Info->Category = "Box2d";
						Info->Version = "";
						Info->Help = "Creates a line joint between 2 bodies";
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





			Box2dCreateLineJointNode(void);
			virtual void Evaluate(int SpreadMax) override;
			virtual bool ForceBodyOneGround() override { return false; };
		protected:
			virtual void OnPluginHostSet() override;
		private:
			v4::IValueIn^ vInPosition;

			v4::IValueIn^ vInLocalAxis;

			v4::IValueIn^ vInEnableLimit;

			v4::IValueIn^ vInLowTranslation;
			v4::IValueIn^ vInUpTranslation;

			v4::IValueIn^ vInMaxMotorForce;
			v4::IValueIn^ vInMotorSpeed;
			v4::IValueIn^ vInEnableMotor;
		};
	}
}
