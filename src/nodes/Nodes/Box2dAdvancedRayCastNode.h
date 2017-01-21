#pragma once

#include "../DataTypes/WorldDataType.h"
#include "../DataTypes/Shapes/ShapeDataType.h"
#include "../DataTypes/BodyDataType.h"

#include "../Internals/Data/ShapeCustomData.h"
#include "../Internals/Data/BodyCustomData.h"

using namespace VVVV::DataTypes;

namespace VVVV 
{
	namespace Nodes 
	{
		public ref class Box2dAdvancedRayCastNode : IPlugin,IPluginConnections
		{
		public:
			Box2dAdvancedRayCastNode(void);

			virtual void SetPluginHost(IPluginHost^ Host);
			virtual void Configurate(IPluginConfig^ Input);
			virtual void Evaluate(int SpreadMax);
			virtual void ConnectPin(IPluginIO^ Pin);
			virtual void DisconnectPin(IPluginIO^ Pin);

			virtual property bool AutoEvaluate { bool get() { return false; } }

			static property IPluginInfo^ PluginInfo 
			{
				IPluginInfo^ get() 
				{
					//IPluginInfo^ Info;
					IPluginInfo^ Info = gcnew VVVV::PluginInterfaces::V1::PluginInfo();
					Info->Name = "RayCast";
					Info->Category = "Box2d";
					Info->Version = "Advanced";
					Info->Help = "";
					Info->Bugs = "";
					Info->Credits = "";
					Info->Warnings = "";
					Info->Author = "vux";
					Info->Tags="";

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
			IPluginHost^ FHost;

			INodeIn^ vInWorld;
			WorldDataType^ m_world;

			IValueIn^ vInStartPos;
			IValueIn^ vInEndPos;
			IValueIn^ vInMaxCount;
			IValueIn^ vInDoQuery;


			IValueOut^ vOutShapeCount;
			IValueOut^ vOutSegmentsCount;

			IValueOut^ vOutSegments;

			INodeOut^ vOutShapes;
			
			ShapeDataType^ mShapes;
			IValueOut^ vOutShapeId;
			
			INodeOut^ vOutBodies;
			BodyDataType^ mBodies;
			IValueOut^ vOutBodyId;
		};
	}
}



