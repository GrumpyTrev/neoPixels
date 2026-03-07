#pragma once
#include "BaseDefinedObject.hpp"
#include <vector>

namespace Lights
{
	class ProviderBase : public BaseDefinedObject
	{
	public:
		virtual inline void Next() {}
		virtual inline void Reset() { initialised = false; }
		virtual inline void Initialise() {}
		inline void SelfIncrement( bool isSelf ) { selfIncrement = isSelf; }

	protected:
		/// @brief Should this provider automatically provide a new value everytime it is called
		bool selfIncrement = true;

		/// @brief Has this provider been initialised. It will need to be initialised before providing a value
		bool initialised = false;
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
		inline void Reset() { index = 0; }

	private:
		vector<T> sequence;
		uint16_t index = 0;
	};
}