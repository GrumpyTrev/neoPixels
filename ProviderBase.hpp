#pragma once
#include "BaseDefinedObject.hpp"
#include <vector>

namespace Lights
{
	class ProviderBase : public BaseDefinedObject
	{
	public:
		virtual void Next() {}
		virtual void Reset() {}
		inline void SelfIncrement( bool isSelf ) { selfIncrement = isSelf; }

	protected:
		bool selfIncrement = true;
	};

	template <class T>
	class ProviderSequenceHelper
	{
	public:
		inline void Add( T item ) { sequence.push_back( item ); }
		inline T Next()
		{
			T retVal = sequence[ index ];

			if ( ++index == sequence.size() )
			{
				index = 0;
			}

			return retVal;
		}
	private:
		std::vector<T> sequence;
		uint16_t index = 0;
	};
}