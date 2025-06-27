#pragma once
#include "Buffer.h"
#include <memory>
#include <iostream>
//#include "IRequestHandler.h"


class IRequestHandler;

struct RequestResult
{
    Buffer response;
    std::unique_ptr<IRequestHandler> newHandler;

    RequestResult(Buffer res, std::unique_ptr<IRequestHandler> handler);
};

RequestResult createErrorResult(const std::string& message);

