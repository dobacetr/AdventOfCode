#include <iostream>
#include <string>
#include <fstream>

#include <vector>

#include "Utility.h"

#include <string_view>
#include <algorithm>

// Must be sequential
// Invalid must be 0 and EndLine must be 1
enum class EOperation{
    Invalid,    // Do not change its value!
    EndLine,    // Do not change its value!
    Mul,
    Count
};

static constexpr std::string GetString_UnCached(const EOperation inOp)
{
    switch(inOp)
    {
        case EOperation::Invalid:
            return "";
        case EOperation::EndLine:
            return "\n";
        case EOperation::Mul:
            return "mul(";
        case EOperation::Count:
            return "";
    }

    return "";
}

struct OperationToken{
    EOperation op;
    int left;
    int right;
};

static void UpdateTokenBegins(const std::string_view inStr,
    std::array<std::string_view::iterator, static_cast<size_t>(EOperation::Count)>& InOut_tokenBegins)
{
    for(size_t idxOp = static_cast<size_t>(EOperation::EndLine); idxOp < InOut_tokenBegins.size(); ++idxOp)
    {
        std::string_view::iterator& tokenBegin = InOut_tokenBegins[idxOp];
        
        // We consider tokenBegin valid, even if it is past the strEnd.
        //      It means that the token does not exist
        const bool bTokenWasConsumed = tokenBegin<inStr.begin();
        if( bTokenWasConsumed )
        {
            const std::string& tokenStr = GetString(static_cast<EOperation>(idxOp));

            size_t idxNew = inStr.find(tokenStr);
            if(idxNew!=std::string_view::npos)
            {
                InOut_tokenBegins[idxOp] = inStr.begin()+idxNew;
            }
            else
            {
                InOut_tokenBegins[idxOp] = inStr.end();
            }
        }
    }
}
static bool StringToInt(const std::string_view inStr, int& outResult)
{
    // Dont accept white space
    if(inStr.front()==' ')
    {
        return false;
    }

    const std::string inStrCopy(inStr);
    // C way of parsing long
    char* pEnd;
    outResult = strtol(inStrCopy.c_str(), &pEnd, 10);
    if(*pEnd!=0)
    {
        return false;
    }

    return true;
}

static OperationToken ConsumeNextToken(std::string_view& inOut_str,
 std::array<std::string_view::iterator, static_cast<size_t>(EOperation::Count)>& InOut_tokenBegins)
{
    constexpr size_t minArgLength = 1;
    constexpr size_t maxArgLength = 3;

    OperationToken Result{
            EOperation::Invalid, 0, 0
        };

    // Early out if string is empty
    if(inOut_str.empty())
    {
        Result.op = EOperation::EndLine;
        return Result;
    }

    // Update Token begins
    UpdateTokenBegins(inOut_str, InOut_tokenBegins);

    // Find the first token we have

    // Default token if we dont find any
    size_t idxOp = static_cast<size_t>(EOperation::EndLine);
    for( size_t idxOp_ = static_cast<size_t>(EOperation::EndLine); idxOp_ < static_cast<size_t>(EOperation::Count); ++idxOp_ )
    {
        const bool bIsBeforeIdxOp = InOut_tokenBegins[idxOp_] < InOut_tokenBegins[idxOp];
        // branchless update for idx
        idxOp = !bIsBeforeIdxOp * idxOp + bIsBeforeIdxOp*idxOp_;
    }

    const EOperation opType = static_cast<EOperation>(idxOp);

    if(opType==EOperation::EndLine)
    {
        // Consume entire string and return
        inOut_str.substr(inOut_str.length());
        Result.op=EOperation::EndLine;
        return Result;
    }

    const size_t opStrLen = GetString(opType).length();

    // Comsume string until the end of the op string
    inOut_str = std::string_view(InOut_tokenBegins[idxOp]+opStrLen);

    // Early out if string is finished
    if(inOut_str.empty())
    {
        return Result;
    }

    // We need a comma
    const size_t idxComma = inOut_str.find(",");
    const bool bCommaFound = idxComma != std::string_view::npos;
    const bool bLeftArgLengthIsValid = idxComma>=minArgLength && idxComma<=maxArgLength;

    if(!bCommaFound || !bLeftArgLengthIsValid)
    {
        return Result;
    }

    // Try to extract left arg
    const std::string_view leftString = inOut_str.substr(0, idxComma);

    const bool bLeftIsValid = StringToInt(leftString, Result.left);
    if(!bLeftIsValid)
    {
        return Result;
    }

    // Try to extract right arg
    const size_t idxParanthesisClose = inOut_str.find(')');
    const bool bCloserFound = idxParanthesisClose != std::string_view::npos;
    const size_t rightArgLength = idxParanthesisClose-idxComma-1;
    const bool bRightArgLengthIsValid = rightArgLength>=minArgLength && rightArgLength<=maxArgLength;

    if(!bCloserFound||!bRightArgLengthIsValid)
    {
        return Result;
    }

    const std::string_view rightString = inOut_str.substr(idxComma+1, rightArgLength);
    
    const bool bRightIsValid = StringToInt(rightString, Result.right);
    if(!bRightIsValid)
    {
        return Result;
    }


    // Consume str to the end of op
    inOut_str = inOut_str.substr(idxParanthesisClose+1);

    std::string_view OpLine(InOut_tokenBegins[idxOp], inOut_str.begin());

    std::cout << "Found: " << OpLine << std::endl;

    Result.op = opType;
    return Result;
}

static std::vector<OperationToken> Tokenize(const std::string& line)
{
    std::vector<OperationToken> tokens;

    std::string_view inStr(line.begin(), line.end());
    std::array<std::string_view::iterator, static_cast<size_t>(EOperation::Count)> tokenBegins;
    // Set all tokens to 'consumed' status
    std::fill_n(tokenBegins.begin(), tokenBegins.size(), inStr.begin()-1);

    tokenBegins[static_cast<size_t>(EOperation::Invalid)] = inStr.end();


    bool bTokenNotEndLine = true;
    while(bTokenNotEndLine)
    {

        const OperationToken newToken = ConsumeNextToken(inStr, tokenBegins);
        const bool bTokenValid = newToken.op != EOperation::Invalid;
        if(bTokenValid)
        {
            tokens.push_back(newToken);
        }
        bTokenNotEndLine = newToken.op != EOperation::EndLine;
    }

    return tokens;
}

static int ProcessOperation(const OperationToken& inToken)
{
    switch(inToken.op)
    {
        case EOperation::Invalid:
        case EOperation::EndLine:
        case EOperation::Count:
            return 0;
        case EOperation::Mul:
            return inToken.left * inToken.right;
    }

    return 0;
}

int main()
{
    // Read From file if exists
    std::istream* inStream = &std::cin;

    std::ifstream file( std::string(InputFilePath));
    if(file.is_open())
    {
        inStream = &file;
    }

    std::string line;
    int result = 0;

    int lineNo = 0;

    while(std::getline(*inStream, line))
    {
        std::cout << "Line " << lineNo++ << ":" << std::endl;

        std::vector<OperationToken> tokens = Tokenize(line);
        for(const auto&token : tokens)
        {
            result+=ProcessOperation(token);
        }
    }

    std::cout << result;

    return 0;
}