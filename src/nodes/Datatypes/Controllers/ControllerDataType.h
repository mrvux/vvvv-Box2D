#pragma once

#include "Box2D.h"
#include <vector>

namespace v4 = VVVV::PluginInterfaces::V1;

namespace VVVV 
{
	namespace DataTypes 
	{
		[System::Runtime::InteropServices::GuidAttribute("2A312EA6-5767-4573-ADB7-1A98968161FE"),
			System::Runtime::InteropServices::InterfaceType(System::Runtime::InteropServices::ComInterfaceType::InterfaceIsIUnknown)]
		public interface class IControllerIO : v4::INodeIOBase
		{
			//b2Controller* GetController();
			b2Controller* GetSlice(int index);
		};



		public ref class ControllerDataType : IControllerIO 
		{
			private:
				static System::Guid^ m_guid;
				std::vector<b2Controller*>* m_controllers;
			public:
				ControllerDataType();


				virtual b2Controller* GetSlice(int index);
				void Reset();
				void Add(b2Controller* ctrl);
				int Size();


				static System::String^ FriendlyName = "Box2d Controller";
				static property System::Guid^ GUID
				{
					System::Guid^ get()
					{
						return gcnew System::Guid("2A312EA6-5767-4573-ADB7-1A98968161FE");
					}
				}
		};

	}
}