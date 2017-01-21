#pragma once
#include "../DataTypes/WorldDataType.h"
#include "../DataTypes/Shapes/ShapeDataType.h"
#include "../DataTypes/BodyDataType.h"

using namespace VVVV::DataTypes;

namespace VVVV 
{
	namespace Nodes 
	{
		public ref class Box2dHasContactNode: IPlugin,IPluginConnections
		{
		public:
			Box2dHasContactNode(void);

			static property IPluginInfo^ PluginInfo 
				{
					IPluginInfo^ get() 
					{
						//IPluginInfo^ Info;
						IPluginInfo^ Info = gcnew VVVV::PluginInterfaces::V1::PluginInfo();
						Info->Name = "HasContact";
						Info->Category = "Box2d";
						Info->Version = "";
						Info->Help = "Box2d search if two shape Ids are in contact";
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
	

			virtual void SetPluginHost(IPluginHost^ Host);
			virtual void Configurate(IPluginConfig^ Input);
			virtual void Evaluate(int SpreadMax);
			virtual void ConnectPin(IPluginIO^ Pin);
			virtual void DisconnectPin(IPluginIO^ Pin);
			
			virtual property bool AutoEvaluate 
			{
				bool get() { return false; }
			}
		protected:

		private:
			IPluginHost^ FHost;

			INodeIn^ vInWorld;
			WorldDataType^ m_world;

			IValueIn^ vInId1;
			IValueIn^ vInMode1;
			IValueIn^ vInId2;
			IValueIn^ vInMode2;

			IValueOut^ vOutContact;

		};
	}
}


