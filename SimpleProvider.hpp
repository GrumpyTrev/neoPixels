#pragma once
#include "BaseDefinedObject.hpp"

namespace Lights
{
	class ProviderCycleInterface : public BaseDefinedObject
	{
	public:
		virtual void Next() = 0;
	};

	template <class T>
	class ProviderInterface : public ProviderCycleInterface
	{
	public:
		virtual T Value() = 0;
		virtual T GetValue() = 0;
		virtual void SetValue(T value) = 0;
		virtual void AddValue(T value) = 0;
	};

	template <class T>
	class BaseProvider : public ProviderInterface<T>
	{
	public:
		inline virtual T Value() { return providerImplementation->Value(); }
		inline virtual T GetValue() { return providerImplementation->GetValue(); }
		inline virtual void SetValue(T value) { providerImplementation->SetValue(value); }
		inline virtual void Next()
		{
			providerImplementation->Next();
		}
		inline virtual void AddValue(T value) { providerImplementation->AddValue(value); }

	protected:
		ProviderInterface<T> *providerImplementation = nullptr;
	};

	template <class T>
	class SimpleProvider : public ProviderInterface<T>
	{
	public:
		inline SimpleProvider(T value) : providedValue(value) {}
		virtual inline T Value() { return providedValue; }
		virtual inline T GetValue() { return providedValue; }
		virtual inline void SetValue(T value) { providedValue = value; }
		virtual inline void Next() {};
		virtual inline void AddValue(T value) {};

	protected:
		T providedValue = 0;
	};
}