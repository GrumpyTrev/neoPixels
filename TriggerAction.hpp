#pragma once
#include <vector>
#include "Action.hpp"

namespace Lights
{
    class CallbackBase
    {
    public:
        virtual void Execute() = 0;
    };

    template <class T>
    class Callback : public CallbackBase
    {
    public:

        typedef void ( T::* TriggerEvent )( );

        Callback( T* trg, TriggerEvent op ) : target( trg ), operation( op ) {}

        void Execute()
        {
            ( target->*operation )( );
        }

    private:

        T* target;
        TriggerEvent operation;
    };

    class TriggerAction : public Action
    {
    public:

        /// @brief Explicit default constructor
        inline TriggerAction() {}

        /// @brief Copy constructor
        /// @param rhs
        inline TriggerAction( const TriggerAction& rhs ) : Action( rhs )
        {
            providers = rhs.providers;
        };

        /// @brief Clone the item.
        /// @return
        inline TriggerAction* Clone() const { return new TriggerAction( *this ); }

        /// @brief Add an action to the trigger
        /// @param item 
        inline void AddCallback( CallbackBase* callBack ) { providers.push_back( callBack ); }

    protected:

        /// @brief Trigger all of the actions
        inline void Execute()
        {
            for ( auto& provider : providers )
            {
                provider->Execute();
            }
        };

    private:

        std::vector<CallbackBase*> providers;
    };
}