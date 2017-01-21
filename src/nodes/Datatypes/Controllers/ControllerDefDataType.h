#pragma once

#include "Box2D.h"
#include <vector>

namespace v4 = VVVV::PluginInterfaces::V1;

namespace VVVV 
{
	namespace DataTypes 
	{
		[System::Runtime::InteropServices::GuidAttribute("6A531781-24FC-4948-AA97-5DDDFE5A3125"),
			System::Runtime::InteropServices::InterfaceType(System::Runtime::InteropServices::ComInterfaceType::InterfaceIsIUnknown)]
		public interface class IControllerDefIO: v4::INodeIOBase
		{
			b2ControllerDef* GetController();
		};

		public ref class ControllerDefDataType : IControllerDefIO
		{
			private:
				static System::Guid^ FGuid;
				b2ControllerDef* m_controller;

			public:
				ControllerDefDataType(void);
				
				static System::String^ FriendlyName = "Box2d Controller Definition";
				static property System::Guid^ GUID 
				{
					System::Guid^ get() 
					{
						if (FGuid == System::Guid::Empty) 
						{
							FGuid = gcnew System::Guid("6A531781-24FC-4948-AA97-5DDDFE5A3125");
						}
						return FGuid;
					}
				}

				virtual b2ControllerDef* GetController()
				{
					return this->m_controller;
				}

				virtual void SetController(b2ControllerDef* ctrl)
				{
					this->m_controller = ctrl;
				}
		};
	
	}
}



