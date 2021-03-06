//!
//! @file 			Callback.hpp
//! @author 		Geoffrey Hunter <gbmhunter@gmail.com> (www.mbedded.ninja)
//! @created		2014-01-14
//! @last-modified 	2014-09-14
//! @brief			Contains the CallbackBase, Callback and CallbackGen classes.
//! @details
//!					See README.rst in root dir for more info.

#ifndef __cplusplus
	#error Please build with C++ compiler
#endif

//===============================================================================================//
//======================================== HEADER GUARD =========================================//
//===============================================================================================//

#ifndef MCALLBACK_CALLBACK_H
#define MCALLBACK_CALLBACK_H

//===============================================================================================//
//========================================== INCLUDES ===========================================//
//===============================================================================================//

// System headers
#include <stdint.h>
#include <iostream>
#include <string.h>

// User headers

//===============================================================================================//
//======================================== NAMESPACE ============================================//
//===============================================================================================//

namespace MCallbacks
{

	//! @brief		This is a base class for the Caller class.
	//! @details	Always requires the return type, and accepts 0 or 1 function arguments whose type needs to be specified.
	template<class returnType, class fArg1Type>
	class CallbackBase
	{	
		
	public:

		typedef returnType (CallbackBase::*funcT)(fArg1Type);

		//===============================================================================================//
		//======================================= PUBLIC VARIABLES ======================================//
		//===============================================================================================//


		CallbackBase *obj;
		funcT func;
			
		//===============================================================================================//
		//========================================= PUBLIC METHODS ======================================//
		//===============================================================================================//

		returnType Execute(fArg1Type fArg1)
		{
			std::cout << "Error: Should never get here!";
			// To keep the compiler happy
			return returnType();
		}

	protected:


		
	};

	//! @brief		This is used to create objects that can be assigned to a callback function and then executed.
	//! @details	Uses the same template arguments as CallerBase.
	template<class returnType, class fArg1Type>
	class Callback : public virtual CallbackBase<returnType, fArg1Type>
	{

	public:

		//! @brief	Constructor
		Callback()
		{
			this->obj = 0;
		}

		//! @brief		Assignment operator overload
		//! @details	Called when converting from CallbackGen to Callback object types
		Callback& operator=(const Callback<returnType, fArg1Type> &callback)
		{
			// Check if the right-hand side Callback object has been initialised
			if(&callback != NULL)
			{
				this->obj = callback.obj;
				this->func = callback.func;

			}
			else
				Callback();
			return *this;
		}

		//! @brief		Use this to determine if the callback is valid, i.e. it has a callback method assigned.
		//! @details	Execute() also checks this before calling the callback to prevent memory errors.
		bool IsValid()
		{
			if(this->obj == NULL)
				return false;
			else
				return true;
		}

		returnType Execute(fArg1Type fArg1)
		{
			if(this->obj == NULL)
				return (returnType)NULL;

			// Callback is NOT NULL
			return (this->obj->*this->func)(fArg1);

		}

	protected:

	};

	//! @brief		This can generate callbacks to member functions!
	//! @details	Note that this has one more type than the callback classes, that is, a type for the object that the callback function belongs to.
	template<class objType, class returnType, class fArg1 = void, class fArg2 = void>
	class CallbackGen : public Callback<returnType, fArg1>
	{
	public:

		// Create method pointer type (points to method of a particular class
		typedef returnType (objType::*funcT)(fArg1);


		//! @brief		Constructor
		CallbackGen(objType* obj, funcT func)
		{
			// This memcpy() trick is used to assign (copy) objects of different types.
			// This trick is key to how the callbacks are type independent
			memcpy(&this->func, &func, sizeof(func));
			memcpy(&this->obj, &obj, sizeof(obj));
		}
	protected:
		CallbackGen();
	};

} // namespace MCallbacks

#endif	// #ifndef MCALLBACK_CALLBACK_H

// EOF
