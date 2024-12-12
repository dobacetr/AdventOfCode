
#include <tuple>
#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include <utility>

namespace Utility
{
    template <typename T> concept IsContainer = 
        requires (T t)
        {
            {t[size_t{}]};
        };

    template <typename T> concept IsResizable = 
        requires(T t)
        {
            
            //{T::value_type};
            //{t.push_back(T::value_type())};
            {std::is_same_v<T,T>};
        };

    template <typename T> concept IsResizableContainer = IsContainer<T> && IsResizable<T>;
    
    template <typename T> concept IsOutStreamable = requires(T t)
    {
        {std::cout<<t};
    };

    template<typename tuple_t,IsContainer TContainer>
    constexpr auto get_container_from_tuple(tuple_t&& tuple)
    {
        constexpr auto get_container = [](auto&& ... x){ return TContainer{std::forward<decltype(x)>(x) ... }; };
        return std::apply(get_container, std::forward<tuple_t>(tuple));
    }

    template <IsOutStreamable ArgType, IsOutStreamable...ArgTypes> void Print(std::ostream& ostream, const ArgType& Arg, const ArgTypes&... Args)
    {
        ostream << Arg;
        ((ostream << " " << Args),...);
        ostream << std::endl;
    }
        
    template <typename Arg, typename...ArgTypes, std::size_t...Index> void Print(std::ostream& ostream, std::tuple<Arg, ArgTypes...> Tuple, std::index_sequence<Index...>)
    {
        ostream << std::get<0>(Tuple);
        ((ostream << " " << std::get<Index+1>(Tuple)), ...);
        ostream << std::endl;
    }

    template <typename...ArgTypes> void Print(std::ostream& ostream, std::tuple<ArgTypes...> Tuple)
    {
        Print(ostream, Tuple, std::make_index_sequence<sizeof...(ArgTypes)-1>{});
    }

    template <IsContainer...ArgTypes> void PrintAtIdx_Checked(std::ostream& ostream,
    const size_t idx, const ArgTypes&... Args)
    {
        Print(ostream, Args[idx]...);
    }

    template <IsContainer...ArgTypes> void PrintAtIdx(std::ostream& ostream,
    const size_t idx, const ArgTypes&... Args)
    {
        bool bAllHasIdx = ((idx < Args.size()) && ...);
        if(bAllHasIdx)
        {
            PrintAtIdx_Checked(ostream, idx, Args...);
        }
    }

    template <IsContainer...ArgTypes> void PrintAll(std::ostream& ostream, const ArgTypes&... Args)
    {
        size_t minSize = std::min({Args.size()...});
        for(size_t idx = 0; idx < minSize; ++idx)
        {
            PrintAtIdx_Checked(ostream, idx, Args...);
        }
    }

    std::vector<std::string> Split(const std::string& s)
    {
        std::stringstream ss(s);
        std::vector<std::string> words;
        for (std::string w; ss>>w; ) 
        {
            words.push_back(w);
        }
        return words;
    }

    template <typename T> T FromString(const std::string& Token);

    template<typename T> using FuncTFromStr = T(const std::string&);

    template <typename...ArgTypes, std::size_t...Index> std::tuple<ArgTypes...> ParseLineToTuple(const std::string& line, std::index_sequence<Index...>, const FuncTFromStr<ArgTypes>*...Funcs)
    {
        std::vector<std::string> words = Split(line);

        std::tuple<ArgTypes...> result{Funcs(words[Index])...};
        
        return result;
    }

    template <typename...ArgTypes> std::tuple<ArgTypes...> 
        ParseLine(const std::string& line, FuncTFromStr<ArgTypes>*...Funcs)
    {
        return ParseLine(line, std::make_integer_sequence<size_t, sizeof...(ArgTypes)>{}, Funcs...);
    }

    template <typename...ArgTypes> std::tuple<ArgTypes...> ParseLine(const std::string& line)
    {
        return ParseLine(line, FromString<ArgTypes>...);
    }

    template <typename T> std::vector<T> ParseLineToVector(const std::string& line, const FuncTFromStr<T>* Func)
    {
        std::vector<std::string> words = Split(line);

        std::vector<T> values;
        values.reserve(words.size());

        for(const std::string& word : words)
        {
            const T value = Func(word);
            values.push_back( value );
        }

        return values;
    }

    template <typename T> std::vector<T> ParseLineToVector(const std::string& line)
    {
        std::vector<std::string> words = Split(line);
        return ParseLineToVector(line, FromString<T>);
    }

    template <typename...ArgTypes, std::size_t...Index> void ParseLineInto(const std::string& line,  std::index_sequence<Index...>, ArgTypes&...OutArgs)
    {
        std::tuple<ArgTypes...> Result = ParseLine<ArgTypes...>(line);
        ((OutArgs = std::get<Index>(Result)),...);
    }

    template <typename...ArgTypes> void ParseLineInto(const std::string& line, ArgTypes&...OutArgs)
    {
        ParseLineInto(line, std::make_index_sequence<sizeof...(ArgTypes)>{}, OutArgs...);
    }

    template<typename T> void ResizeIfSmaller(std::vector<T>& InOutVec, const size_t idx)
    {
        while(InOutVec.size()<=idx)
        {
            InOutVec.push_back(T{});
        }
    }

    template <IsResizableContainer...ArgTypes> 
        bool ParseLineIntoIdx(std::istream& istream, const size_t idx, ArgTypes&...Args)
        {
            std::string line;
            if(std::getline(istream, line))
            {
                const size_t Unused[] = {(ResizeIfSmaller(Args, idx), Args.size())...};
                (void)Unused;
                ParseLineInto(line, Args[idx]...);
                return true;
            }

            return false;
        }
    
    template<typename T> std::vector<std::vector<T>> Parse(std::istream& istream){
        std::vector<std::vector<T>> result;

        std::string line;
        while(std::getline(istream, line))
        {
            const std::vector<T> arrCurr = ParseLineToVector<T>(line);
            result.push_back( arrCurr );

        }

        return result;
    }

    template <IsResizableContainer...ArgTypes> 
        void Parse(std::istream& istream, ArgTypes&...Args) 
    {
        size_t idx = 0;

        while( ParseLineIntoIdx(istream, idx, Args...) )
        {
            ++idx;
        }
    }
}

// Can define custom conversions 
template<> int Utility::FromString(const std::string& Token)
{
    return std::stoi(Token);
}

