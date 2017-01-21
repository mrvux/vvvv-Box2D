#pragma once

#include "../../DataTypes/Controllers/ControllerDefDataType.h"
#include "Box2dBaseControllerDefNode.h"

#include "../../Utils/ArrayUtils.h"

namespace v4 = VVVV::PluginInterfaces::V1;
namespace gen = System::Collections::Generic;
namespace v4b2d = VVVV::DataTypes;

namespace VVVV 
{
	namespace Nodes 
	{
		public ref class Box2dTensorDampingControllerDefNode : Box2dBaseControllerDefNode, v4::IPlugin,v4::IPluginConnections,public System::IDisposable
		{
		public:
			Box2dTensorDampingControllerDefNode(void);

			static property v4::IPluginInfo^ PluginInfo 
				{
					v4::IPluginInfo^ get() 
					{
						//v4::IPluginInfo^ Info;
						v4::IPluginInfo^ Info = gcnew VVVV::PluginInterfaces::V1::PluginInfo();
						Info->Name = "TensorDamping";
						Info->Category = "Box2d";
						Info->Version = "Controller";
						Info->Help = "Box2d Constant Force controller";
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

		protected:
			virtual void OnPluginHostSet() override;
			virtual void OnEvaluate(int SpreadMax, bool reset) override;

		private:
			v4::IValueIn^ vInModel;
		};
	}

}
