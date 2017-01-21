#pragma once

namespace VVVV 
{
	namespace Utils 
	{
		public ref class ArrayUtils
		{
			public:

				static array<System::Guid>^ SingleGuidArray(System::Guid^ uid)
				{
					array<System::Guid>^ arr1d = gcnew array<System::Guid>(1);
					arr1d->SetValue(uid,0);

					return arr1d;
				}

				static array<System::Guid> ^ DoubleGuidArray(System::Guid^ uid1, System::Guid^ uid2)
				{
					array<System::Guid> ^ arr1d = gcnew array<System::Guid>(2);
					arr1d->SetValue(uid1,0);
					arr1d->SetValue(uid2,1);

					return arr1d;
				}

		};
	}
}
