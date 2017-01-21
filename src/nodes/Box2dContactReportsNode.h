#pragma once
#include "DataTypes/WorldDataType.h"
#include "DataTypes/Shapes/ShapeDataType.h"
#include "DataTypes/BodyDataType.h"
#include "Utils/ArrayUtils.h"


#include "Box2D.h"
#include <vector>

namespace v4 = VVVV::PluginInterfaces::V1;
namespace v4b2d = VVVV::DataTypes;

namespace VVVV 
{
	namespace Nodes 
	{
		public ref class Box2dContactReportsNode : v4::IPlugin, v4::IPluginConnections
		{
		public:
			Box2dContactReportsNode(void);
			
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
						Info->Name = "GetContactReports";
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

			v4::INodeIn^ vInWorld;
			v4b2d::WorldDataType^ m_world;

			v4::IValueOut^ vOutId;
			v4::IValueOut^ vOutPosition;
			v4::IValueOut^ vOutBodyId1;
			v4::IValueOut^ vOutShapes1;
			v4::IValueOut^ vOutBodyId2;
			v4::IValueOut^ vOutShapes2;


			v4::IValueOut^ vOutNew;

			v4::INodeOut^ vOutShape1;
			v4b2d::ShapeDataType^ m_shape1;
			v4::INodeOut^ vOutShape2;
			v4b2d::ShapeDataType^ m_shape2;

			v4b2d::BodyDataType^ mBody1;
			v4b2d::BodyDataType^ mBody2;
			v4::INodeOut^ vOutBody1;
			v4::INodeOut^ vOutBody2;

			v4::IValueOut^ vOutContactCount;

		};
	}
}