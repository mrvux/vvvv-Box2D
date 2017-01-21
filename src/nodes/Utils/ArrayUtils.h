#pragma once

namespace VVVV 
{
	namespace Utils 
	{
		public ref class ArrayUtils
		{
			public:

				static array<Guid> ^ SingleGuidArray(Guid^ uid) 
				{
					array<Guid> ^ arr1d = gcnew array<Guid>(1);
					arr1d->SetValue(uid,0);

					return arr1d;
				}

				static array<Guid> ^ DoubleGuidArray(Guid^ uid1,Guid^ uid2) 
				{
					array<Guid> ^ arr1d = gcnew array<Guid>(2);
					arr1d->SetValue(uid1,0);
					arr1d->SetValue(uid2,1);

					return arr1d;
				}

		};
	}
}
