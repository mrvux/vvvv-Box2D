#pragma once

#include "Box2D.h"
#include <vector>

namespace v4 = VVVV::PluginInterfaces::V1;

namespace VVVV 
{
	namespace DataTypes 
	{
		[System::Runtime::InteropServices::GuidAttribute("AA9E5FA2-50B5-4cb8-AE16-4F160CAB3A69"),
			System::Runtime::InteropServices::InterfaceType(System::Runtime::InteropServices::ComInterfaceType::InterfaceIsIUnknown)]
		public interface class IGroundIO: v4::INodeIOBase
		{
			b2Body* GetGround();
			bool IsValid();
			void SetIsValid(bool value);
			void SetGround(b2Body* world);
		};


		public ref class GroundDataType : IGroundIO 
		{
			private:
				static System::Guid^ m_guid;
				b2Body* m_ground;
				bool m_isvalid;
			public:
				GroundDataType();

				virtual b2Body* GetGround();
				virtual bool IsValid();
				virtual void SetIsValid(bool value);
				virtual void SetGround(b2Body* ground);


				static System::String^ FriendlyName = "Box2d Ground";
				static property System::Guid^ GUID 
				{
					System::Guid^ get() 
					{
						return gcnew System::Guid("AA9E5FA2-50B5-4cb8-AE16-4F160CAB3A69");
					}
				}
		};
	}
}


