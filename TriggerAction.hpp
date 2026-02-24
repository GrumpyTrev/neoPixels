#pragma once
#include <vector>
#include "Action.hpp"

namespace Lights
{
    /// @brief A non-templated base class for all callbacks. Required so that all instances can be held in
    ///        a single collectiomn
    class CallbackBase
    {
    public:
        virtual void Execute() = 0;
    };

    template <class T>
    class Callback : public CallbackBase
    {
    public:

        /// @brief Define TriggerEvent as a pointer to a method in class T that takes no parameters
        typedef void ( T::* TriggerEvent )( );

        /// @brief Create a Callback 
        /// @param trg Instance of the class containing the callback method
        /// @param op The method to cal
        Callback( T* trg, TriggerEvent op ) : target( trg ), operation( op ) {}

        /// @brief Execute the method on the save class instance
        void Execute()
        {
            ( target->*operation )( );
        }

    private:

        /// @brief Instance of the class to be called
        T* target;

        /// @brief Pointer to the method to be called
        TriggerEvent operation;
    };

    /// @brief The TriggerAction holds one or more callback methods that are called when the action is executed
    class TriggerAction : public Action
    {
    public:

        /// @brief Explicit default constructor
        inline TriggerAction() {}

        /// @brief Copy constructor
        /// @param rhs
        inline TriggerAction( const TriggerAction& rhs ) : Action( rhs )
        {
            callbacks = rhs.callbacks;
        };

        /// @brief Clone the item.
        /// @return
        inline TriggerAction* Clone() const { return new TriggerAction( *this ); }

        /// @brief Add an action to the trigger
        /// @param item 
        inline void AddCallback( CallbackBase* callBack ) { callbacks.push_back( callBack ); }

    protected:

        /// @brief Trigger all of the callbacks
        inline void Execute()
        {
            for ( CallbackBase* callback : callbacks )
            {
                callback->Execute();
            }
        };

    private:

        /// @brief The callback methods to call when this action is executed
        vector<CallbackBase*> callbacks;
    };
}