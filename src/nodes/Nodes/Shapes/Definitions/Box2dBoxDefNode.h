#pragma once

#include "Box2dBaseShapeDefNode.h"
#include "../../../Utils/ArrayUtils.h"

namespace v4 = VVVV::PluginInterfaces::V1;
namespace gen = System::Collections::Generic;
namespace v4b2d = VVVV::DataTypes;

namespace VVVV 
{
	namespace Nodes 
	{
		public ref class Box2dBoxDefNode : Box2dBaseShapeDefNode, v4::IPlugin,public System::IDisposable
		{
		public:
			Box2dBoxDefNode(void);
			~Box2dBoxDefNode(void);

			static property v4::IPluginInfo^ PluginInfo 
				{
					v4::IPluginInfo^ get() 
					{
						//v4::IPluginInfo^ Info;
						v4::IPluginInfo^ Info = gcnew VVVV::PluginInterfaces::V1::PluginInfo();
						Info->Name = "Box";
						Info->Category = "Box2d";
						Info->Version = "";
						Info->Help = "Box2d Box Shape definition";
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





			virtual void Evaluate(int SpreadMax) override;
		
		protected:
			virtual void OnPluginHostSet() override;

		private:
			v4::IValueIn^ vInLocalPosition;
			v4::IValueIn^ vInAngle;
			v4::IValueIn^ vInSize;
		};
	}
}
