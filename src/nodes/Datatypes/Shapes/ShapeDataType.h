#pragma once

#include "Box2D.h"
#include <vector>

namespace v4 = VVVV::PluginInterfaces::V1;

namespace VVVV 
{
	namespace DataTypes 
	{
		[System::Runtime::InteropServices::GuidAttribute("D0FA06A8-E796-46b2-AB21-497009F871BD"),
			System::Runtime::InteropServices::InterfaceType(System::Runtime::InteropServices::ComInterfaceType::InterfaceIsIUnknown)]
		public interface class IShapeIO: v4::INodeIOBase
		{
			b2Shape* GetSlice(int index);
		};


		public ref class ShapeDataType : IShapeIO 
		{
			private:
				static System::Guid^ m_guid;
				std::vector<b2Shape*>* m_shapes;
			public:
				ShapeDataType();

				virtual b2Shape* GetSlice(int index);
				void Reset();
				void Add(b2Shape* shape);
				int Count();


				static System::String^ FriendlyName = "Box2d Shape";
				static property System::Guid^ GUID 
				{
					System::Guid^ get() 
					{
						return gcnew System::Guid("D0FA06A8-E796-46b2-AB21-497009F871BD");
					}
				}
		};
	}
}
