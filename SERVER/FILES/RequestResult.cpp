#include "RequestResult.h"
#include "IRequestHandler.h"  
#include "JsonResponsePacketSerializer.h"


RequestResult::RequestResult(Buffer buffer, std::unique_ptr<IRequestHandler> nextHandler)
    : response(std::move(buffer)), newHandler(std::move(nextHandler)) {}


RequestResult createErrorResult(const std::string& message)
{
    ErrorResponse error{ message };
    auto buffer = JsonResponsePacketSerializer::serializeResponse(error);
    return RequestResult(buffer, nullptr);
}
