#pragma once
#include "../DataTypes/WorldDataType.h"
#include "../DataTypes/BodyDataType.h"
#include "../DataTypes/JointDataType.h"
#include "../DataTypes/GroundDataType.h"
#include "../Internals/Contact/ContactListener.h"
#include "../Internals/Engine/ObjectManager.h"

#include "../Internals/Data/BodyCustomData.h"
#include "../Internals/Data/ShapeCustomData.h"
#include "../Internals/Data/JointCustomData.h"
#include "../Internals/Data/ContactResultData.h"

#include "../Utils/ArrayUtils.h"

#include "Box2D.h"
#include <vector>

namespace v4 = VVVV::PluginInterfaces::V1;
namespace gen = System::Collections::Generic;
namespace v4b2d = VVVV::DataTypes;

namespace VVVV 
{
	namespace Nodes 
	{
		public ref class Box2dWorldNode : v4::IPlugin, public System::IDisposable
		{
			private:
				v4::IPluginHost^ FHost;

				v4::IValueIn^ vInLowerBound;
				v4::IValueIn^ vInUpperBound;
				v4::IValueIn^ vInGravity;
				v4::IValueIn^ vInAllowSleep;
				v4::IValueIn^ vInEnabled;
				v4::IValueFastIn^ vInTimeStep;
				v4::IValueFastIn^ vInPosIterations;
				v4::IValueFastIn^ vInVelIterations;
				v4::IValueIn^ vInReset;

				v4::IValueOut^ vOutWorldValid;
				v4::IValueOut^ vOutControllerCount;
				v4::INodeOut^ vOutWorldNode;
				v4::INodeOut^ vOutBodies;
				v4::INodeOut^ vOutGround;
				v4::INodeOut^ vOutJoints;
				v4::IValueOut^ vOutReset;

				v4b2d::WorldDataType^ mWorld;
				v4b2d::BodyDataType^ mBodies;
				v4b2d::GroundDataType^ mGround;
				v4b2d::JointDataType^ mJoints;

				b2World* internalworld;

				bool ctrlconnected;
				
				std::vector<ContactReportId*>* reports;
				std::vector<ContactResultData*>* results;
				std::vector<b2Controller*>* controller;
				ContactListener* MyListener;
			
			public:
				static property v4::IPluginInfo^ PluginInfo
				{
					v4::IPluginInfo^ get()
					{
						//v4::IPluginInfo^ Info;
						v4::IPluginInfo^ Info = gcnew VVVV::PluginInterfaces::V1::PluginInfo();
						Info->Name = "World";
						Info->Category = "Box2d";
						Info->Version = "";
						Info->Help = "Box2d World definition";
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




				Box2dWorldNode(void);
				 ~Box2dWorldNode();

				virtual void SetPluginHost(v4::IPluginHost^ Host);
				virtual void Configurate(v4::IPluginConfig^ Input);
				virtual void Evaluate(int SpreadMax);
				
				virtual property bool AutoEvaluate 
				{
					bool get() { return false; }
				}



		};
	}
}

