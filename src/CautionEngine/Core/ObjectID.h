#pragma once
#include "stdafx.h"
#include <CautionEngineDefinitions.h>

#include <chrono>

struct CAUTION_ENGINE_CLASS SObjectID
{
	SObjectID() {}

    uint64_t first;
    uint64_t mid;
    uint64_t last;

	static uint64_t Generate()
	{
		// TODO: implement...
        static std::mt19937_64 eng{ std::random_device{}() };

        auto time_now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        auto thread_id = std::hash<std::thread::id>()(std::this_thread::get_id());
        auto random_value = eng();

        uint64_t a = static_cast<uint64_t>(time_now);
        uint64_t b = static_cast<uint64_t>(thread_id);
        uint64_t c = random_value;

        // mix function based on Knuth's multiplicative method
        // https://gist.github.com/badboy/6267743
        a *= 2654435761u;
        b *= 2654435761u;
        c *= 2654435761u;

        a ^= (a >> 16);
        b ^= (b >> 16);
        c ^= (c >> 16);

		return a + b + c;
	}

	static SObjectID FromString(const std::string& objectIdStr)
	{
		return SObjectID();
	}
};