#include <utility>

#include "patternscanning.h"
#include "process.h"

#include <cstdlib>
#include <cstring>

XKLib::PatternByte::Value::Value(int value) : value(value)
{
}

XKLib::PatternByte::PatternByte(std::vector<std::shared_ptr<Value>> values,
                                std::string areaName,
                                std::vector<ptr_t> matches)
 : _values(std::move(values)), _matches(std::move(matches)),
   _area_name(std::move(areaName)), _fast_aligned_values_count(0)
{
    if (!isValid())
    {
        XKLIB_EXCEPTION("Invalid pattern.");
    }

    /**
     *  Let's do some preprocessing now *
     */

    {
        _unknown_values.reserve(_values.size());

        size_t index                       = 0;
        size_t ukval_contigous_count       = 0;
        size_t index_since_contigous_count = 0;
        size_t byte_simd_index             = 0;
        size_t count_unknown_byte          = 0;
        bool are_known_values              = true;
        std::vector<byte_t> known_values;
        _fast_aligned_masks = view_as<simd_value_t*>(
          std::aligned_alloc(sizeof(simd_value_t), _values.size()));
        _fast_aligned_values = view_as<simd_value_t*>(
          std::aligned_alloc(sizeof(simd_value_t), _values.size()));
        simd_value_t simd_value {}, simd_mask {};

        for (auto&& value : _values)
        {
            value->index = index;

            if (value->value == Value::UNKNOWN)
            {
                index_since_contigous_count = index;
                ukval_contigous_count++;

                if (ukval_contigous_count >= sizeof(simd_value_t))
                {
                    _unknown_values.push_back(
                      { index_since_contigous_count / sizeof(simd_value_t),
                        index_since_contigous_count % sizeof(simd_value_t),
                        index_since_contigous_count,
                        ukval_contigous_count });
                    ukval_contigous_count = 0;
                }

                if (are_known_values)
                {
                    _vec_known_values.push_back(known_values);
                    known_values.clear();
                    are_known_values = false;
                }

                count_unknown_byte++;
            }
            else
            {
                if (!are_known_values)
                {
                    _vec_skipper_uk_values.push_back(count_unknown_byte);
                    are_known_values = true;
                }

                known_values.push_back(view_as<byte_t>(value->value));

                if (ukval_contigous_count > 0)
                {
                    _unknown_values.push_back(
                      { index_since_contigous_count / sizeof(simd_value_t),
                        index_since_contigous_count % sizeof(simd_value_t),
                        index_since_contigous_count,
                        ukval_contigous_count });
                    ukval_contigous_count = 0;
                }

                view_as<byte_t*>(&simd_value)[byte_simd_index] = view_as<
                  byte_t>(value->value);
                view_as<byte_t*>(&simd_mask)[byte_simd_index] = 0xFF;
            }

            index++;
            byte_simd_index++;

            if (byte_simd_index >= sizeof(simd_value_t))
            {
#if defined(__AVX512F__)
                _mm512_store_si512(
                  &_fast_aligned_values[_fast_aligned_values_count],
                  simd_value);
                _mm512_store_si512(
                  &_fast_aligned_masks[_fast_aligned_values_count],
                  simd_mask);
#elif defined(__AVX2__)
                _mm256_store_si256(
                  &_fast_aligned_values[_fast_aligned_values_count],
                  simd_value);
                _mm256_store_si256(
                  &_fast_aligned_masks[_fast_aligned_values_count],
                  simd_mask);
#else
                _fast_aligned_values[_fast_aligned_values_count] = simd_value;
                _fast_aligned_masks[_fast_aligned_values_count] = simd_mask;
#endif
                std::memset(&simd_value, 0, sizeof(simd_value));
                std::memset(&simd_mask, 0, sizeof(simd_mask));
                byte_simd_index = 0;
                _fast_aligned_values_count++;
            }
        }

        if (known_values.size())
        {
            _vec_known_values.push_back(known_values);
        }

        if (byte_simd_index > 0)
        {
#if defined(__AVX512F__)
            _mm512_store_si512(
              &_fast_aligned_values[_fast_aligned_values_count],
              simd_value);
            _mm512_store_si512(
              &_fast_aligned_masks[_fast_aligned_values_count],
              simd_mask);
#elif defined(__AVX2__)
            _mm256_store_si256(
              &_fast_aligned_values[_fast_aligned_values_count],
              simd_value);
            _mm256_store_si256(
              &_fast_aligned_masks[_fast_aligned_values_count],
              simd_mask);
#else
            _fast_aligned_values[_fast_aligned_values_count] = simd_value;
            _fast_aligned_masks[_fast_aligned_values_count] = simd_mask;
#endif
            _fast_aligned_values_count++;
        }
    }

    if (_vec_skipper_uk_values.size() != (_vec_known_values.size() - 1))
    {
        XKLIB_EXCEPTION("The amount of vec known values should be the "
                        "same of vec of unknown values");
    }
}

XKLib::PatternByte::~PatternByte()
{
    std::free(_fast_aligned_values);
    std::free(_fast_aligned_masks);
}

auto XKLib::PatternByte::values() -> std::vector<std::shared_ptr<Value>>&
{
    return _values;
}

auto XKLib::PatternByte::simd_unknown_values()
  -> std::vector<simd_unknown_value_t>&
{
    return _unknown_values;
}

auto XKLib::PatternByte::matches() -> std::vector<ptr_t>&
{
    return _matches;
}

auto XKLib::PatternByte::isValid() -> bool
{
    if (_values.size() == 0)
    {
        return false;
    }

    for (auto&& byte : _values)
    {
        if (byte->value == Value::INVALID)
        {
            return false;
        }
    }

    /* ? xx xx ... */
    if (_values[0]->value == Value::UNKNOWN)
    {
        return false;
    }

    /* xx xx ? */
    if (_values[_values.size() - 1]->value == Value::UNKNOWN)
    {
        return false;
    }

    return true;
}

auto XKLib::PatternByte::scan(Process& process) -> void
{
    PatternScanning::searchInProcess(*this, process);
}

auto XKLib::PatternByte::areaName() -> std::string
{
    return _area_name;
}

auto XKLib::PatternByte::vec_known_values()
  -> std::vector<std::vector<byte_t>>&
{
    return _vec_known_values;
}

auto XKLib::PatternByte::vec_skipper_uk_values() -> std::vector<size_t>&
{
    return _vec_skipper_uk_values;
}

auto XKLib::PatternByte::fast_aligned_values() -> simd_value_t*
{
    return _fast_aligned_values;
}

auto XKLib::PatternByte::fast_aligned_masks() -> simd_value_t*
{
    return _fast_aligned_masks;
}

auto XKLib::PatternByte::fast_aligned_values_count() -> size_t
{
    return _fast_aligned_values_count;
}
