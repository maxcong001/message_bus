#pragma once
#include <string>
#include <functional>
#include <map>
#include "common/Any.hpp"
#include "common/function_traits.hpp"
#include "common/NonCopyable.hpp"
#include "MThreadPool.hpp"

using namespace std;

class MessageBus: NonCopyable
{
public:
	MessageBus()
	{
		// suppose the new always success
		thread_pool_p = new ThreadPool(std::thread::hardware_concurrency()); 
	};
	~MessageBus()
	{
		if(thread_pool_p)
		{
			delete thread_pool_p;
		}
	}
	
	template<typename F>
	void Attach(F&& f, const string& strTopic="")
	{
		auto func = to_function(std::forward<F>(f));
		Add(strTopic, std::move(func));
	}

	
	template<typename R>
	void SendReq(const string& strTopic = "")
	{
		using function_type = std::function<R()>;
		string strMsgType =strTopic+ typeid(function_type).name();
		auto range = m_map.equal_range(strMsgType);
		for (Iterater it = range.first; it != range.second; ++it)
		{
			auto f = it->second.AnyCast < function_type >();
			thread_pool_p->enqueue(f);
		}
	}
	template<typename R, typename... Args>
	void SendReq(Args&&... args, const string& strTopic = "")
	{
		using function_type = std::function<R(Args...)>;
		string strMsgType =strTopic+ typeid(function_type).name();
		auto range = m_map.equal_range(strMsgType);
		for (Iterater it = range.first; it != range.second; ++it)
		{
			auto f = it->second.AnyCast < function_type >();
			thread_pool_p->enqueue(f, std::forward<Args>(args)...);
		}
	}

	
	template<typename R, typename... Args>
	void Remove(const string& strTopic = "")
	{
		using function_type = std::function<R(Args...)>; //typename function_traits<void(CArgs)>::stl_function_type;

		string strMsgType =strTopic +typeid(function_type).name();
		int count = m_map.count(strMsgType);
		auto range = m_map.equal_range(strMsgType);
		m_map.erase(range.first, range.second);
	}

private:
	template<typename F>
	void Add(const string& strTopic, F&& f)
	{
		string strMsgType = strTopic + typeid(F).name();
		m_map.emplace(std::move(strMsgType), std::forward<F>(f));
	}

private:
	std::multimap<string, Any> m_map;
	typedef std::multimap<string, Any>::iterator Iterater;

	ThreadPool* thread_pool_p;
};
