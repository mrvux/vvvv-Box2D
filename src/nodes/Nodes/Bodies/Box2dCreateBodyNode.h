#pragma once
#include "../../DataTypes/WorldDataType.h"
#include "../../DataTypes/Shapes/ShapeDefDataType.h"
#include "../../DataTypes/BodyDataType.h"

#include "../../Utils/ArrayUtils.h"

namespace v4 = VVVV::PluginInterfaces::V1;
namespace gen = System::Collections::Generic;
namespace v4b2d = VVVV::DataTypes;

namespace VVVV 
{
	namespace Nodes 
	{
		public ref class Box2dCreateBodyNode : v4::IPlugin,v4::IPluginConnections
		{
		public:
			Box2dCreateBodyNode(void);

			static property v4::IPluginInfo^ PluginInfo 
				{
					v4::IPluginInfo^ get() 
					{
						//v4::IPluginInfo^ Info;
						v4::IPluginInfo^ Info = gcnew VVVV::PluginInterfaces::V1::PluginInfo();
						Info->Name = "CreateBody";
						Info->Category = "Box2d";
						Info->Version = "";
						Info->Help = "Creates box2d body";
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

			v4::INodeIn^ vInWorld;
			v4::INodeIn^ vInShapes;

			v4::IValueIn^ vInPosition;
			v4::IValueIn^ vInAngle;
			v4::IValueIn^ vInVelocity;
			v4::IValueIn^ vInAngularVelocity;
			v4::IValueIn^ vInLinearDamping;
			v4::IValueIn^ vInAngularDamping;
			v4::IValueIn^ vInIsBullet;
			v4::IValueIn^ vInFixedRotation;
			v4::IStringIn^ vInCustom;
			v4::IValueIn^ vInHasTTL;
			v4::IValueIn^ vInTTL;
			v4::IValueIn^ vInDoCreate;

			v4b2d::WorldDataType^ mWorld;
			v4b2d::ShapeDefDataType^ mShapes;

			v4b2d::BodyDataType^ mBodies;

			v4::IValueOut^ vOutCanCreate;
			v4::INodeOut^ vOutBodies;
			v4::IValueOut^ vOutId;

		};
	}
}
