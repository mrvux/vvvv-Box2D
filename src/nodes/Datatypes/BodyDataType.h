#pragma once

#include "WorldDataType.h"

#include "Box2D.h"
#include <vector>

namespace v4 = VVVV::PluginInterfaces::V1;

namespace VVVV 
{
	namespace DataTypes 
	{
		[System::Runtime::InteropServices::GuidAttribute("69EEFDBE-50BD-42b9-A77F-111D6D8F375B"),
			System::Runtime::InteropServices::InterfaceType(System::Runtime::InteropServices::ComInterfaceType::InterfaceIsIUnknown)]
		public interface class IBodyIO: v4::INodeIOBase
		{
			b2Body* GetSlice(int index);
		};


		public ref class BodyDataType : IBodyIO 
		{
			private:
				static System::Guid^ m_guid;
				std::vector<b2Body*>* m_bodies;
			public:
				BodyDataType();

				virtual b2Body* GetSlice(int index);
				void Reset();
				void Add(b2Body* body);
				int Size();

				static System::String^ FriendlyName = "Box2d Body";
				static property System::Guid^ GUID 
				{
					System::Guid^ get() 
					{
						return gcnew System::Guid("69EEFDBE-50BD-42b9-A77F-111D6D8F375B");
					}
				}
		};
	}
}

