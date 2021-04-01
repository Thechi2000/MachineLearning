#pragma once
#include <string>

#define for_in_range(iteration_variable_name, initial_value, iterations_count) for(unsigned int iteration_variable_name = initial_value; iteration_variable_name < iterations_count; ++iteration_variable_name)
#define THROW_IF(cond) if (cond) throw std::exception((std::string("Error at ") + std::string(__FILE__) + std::to_string(__LINE__)).c_str())
