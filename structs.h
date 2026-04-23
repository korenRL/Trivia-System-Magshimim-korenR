#pragma once
#include <string>

enum ResponseCode : unsigned char {
    ERROR_RES = 10,
    LOGIN_RES = 11,
    SIGNUP_RES = 12
};

struct ErrorResponse {
    std::string message;
};

struct LoginResponse {
    unsigned int status;
};

struct SignupResponse {
    unsigned int status;
};
