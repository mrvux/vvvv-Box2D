#pragma once

#include "Box2D.h"
#include <vector>

namespace v4 = VVVV::PluginInterfaces::V1;

#include "../Internals/Contact/ContactListener.h"


namespace VVVV 
{
	namespace DataTypes 
	{
		[System::Runtime::InteropServices::GuidAttribute("A1FFB623-3077-4435-A482-D87BC60A4443"),
		System::Runtime::InteropServices::InterfaceType(System::Runtime::InteropServices::ComInterfaceType::InterfaceIsIUnknown)]
		public interface class IBoxWorldIO: v4::INodeIOBase
		{
			bool GetIsValid();
			void SetIsValid(bool value);
			b2World* GetWorld();
			void SetWorld(b2World* world);
			bool GetIsEnabled();
			void SetIsEnabled(bool value);
			bool HasReset();
			void SetReset(bool value);
		};

		public ref class WorldDataType : IBoxWorldIO
		{
			private:
				static System::Guid^ FGuid;
				bool enabled;
				bool isvalid;
				bool hasreset;
				b2World* mWorld;
				bool worldchanged;
				int bodycounter;
				int shapecounter;
				int jointcounter;

			public:
				WorldDataType(void);
				
				virtual bool GetIsValid();
				virtual void SetIsValid(bool value);
				virtual bool GetIsEnabled();
				virtual void SetIsEnabled(bool value);
				virtual b2World* GetWorld();		
				virtual void SetWorld(b2World* world);
				virtual bool HasReset() { return this->hasreset; }
				virtual void SetReset(bool value) { this->hasreset = value; }

				int GetNewBodyId();
				int GetNewShapeId();
				int GetNewJointId();

				bool Reset;
				std::vector<ContactReportId*>* Reports;
				std::vector<ContactResultData*>* Results;
				std::vector<int>* DeletedIds;

				static System::String^ FriendlyName = "Box2d World";
				static property System::Guid^ GUID 
				{
					System::Guid^ get() 
					{
						if (FGuid == System::Guid::Empty) 
						{
							FGuid = gcnew System::Guid("A1FFB623-3077-4435-A482-D87BC60A4443");
						}
						return FGuid;
					}
				}



		};
	
	}
}



