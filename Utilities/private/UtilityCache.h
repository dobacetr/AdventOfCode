#include <string>
#include <array>

template<typename T> static constexpr std::array<std::string, static_cast<size_t>(T::Count)> ConstructStrings()
{
    constexpr size_t iMax = static_cast<size_t>(T::Count);

    std::array<std::string, iMax> Strings;

    for( size_t i = 0; i < iMax; ++i )
    {
        Strings[i] = GetString_UnCached( static_cast<T>(i) );
    }
    return Strings;
}

// Define static constexpr std::string GetString_UnCached(const T inEnum) to construct the cache of strings
template<typename T> static constexpr std::string& GetString(const T InEnum)
{
    static auto Strings = ConstructStrings<T>();
    return Strings[static_cast<size_t>(InEnum)];
}