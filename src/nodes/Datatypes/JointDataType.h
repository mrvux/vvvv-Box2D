#pragma once

#include "Box2D.h"
#include <vector>

namespace v4 = VVVV::PluginInterfaces::V1;

namespace VVVV 
{
	namespace DataTypes
	{
		[System::Runtime::InteropServices::GuidAttribute("D2E73B5C-22DA-449f-A6FB-4117A5761307"),
			System::Runtime::InteropServices::InterfaceType(System::Runtime::InteropServices::ComInterfaceType::InterfaceIsIUnknown)]
		public interface class IJointIO: v4::INodeIOBase
		{
			b2Joint* GetSlice(int index);
		};


		public ref class JointDataType : IJointIO 
		{
			private:
				static System::Guid^ m_guid;
				std::vector<b2Joint*>* m_joints;
			public:
				JointDataType();

				virtual b2Joint* GetSlice(int index);
				void Reset();
				void Add(b2Joint* body);
				int Size();

				static System::String^ FriendlyName = "Box2d Joint";
				static property System::Guid^ GUID 
				{
					System::Guid^ get() 
					{
						return gcnew System::Guid("D2E73B5C-22DA-449f-A6FB-4117A5761307");
					}
				}
		};
	}
}