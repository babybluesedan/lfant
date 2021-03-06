/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
#pragma once
#include <lfant/stdafx.h>

#define BOOST_BIND_NO_PLACEHOLDERS

// Internal
#include <lfant/TypeInfo.h>
#include <lfant/Properties.h>
#include <lfant/ptr.h>

// External
#include <boost/algorithm/string.hpp>
#include <boost/bind.hpp>
#include <boost/bind/placeholders.hpp>
#include <boost/function.hpp>
#include <boost/signals2.hpp>
#include <tuple>
#include <typeinfo>
#include <forward_list>
#include <iostream>

#include <sqrat/sqratFunction.h>

#define SENDER(obj, sig) obj, #sig
#define RECEIVER(obj, slot) obj, &lfant::remove_ref<decltype(*obj)>::type::slot


namespace lfant
{

class Game;

/** @addtogroup Game
 *	@{
 */
/** @addtogroup Core
 *	@{
 */

/**
 *	The base class for any object-oriented classes.
 *	Implements a signals and slots system similar to Qt's. Also contains
 *	a few basic game-loop functions to allow homogeneous pointers to an Object
 *	instance, like in ActionScript.
 *	@todo
 *		Possibly make the connection naming more efficient.
 */
class Object
{
	class EventBase
	{
	public:
		EventBase()
		{
		}
		virtual ~EventBase()
		{
		}

		virtual void Clear() {}
	};

/*	class Event0 : public Event
	{
	public:
		typedef boost::function<void()> funcType;
		deque<funcType> func;

		Event0(string name, Object* sender) :
			Event(name, sender)
		{
		}
	};*/

	template<typename ... P>
	class Event : public EventBase
	{
	public:
		typedef boost::signals2::signal<void(P...)> sigType;
		sigType sig;

		virtual ~Event()
		{
		}

		boost::signals2::connection Register(typename sigType::slot_type func)
		{
			return sig.connect(func);
		}

		void Clear()
		{
		}

		void Trigger(P... args)
		{
			sig(args...);
		}
	};

	template<typename T>
	class EventVar : public EventBase
	{
	public:
		T* var;

		EventVar(T* var) :
			var(var)
		{
		}
	};

	class EventScript : public EventBase
	{
	public:
		std::vector<Sqrat::Function> funcs;
	//	Sqrat::Object obj;

		void Register(Sqrat::Function func)
		{
			funcs.push_back(func);
		}

		template<typename... P>
		void Trigger(P... args)
		{
			for(auto& f : funcs)
			{
				f.Execute(args...);
			}
		}
	};

	class EventScriptVar : public EventBase
	{
	public:
		Sqrat::Object var;

		EventScriptVar(Sqrat::Object var) :
			var(var)
		{
		}
	};

public:
	Object();
	virtual ~Object();

	virtual void Serialize(Properties* prop);

	/**
	 *	Loads a Properties structure from a file, then calling Load().
	 *	@param path File to load from.
	 */
	virtual void LoadFile(string path);

	/**
	 *	Creates a file and calls Save(), outputting to the file.
	 *	@param path File to save to.
	 */
	virtual void SaveFile(string path);

	static void ScriptBind();

	template<typename C>
	static void RegisterEventFunc(Event<>* evt, C* receiver, void (C::*func)())
	{
		receiver->eventConnections.push_back(evt->Register(boost::bind(func, receiver)));
	}

	template<typename C, typename P1>
	static void RegisterEventFunc(Event<P1>* evt, C* receiver, void (C::*func)(P1))
	{
		receiver->eventConnections.push_back(evt->Register(boost::bind(func, receiver, _1)));
	}

	template<typename C, typename P1, typename P2>
	static void RegisterEventFunc(Event<P1,P2>* evt, C* receiver, void (C::*func)(P1,P2))
	{
		receiver->eventConnections.push_back(evt->Register(boost::bind(func, receiver, _1, _2)));
	}

	template<typename C, typename P1, typename P2, typename P3>
	static void RegisterEventFunc(Event<P1,P2,P3>* evt, C* receiver, void (C::*func)(P1,P2,P3))
	{
		receiver->eventConnections.push_back(evt->Register(boost::bind(func, receiver, _1, _2, _3)));
	}

	template<typename C, typename P1, typename P2, typename P3, typename P4>
	static void RegisterEventFunc(Event<P1,P2,P3,P4>* evt, C* receiver, void (C::*func)(P1,P2,P3,P4))
	{
		receiver->eventConnections.push_back(evt->Register(boost::bind(func, receiver, _1, _2, _3, _4)));
	}

	template<typename R>
	static void ConnectEvent(Object* sender, string name, R* receiver, void (R::*func)())
	{
		erase_all(name, " ");
		name.append("()");
		ptr<EventBase>& evt = sender->events[name];
		if(evt && typeid(*evt) == typeid(Event<>))
		{
			Event<>* evtp = static_cast<Event<>*>(evt.get());
			RegisterEventFunc(evtp, receiver, func);
		}
		else
		{
			Event<>* evtp = new Event<>();
			RegisterEventFunc(evtp, receiver, func);
			evt = evtp;
		}
	}

	template<typename R, typename P1, typename... P>
	static void ConnectEvent(Object* sender, string name, R* receiver, void (R::*func)(P1, P...))
	{
		erase_all(name, " ");
		name.append("(" + type::Name<P1, P...>() + ")");
		std::cout << "connecting '" << name << "'\n";

		auto& evt = sender->events[name];
		Event<P1, P...>* evtp = nullptr;
		if(evt && typeid(*evt) == typeid(Event<P1, P...>))
		{
			 evtp = static_cast<Event<P1, P...>*>(evt.get());
		}
		else
		{
			evtp = new Event<P1, P...>();
			sender->events[name] = evtp;
		}

		if(evtp)
		{
			std::cout << "registering \n";
			RegisterEventFunc(evtp, receiver, func);
		}
	}

	void ClearEvent(string name)
	{
		auto& evt = events[name];
		if(evt)
		{
			evt->Clear();
		}
	}

	template<typename T>
	static void ConnectEvent(Object* sender, string name, T* var)
	{
		erase_all(name, " ");
		name.append("()");

		auto& evt = sender->events[name];
		EventVar<T>* con = nullptr;
		if(evt && typeid(*evt) == typeid(EventVar<T>))
		{
			con = static_cast<EventVar<T>*>(evt.get());
			con->var = var;
		}
		else
		{
			con = new EventVar<T>(var);
		}

		if(con)
		{
			evt = con;
		}
	}

	static void ConnectEvent(Object* sender, string name, Sqrat::Object var)
	{
	//	name.append("()");
	//	name = type::Name(sender) + "::" + name;
		auto& evt = sender->events[name];
		EventScriptVar* evtp = nullptr;
		if(evt && typeid(*evt) == typeid(EventScriptVar))
		{
			evtp = static_cast<EventScriptVar*>(evt.get());
		}
		else
		{
			evtp = new EventScriptVar(var);
		}

		if(evtp)
		{
			evtp->var = var;
			evt = evtp;
		}
	}

	void TriggerEvent(string name)
	{
		name.append("()");

		auto& evt = events[name];
		if(evt)
		{
			if(type::Compare<Event<>>(evt))
			{
				Event<>* con = static_cast<Event<>*>(evt.get());
				con->Trigger();
			}
			else if(typeid(*evt) == typeid(EventScript))
			{
				EventScript* con = static_cast<EventScript*>(evt.get());
				con->Trigger();
			}
		}
	}

	template<typename P1>
	void TriggerEvent(string name, P1 arg)
	{
		name.append("("+type::Name<P1>()+")");

		std::map<string, ptr<EventBase>>::const_iterator iter = events.find(name);
		if(iter != events.end())
		{
			auto& evt = iter->second;
			auto& ti = typeid(*evt);
			if(name.find("CollideEnter") != -1) std::cout << "triggering '" << name << "' on " << type::Name(evt) << "\n";
			if(type::Name(evt) == type::Name<Event<P1>>())
			{
				Event<P1>* con = static_cast<Event<P1>*>(evt.get());
				if(name.find("CollideEnter") != -1) std::cout << "triggering " << con->sig.num_slots() << " slots\n";
				con->Trigger(arg);
			}
			else if(ti == typeid(EventVar<P1>))
			{
				EventVar<P1>* conv = static_cast<EventVar<P1>*>(evt.get());
				if(conv)
				{
					*conv->var = arg;
				}
			}
			else if(ti == typeid(EventScriptVar))
			{
				EventScriptVar* conv = static_cast<EventScriptVar*>(evt.get());
				if(conv)
				{
					/// @todo implement this
				}
			}
			else if(ti == typeid(EventScript))
			{
				EventScript* con = static_cast<EventScript*>(evt.get());
				con->Trigger(arg);
			}
		}
	}


	template<typename P1, typename P2, typename... P>
	void TriggerEvent(string name, P1 arg1, P2 arg2, P... args)
	{
		name.append("(" + type::Name<P1, P2, P...>() + ")");

		std::map<string, ptr<EventBase>>::const_iterator iter = events.find(name);
		if(iter != events.end())
		{
			auto& evt = iter->second;
			if(typeid(*evt) == typeid(Event<P1,P2,P...>))
			{
				Event<P1, P2, P...>* con = static_cast<Event<P1, P2, P...>*>(evt.get());
				con->Trigger(arg1, arg2, args...);
			}
			else if(typeid(*evt) == typeid(EventScript))
			{
				EventScript* con = static_cast<EventScript*>(evt.get());
				con->Trigger(arg1, arg2, args...);
			}
		}
	}


	/**
	 *	Called when the object is initialised. Used instead of
	 *	constructor to ensure that all subsystems are created as well.
	 */
	virtual void Init();

	/**
	 *	Called every frame to update the object.
	 */
	virtual void Update();

	virtual void Render();

	virtual void FixedUpdate();

	/**
	 *	Call this to destroy the object.
	 */
	virtual void Destroy();

	virtual Game* GetGame() const { return nullptr; }

protected:
	struct Delete
	{
		void operator()(Object* obj)
		{
		//	obj->Destroy();
			delete obj;
		}
	};

	/**
	 *	Called just before the object is destroyed.
	 */
	virtual void Deinit();

	uint32 GetEventCount();


private:
	// For scripts
	void ConnectScriptEvent(Object* sender, string name, Sqrat::Object receiver, string funcName);

	std::map<string, ptr<EventBase>> events;
	std::vector<boost::signals2::connection> eventConnections;
};

/// @}
/// @}

} /* namespace lfant */
