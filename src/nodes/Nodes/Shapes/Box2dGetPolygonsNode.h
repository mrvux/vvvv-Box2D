#pragma once
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
		public ref class Box2dGetPolygonsNode: v4::IPlugin,v4::IPluginConnections
		{
		public:
			Box2dGetPolygonsNode(void);

			static property v4::IPluginInfo^ PluginInfo 
				{
					v4::IPluginInfo^ get() 
					{
						//v4::IPluginInfo^ Info;
						v4::IPluginInfo^ Info = gcnew VVVV::PluginInterfaces::V1::PluginInfo();
						Info->Name = "GetPolygons";
						Info->Category = "Box2d";
						Info->Version = "";
						Info->Help = "Box2d Polygon Shape";
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
				bool get() { return false; }
			}
		protected:

		private:
			v4::IPluginHost^ FHost;

			v4::INodeIn^ vInShapes;
			v4::IValueIn^ vInClosed;
			v4::IValueIn^ vInLocal;
			v4b2d::ShapeDataType^ m_polygons;

			v4::IValueOut^ vOutCenters;
			v4::IValueOut^ vOutVertices;
			v4::IValueOut^ vOutVerticesCount;
			v4::IValueOut^ vOutIsSensor;
			v4::IValueOut^ vOutId;
			v4::IValueOut^ vOutBodyId;
			v4::IStringOut^ vOutCustom;
			v4::IValueOut^ vOutLifeTime;

			gen::List<v4math::Vector2D>^ tempVertices;
			gen::List<v4math::Vector2D>^ tempCenters;

			gen::List<System::String^>^ tempCustomList;
			gen::List<int>^ tempCustomId;
			gen::List<double>^ tempLifeTime;;

			bool m_closed;
			bool m_local;
		};
	}
}

