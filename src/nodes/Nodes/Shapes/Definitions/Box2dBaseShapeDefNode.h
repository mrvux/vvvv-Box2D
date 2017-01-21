#pragma once


#include "../../../DataTypes/Shapes/ShapeDefDataType.h"
#include "../../../Internals/Data/ShapeCustomData.h"

#include "../../../Utils/ArrayUtils.h"

namespace v4 = VVVV::PluginInterfaces::V1;
namespace gen = System::Collections::Generic;
namespace v4b2d = VVVV::DataTypes;

namespace VVVV 
{
	namespace Nodes 
	{
		public ref class Box2dBaseShapeDefNode
		{
		public:
			Box2dBaseShapeDefNode(void);

			virtual void SetPluginHost(v4::IPluginHost^ Host);
			virtual void Configurate(v4::IPluginConfig^ Input);

			virtual void Evaluate(int SpreadMax) abstract;
			
			virtual property bool AutoEvaluate 
			{
				bool get() { return false; }
			}
	
		protected:
			v4::IPluginHost^ FHost;
						//Details
			v4::IValueIn^ vInDensity;
			v4::IValueIn^ vInFriction;
			v4::IValueIn^ vInRestitution;
			v4::IValueIn^ vInIsSensor;
			v4::IValueIn^ vInGroupIndex;
			v4::IStringIn^ vInCustom;

			v4::INodeOut^ vOutShapes;

			v4b2d::ShapeDefDataType^ m_shapes;

			virtual void OnPluginHostSet() abstract;

		private:
			

		};
	}
}
