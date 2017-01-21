#pragma once

#include "Box2D.h"
#include <vector>

namespace v4 = VVVV::PluginInterfaces::V1;
namespace gen = System::Collections::Generic;

namespace VVVV 
{
	namespace DataTypes 
	{
		[System::Runtime::InteropServices::GuidAttribute("2104667E-DEF9-402c-BC1A-CF94616B62FC"),
			System::Runtime::InteropServices::InterfaceType(System::Runtime::InteropServices::ComInterfaceType::InterfaceIsIUnknown)]
		public interface class IShapeDefIO: v4::INodeIOBase
		{
			b2ShapeDef* GetSlice(int index);
			System::String^ GetCustom(int index);
		};


		public ref class ShapeDefDataType : IShapeDefIO , public System::IDisposable
		{
			private:
				std::vector<b2ShapeDef*>* m_shapes;
				gen::List<System::String^>^ m_custom;
			public:

				ShapeDefDataType();
				~ShapeDefDataType();
				virtual b2ShapeDef* GetSlice(int index);
				virtual System::String^ GetCustom(int index);
				void Reset();

				b2CircleDef* AddCircle();
				b2PolygonDef* AddPolygon();
				b2EdgeChainDef* AddEdgeChain();
				void AddCustom(System::String^ cust);

				static System::String^ FriendlyName = "Box2d Shape Definition";
				static property System::Guid^ GUID 
				{
					System::Guid^ get() 
					{
						return gcnew System::Guid("2104667E-DEF9-402c-BC1A-CF94616B62FC");
					}
				}
		};
	}
}
