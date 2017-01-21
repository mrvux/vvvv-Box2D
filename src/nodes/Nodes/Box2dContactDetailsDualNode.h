#pragma once
#include "../DataTypes/WorldDataType.h"
#include "../DataTypes/Shapes/ShapeDataType.h"
#include "../DataTypes/BodyDataType.h"

#include "../Utils/ArrayUtils.h"


namespace v4 = VVVV::PluginInterfaces::V1;
namespace gen = System::Collections::Generic;
namespace v4b2d = VVVV::DataTypes;

namespace VVVV 
{
	namespace Nodes 
	{
		public ref class Box2dContactDetailsDualNode : v4::IPlugin,v4::IPluginConnections
		{
		public:
			Box2dContactDetailsDualNode(void);
			
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
						Info->Name = "GetContactDetails";
						Info->Category = "Box2d";
						Info->Version = "Spreaded";
						Info->Help = "Get details about a collision, returning groups of 2 per shape";
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
			v4::IValueOut^ vOutNormals;
			v4::IValueOut^ vOutNormalImpulse;
			v4::IValueOut^ vOutTangentImpulse;
			v4::IValueOut^ vOutBodyId;
			v4::IValueOut^ vOutShapes;

			v4::IValueOut^ vOutNew;

			v4::INodeOut^ vOutShape;
			v4b2d::ShapeDataType^ m_shape;

			v4b2d::BodyDataType^ mBody;
			v4::INodeOut^ vOutBody;

			v4::IValueOut^ vOutContactCount;

		};
	}
}