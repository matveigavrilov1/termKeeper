#include "utils/generate_uuid.h"

uuids::uuid utils::generate_uuid()
{
	static std::random_device rd;
	static auto seed_data = std::array<int, std::mt19937::state_size> {};
	std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
	static std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
	static std::mt19937 generator(seq);
	static uuids::uuid_random_generator gen { generator };

	return gen();
}